// mini-tmux: A simplified terminal multiplexer
// Single-file C++17 implementation: Server/Client architecture with PTY, Unix domain socket, poll()

#include <algorithm>
#include <cerrno>
#include <climits>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fcntl.h>
#include <map>
#include <poll.h>
#include <pty.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

// ============================================================
// Protocol
// ============================================================

// Wire format: [4 bytes type][4 bytes payload_length][payload...]
enum MsgType : uint32_t {
    MSG_HELLO       = 1,   // C->S: {u8 readonly, u16 rows, u16 cols}
    MSG_INPUT       = 2,   // C->S: raw bytes
    MSG_OUTPUT      = 3,   // S->C: {i32 pane_id, raw bytes...}
    MSG_WINSIZE     = 4,   // C->S: {u16 rows, u16 cols}
    MSG_COMMAND     = 5,   // C->S: command string
    MSG_CMD_RESULT  = 6,   // S->C: result string
    MSG_PANE_EVENT  = 7,   // S->C: {u8 event, i32 pane_id, i32 focused_id}
    MSG_DETACH      = 8,   // C->S: (no payload)
    MSG_SERVER_EXIT = 9,   // S->C: (no payload)
};

enum PaneEvent : uint8_t {
    EVT_PANE_CREATED   = 1,
    EVT_PANE_DESTROYED = 2,
    EVT_FOCUS_CHANGED  = 3,
};

enum HelloMode : uint8_t {
    HELLO_ATTACH      = 0,
    HELLO_NEW_SESSION = 1,
};

static const size_t HDR_SZ = 8; // sizeof(type) + sizeof(length)

static std::string msg_build(MsgType t, const void* data, size_t len) {
    std::string m(HDR_SZ + len, '\0');
    auto* h = reinterpret_cast<uint32_t*>(&m[0]);
    h[0] = t;
    h[1] = static_cast<uint32_t>(len);
    if (len && data) memcpy(&m[HDR_SZ], data, len);
    return m;
}
static std::string msg_build(MsgType t, const std::string& s) {
    return msg_build(t, s.data(), s.size());
}

static std::string msg_hello(bool ro, HelloMode mode, int32_t session_id, uint16_t rows, uint16_t cols) {
    uint8_t buf[10];
    buf[0] = ro ? 1 : 0;
    buf[1] = static_cast<uint8_t>(mode);
    memcpy(buf + 2, &session_id, 4);
    memcpy(buf + 6, &rows, 2);
    memcpy(buf + 8, &cols, 2);
    return msg_build(MSG_HELLO, buf, sizeof(buf));
}

static std::string msg_output(int32_t pane_id, const char* data, size_t len) {
    std::string payload(4 + len, '\0');
    memcpy(&payload[0], &pane_id, 4);
    if (len) memcpy(&payload[4], data, len);
    return msg_build(MSG_OUTPUT, payload);
}

static std::string msg_winsize(uint16_t rows, uint16_t cols) {
    uint8_t buf[4];
    memcpy(buf, &rows, 2);
    memcpy(buf + 2, &cols, 2);
    return msg_build(MSG_WINSIZE, buf, 4);
}

static std::string msg_pane_event(PaneEvent e, int32_t pid, int32_t fid) {
    uint8_t buf[9];
    buf[0] = e;
    memcpy(buf + 1, &pid, 4);
    memcpy(buf + 5, &fid, 4);
    return msg_build(MSG_PANE_EVENT, buf, 9);
}

// ============================================================
// Utility
// ============================================================

static std::string get_socket_name() {
    const char* v = getenv("MINI_TMUX_SERVER");
    return (v && *v) ? std::string(v) : std::string("default");
}

static std::string get_socket_path() {
    return std::string("/tmp/mini-tmux-") + get_socket_name() + ".sock";
}

static std::string get_socket_abstract_name() {
    return "mini-tmux:" + get_socket_name();
}

static bool fill_socket_addr(sockaddr_un& addr, socklen_t& addr_len, const std::string& value, bool abstract) {
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if (abstract) {
        if (value.size() + 1 > sizeof(addr.sun_path)) return false;
        addr.sun_path[0] = '\0';
        memcpy(addr.sun_path + 1, value.data(), value.size());
        addr_len = static_cast<socklen_t>(offsetof(sockaddr_un, sun_path) + 1 + value.size());
        return true;
    }
    if (value.size() >= sizeof(addr.sun_path)) return false;
    strncpy(addr.sun_path, value.c_str(), sizeof(addr.sun_path) - 1);
    addr_len = sizeof(addr);
    return true;
}

static bool try_connect_socket(int fd, const std::string& value, bool abstract) {
    sockaddr_un addr{};
    socklen_t addr_len = 0;
    if (!fill_socket_addr(addr, addr_len, value, abstract)) return false;
    return connect(fd, (sockaddr*)&addr, addr_len) == 0;
}

static bool socket_available(const std::string& spath) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return false;
    bool ok = try_connect_socket(fd, spath, false) ||
              try_connect_socket(fd, get_socket_abstract_name(), true);
    close(fd);
    return ok;
}

static void set_nonblock(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

static void set_cloexec(int fd) {
    fcntl(fd, F_SETFD, fcntl(fd, F_GETFD, 0) | FD_CLOEXEC);
}

static bool try_write(int fd, const char* p, size_t n) {
    while (n > 0) {
        ssize_t w = write(fd, p, n);
        if (w < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        p += w;
        n -= w;
    }
    return true;
}

static bool flush_buf(int fd, std::string& buf) {
    while (!buf.empty()) {
        ssize_t w = write(fd, buf.data(), buf.size());
        if (w < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) return true;
            return false;
        }
        buf.erase(0, w);
    }
    return true;
}

// ============================================================
// Signal infrastructure (self-pipe)
// ============================================================

static int g_sigpipe_rd = -1, g_sigpipe_wr = -1;
static volatile sig_atomic_t g_sigchld = 0;
static volatile sig_atomic_t g_sigwinch = 0;

static void sig_write(char c) {
    int e = errno;
    if (g_sigpipe_wr >= 0) {
        ssize_t r;
        do { r = write(g_sigpipe_wr, &c, 1); } while (r < 0 && errno == EINTR);
    }
    errno = e;
}

static void on_sigchld(int) { g_sigchld = 1; sig_write('C'); }
static void on_sigwinch(int) { g_sigwinch = 1; sig_write('W'); }

static void drain_sigpipe() {
    char buf[128];
    while (read(g_sigpipe_rd, buf, sizeof(buf)) > 0) {}
}

static void make_sigpipe() {
    int p[2];
    if (pipe(p) < 0) { perror("pipe"); return; }
    g_sigpipe_rd = p[0];
    g_sigpipe_wr = p[1];
    set_nonblock(g_sigpipe_rd);
    set_nonblock(g_sigpipe_wr);
    set_cloexec(g_sigpipe_rd);
    set_cloexec(g_sigpipe_wr);
}

// ============================================================
// Pane
// ============================================================

static const int CAPTURE_MAX_LINES = 1000;

struct ScreenBuffer {
    std::deque<std::string> lines;
    size_t cur_row = 0;
    size_t cur_col = 0;
    int max_lines = CAPTURE_MAX_LINES;
    enum EscState { ESC_NONE, ESC_SEEN, ESC_CSI, ESC_OSC, ESC_OSC_ESC } esc_state = ESC_NONE;
    std::string esc_buf;

    ScreenBuffer() {
        lines.emplace_back();
    }

    void ensure_cursor() {
        if (lines.empty()) lines.emplace_back();
        while (lines.size() <= cur_row) lines.emplace_back();
        while ((int)lines.size() > max_lines) {
            lines.pop_front();
            if (cur_row > 0) cur_row--;
        }
        if (lines.empty()) {
            lines.emplace_back();
            cur_row = 0;
        }
        if (cur_row >= lines.size()) cur_row = lines.size() - 1;
    }

    std::string& current_line() {
        ensure_cursor();
        return lines[cur_row];
    }

    void line_feed() {
        cur_row++;
        cur_col = 0;
        ensure_cursor();
    }

    void write_char(char ch) {
        std::string& line = current_line();
        if (cur_col > line.size()) line.resize(cur_col, ' ');
        if (cur_col == line.size()) line.push_back(ch);
        else line[cur_col] = ch;
        cur_col++;
    }

    void clear_all() {
        lines.clear();
        lines.emplace_back();
        cur_row = 0;
        cur_col = 0;
    }

    void clear_to_end() {
        std::string& line = current_line();
        if (cur_col < line.size()) line.erase(cur_col);
        for (size_t i = cur_row + 1; i < lines.size(); i++) lines[i].clear();
    }

    void clear_to_start() {
        ensure_cursor();
        for (size_t i = 0; i < cur_row; i++) lines[i].clear();
        std::string& line = lines[cur_row];
        if (cur_col >= line.size()) line.resize(cur_col + 1, ' ');
        size_t upto = std::min(cur_col + 1, line.size());
        for (size_t i = 0; i < upto; i++) line[i] = ' ';
    }

    void clear_line(int mode) {
        std::string& line = current_line();
        if (mode == 2) {
            line.clear();
        } else if (mode == 1) {
            if (cur_col >= line.size()) line.resize(cur_col + 1, ' ');
            size_t upto = std::min(cur_col + 1, line.size());
            for (size_t i = 0; i < upto; i++) line[i] = ' ';
        } else if (cur_col < line.size()) {
            line.erase(cur_col);
        }
    }

    void move_row(int delta) {
        int next = (int)cur_row + delta;
        if (next < 0) next = 0;
        cur_row = (size_t)next;
        ensure_cursor();
    }

    void set_cursor(size_t row, size_t col) {
        cur_row = row;
        cur_col = col;
        ensure_cursor();
    }

    static std::vector<int> parse_params(const std::string& s) {
        std::vector<int> out;
        if (s.empty()) return out;
        size_t start = 0;
        while (start <= s.size()) {
            size_t end = s.find(';', start);
            std::string part = s.substr(start, end == std::string::npos ? std::string::npos : end - start);
            out.push_back(part.empty() ? 0 : std::atoi(part.c_str()));
            if (end == std::string::npos) break;
            start = end + 1;
        }
        return out;
    }

    static int param_or(const std::vector<int>& params, size_t idx, int def) {
        if (idx >= params.size() || params[idx] == 0) return def;
        return params[idx];
    }

    void handle_csi(char final_byte) {
        std::vector<int> params = parse_params(esc_buf);
        if (final_byte == 'J') {
            int mode = param_or(params, 0, 0);
            if (mode == 2 || mode == 3) {
                clear_all();
            } else if (mode == 0) {
                clear_to_end();
            } else if (mode == 1) {
                clear_to_start();
            }
        } else if (final_byte == 'K') {
            clear_line(param_or(params, 0, 0));
        } else if (final_byte == 'A') {
            move_row(-std::max(1, param_or(params, 0, 1)));
        } else if (final_byte == 'B') {
            move_row(std::max(1, param_or(params, 0, 1)));
        } else if (final_byte == 'G') {
            cur_col = std::max(0, param_or(params, 0, 1) - 1);
            ensure_cursor();
        } else if (final_byte == 'C') {
            cur_col += std::max(1, param_or(params, 0, 1));
            ensure_cursor();
        } else if (final_byte == 'D') {
            int amount = std::max(1, param_or(params, 0, 1));
            cur_col = (cur_col >= (size_t)amount) ? (cur_col - amount) : 0;
        } else if (final_byte == 'H' || final_byte == 'f') {
            int row = param_or(params, 0, 1);
            int col = param_or(params, 1, 1);
            set_cursor((size_t)std::max(0, row - 1), (size_t)std::max(0, col - 1));
        }
        esc_buf.clear();
    }

    void feed(const char* data, size_t len) {
        for (size_t i = 0; i < len; i++) {
            unsigned char c = static_cast<unsigned char>(data[i]);
            switch (esc_state) {
            case ESC_NONE:
                if (c == '\n') {
                    line_feed();
                } else if (c == '\r') {
                    cur_col = 0;
                } else if (c == '\b') {
                    if (cur_col > 0) cur_col--;
                } else if (c == '\t') {
                    size_t next = ((cur_col / 8) + 1) * 8;
                    std::string& line = current_line();
                    if (next > line.size()) line.resize(next, ' ');
                    cur_col = next;
                } else if (c == 0x1b) {
                    esc_state = ESC_SEEN;
                    esc_buf.clear();
                } else if (c >= 32) {
                    write_char(static_cast<char>(c));
                }
                break;
            case ESC_SEEN:
                if (c == '[') {
                    esc_state = ESC_CSI;
                    esc_buf.clear();
                } else if (c == ']') {
                    esc_state = ESC_OSC;
                } else {
                    esc_state = ESC_NONE;
                }
                break;
            case ESC_CSI:
                if (c >= 0x40 && c <= 0x7e) {
                    handle_csi((char)c);
                    esc_state = ESC_NONE;
                } else {
                    esc_buf.push_back((char)c);
                }
                break;
            case ESC_OSC:
                if (c == '\a') esc_state = ESC_NONE;
                else if (c == 0x1b) esc_state = ESC_OSC_ESC;
                break;
            case ESC_OSC_ESC:
                esc_state = (c == '\\') ? ESC_NONE : ESC_OSC;
                break;
            }
        }
    }

    std::vector<std::string> visible_lines() const {
        ssize_t last = -1;
        for (size_t i = 0; i < lines.size(); i++) {
            if (!lines[i].empty()) last = static_cast<ssize_t>(i);
        }
        if (last < 0) return {};
        return std::vector<std::string>(lines.begin(), lines.begin() + last + 1);
    }

    std::string dump_text() const {
        std::string out;
        auto snapshot = visible_lines();
        for (const auto& line : snapshot) {
            out += line;
            out += '\n';
        }
        return out;
    }
};

struct Pane {
    int id = -1;
    int session_id = -1;
    int master_fd = -1;
    pid_t child_pid = -1;
    bool alive = false;
    int pane_rows = 24, pane_cols = 80;

    int log_fd = -1;
    int pipeout_wr = -1;
    pid_t pipeout_pid = -1;
    std::string pipeout_buf;

    ScreenBuffer screen;

    std::string input_buf;

    std::string cap_dump() const {
        return screen.dump_text();
    }

    void stop_log() {
        if (log_fd >= 0) { close(log_fd); log_fd = -1; }
    }

    void stop_pipeout(bool terminate_child = false) {
        if (pipeout_wr >= 0) { close(pipeout_wr); pipeout_wr = -1; }
        pipeout_buf.clear();
        if (!terminate_child || pipeout_pid <= 0) {
            if (!terminate_child) pipeout_pid = -1;
            return;
        }

        pid_t pid = pipeout_pid;
        pipeout_pid = -1;
        kill(-pid, SIGHUP);
        kill(-pid, SIGTERM);

        int st = 0;
        for (int i = 0; i < 20; i++) {
            pid_t rc = waitpid(pid, &st, WNOHANG);
            if (rc == pid || (rc < 0 && errno == ECHILD)) return;
            usleep(10000);
        }

        kill(-pid, SIGKILL);
        (void)waitpid(pid, &st, 0);
    }

    void cleanup() {
        stop_log();
        stop_pipeout(true);
        if (master_fd >= 0) { close(master_fd); master_fd = -1; }
        alive = false;
    }
};

struct Session {
    int id = -1;
    int focus = -1;
    std::vector<Pane*> panes;
};

// ============================================================
// ClientConn  (server-side per-client state)
// ============================================================

struct ClientConn {
    int fd = -1;
    bool readonly = false;
    bool dead = false;
    int session_id = -1;
    HelloMode hello_mode = HELLO_ATTACH;
    int target_session = 0;
    uint16_t rows = 24, cols = 80;
    std::string rbuf, wbuf;

    void enqueue(const std::string& m) { wbuf.append(m); }
};

// ============================================================
// Server
// ============================================================

class Server {
public:
    void run(const std::string& spath, int ready_fd) {
        spath_ = spath;
        init_signals();
        init_socket();

        if (lfd_ < 0) {
            if (ready_fd >= 0) close(ready_fd);
            return;
        }

        if (ready_fd >= 0) {
            (void)!write(ready_fd, "R", 1);
            close(ready_fd);
        }

        create_session();
        loop();
        teardown();
    }

private:
    std::string spath_;
    int lfd_ = -1;
    bool using_abstract_socket_ = false;
    std::vector<Session*> sessions_;
    std::vector<Pane*> panes_;
    std::vector<ClientConn*> clients_;
    int next_session_id_ = 0;
    int next_pane_id_ = 0;
    bool running_ = true;

    // ---- signals ----
    void init_signals() {
        make_sigpipe();
        signal(SIGPIPE, SIG_IGN);
        signal(SIGINT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGHUP, SIG_IGN);
        struct sigaction sa{};
        sa.sa_handler = on_sigchld;
        sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
        sigaction(SIGCHLD, &sa, nullptr);
    }

    // ---- socket ----
    void init_socket() {
        lfd_ = socket(AF_UNIX, SOCK_STREAM, 0);
        if (lfd_ < 0) {
            perror("socket");
            return;
        }
        set_nonblock(lfd_);
        set_cloexec(lfd_);
        struct sockaddr_un addr{};
        socklen_t addr_len = 0;
        unlink(spath_.c_str());
        if (!fill_socket_addr(addr, addr_len, spath_, false) ||
            bind(lfd_, (sockaddr*)&addr, addr_len) < 0) {
            if (errno != EPERM ||
                !fill_socket_addr(addr, addr_len, get_socket_abstract_name(), true) ||
                bind(lfd_, (sockaddr*)&addr, addr_len) < 0) {
                perror("bind");
                close(lfd_);
                lfd_ = -1;
                return;
            }
            using_abstract_socket_ = true;
        }
        if (listen(lfd_, 32) < 0) {
            perror("listen");
            close(lfd_);
            lfd_ = -1;
        }
    }

    Session* find_session(int id) {
        for (auto* s : sessions_) if (s->id == id) return s;
        return nullptr;
    }

    Session* default_session() {
        for (auto* s : sessions_) {
            if (!s->panes.empty()) return s;
        }
        return nullptr;
    }

    Pane* find_pane(int id) {
        for (auto* p : panes_) if (p->id == id) return p;
        return nullptr;
    }

    Pane* find_pane(Session* s, int id) {
        if (!s) return nullptr;
        for (auto* p : s->panes) if (p->id == id) return p;
        return nullptr;
    }

    int alive_count(Session* s) {
        if (!s) return 0;
        int n = 0;
        for (auto* p : s->panes) if (p->alive) n++;
        return n;
    }

    std::vector<Pane*> alive_panes(Session* s) {
        std::vector<Pane*> alive;
        if (!s) return alive;
        for (auto* p : s->panes) {
            if (p->alive) alive.push_back(p);
        }
        return alive;
    }

    // ---- terminal size helpers ----
    void effective_size(Session* s, int& r, int& c) {
        if (!s) { r = 24; c = 80; return; }
        r = INT_MAX; c = INT_MAX;
        for (auto* cl : clients_) {
            if (!cl->dead && cl->session_id == s->id) {
                if (cl->rows > 0) r = std::min(r, (int)cl->rows);
                if (cl->cols > 0) c = std::min(c, (int)cl->cols);
            }
        }
        if (r == INT_MAX || r < 1) r = 24;
        if (c == INT_MAX || c < 1) c = 80;
    }

    void recalc_layout(Session* s) {
        int np = alive_count(s);
        if (np == 0) return;
        int tr, tc;
        effective_size(s, tr, tc);
        int seps = np - 1;
        int avail = std::max(np, tr - seps);
        int base = avail / np, extra = avail % np;
        int idx = 0;
        for (auto* p : s->panes) {
            if (!p->alive) continue;
            int pr = base + (idx < extra ? 1 : 0);
            p->pane_rows = pr;
            p->pane_cols = tc;
            if (p->master_fd >= 0) {
                struct winsize ws{};
                ws.ws_row = pr;
                ws.ws_col = tc;
                ioctl(p->master_fd, TIOCSWINSZ, &ws);
                pid_t fg = tcgetpgrp(p->master_fd);
                if (fg > 0) killpg(fg, SIGWINCH);
                else if (p->child_pid > 0) kill(p->child_pid, SIGWINCH);
            }
            idx++;
        }
    }

    void stop_pipeout(Pane* p, bool terminate_child) {
        if (!p) return;
        p->stop_pipeout(terminate_child);
    }

    void send_snapshot(ClientConn* c) {
        Session* s = find_session(c->session_id);
        if (!s) return;
        auto alive = alive_panes(s);
        for (auto* p : alive) {
            c->enqueue(msg_pane_event(EVT_PANE_CREATED, p->id, s->focus));
        }
        c->enqueue(msg_pane_event(EVT_FOCUS_CHANGED, s->focus, s->focus));
        for (auto* p : alive) {
            std::string snap = p->cap_dump();
            if (!snap.empty()) c->enqueue(msg_output(p->id, snap.data(), snap.size()));
        }
    }

    void broadcast_to_session(Session* s, const std::string& m) {
        if (!s) return;
        for (auto* c : clients_) {
            if (!c->dead && c->session_id == s->id) c->enqueue(m);
        }
    }

    // ---- pane lifecycle ----
    Session* create_session() {
        auto* s = new Session();
        s->id = next_session_id_++;
        sessions_.push_back(s);
        Pane* p = create_pane(s);
        if (!p) {
            sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), s), sessions_.end());
            delete s;
            return nullptr;
        }
        return s;
    }

    Pane* create_pane(Session* s) {
        auto* p = new Pane();
        p->id = next_pane_id_++;
        p->session_id = s->id;

        int tr, tc;
        effective_size(s, tr, tc);
        int np = alive_count(s) + 1;
        int seps = np - 1;
        int avail = std::max(np, tr - seps);
        int pr = avail / np;
        p->pane_rows = pr;
        p->pane_cols = tc;

        int mfd, sfd;
        struct winsize ws{};
        ws.ws_row = pr;
        ws.ws_col = tc;
        if (openpty(&mfd, &sfd, nullptr, nullptr, &ws) < 0) {
            delete p;
            return nullptr;
        }

        pid_t pid = fork();
        if (pid < 0) { close(mfd); close(sfd); delete p; return nullptr; }

        if (pid == 0) {
            // --- child ---
            close(mfd);
            close(lfd_);
            close(g_sigpipe_rd);
            close(g_sigpipe_wr);
            for (auto* op : panes_) {
                if (op->master_fd >= 0) close(op->master_fd);
                if (op->log_fd >= 0) close(op->log_fd);
                if (op->pipeout_wr >= 0) close(op->pipeout_wr);
            }
            for (auto* cl : clients_) if (cl->fd >= 0) close(cl->fd);

            setsid();
            ioctl(sfd, TIOCSCTTY, 0);
            dup2(sfd, 0);
            dup2(sfd, 1);
            dup2(sfd, 2);
            if (sfd > 2) close(sfd);
            tcsetpgrp(STDIN_FILENO, getpgrp());

            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGCHLD, SIG_DFL);
            signal(SIGPIPE, SIG_DFL);
            signal(SIGHUP, SIG_DFL);
            signal(SIGWINCH, SIG_DFL);

            setenv("TERM", "xterm-256color", 1);
            unsetenv("MINI_TMUX_INSIDE");

            const char* sh = getenv("SHELL");
            if (!sh || !*sh) sh = "/bin/bash";
            execlp(sh, sh, nullptr);
            _exit(127);
        }

        // --- parent ---
        close(sfd);
        p->master_fd = mfd;
        p->child_pid = pid;
        p->alive = true;
        set_nonblock(mfd);
        set_cloexec(mfd);

        panes_.push_back(p);
        s->panes.push_back(p);
        s->focus = p->id;
        recalc_layout(s);
        broadcast_to_session(s, msg_pane_event(EVT_PANE_CREATED, p->id, s->focus));
        broadcast_to_session(s, msg_pane_event(EVT_FOCUS_CHANGED, p->id, s->focus));
        return p;
    }

    Pane* next_alive(Session* s, int cur) {
        std::vector<Pane*> alive = alive_panes(s);
        if (alive.empty()) return nullptr;
        for (size_t i = 0; i < alive.size(); i++)
            if (alive[i]->id == cur) return alive[(i + 1) % alive.size()];
        return alive[0];
    }

    Pane* prev_alive(Session* s, int cur) {
        std::vector<Pane*> alive = alive_panes(s);
        if (alive.empty()) return nullptr;
        for (size_t i = 0; i < alive.size(); i++)
            if (alive[i]->id == cur) return alive[(i + alive.size() - 1) % alive.size()];
        return alive.back();
    }

    void destroy_session(Session* s) {
        if (!s) return;
        for (auto* c : clients_) {
            if (!c->dead && c->session_id == s->id) drop_client(c);
        }
        sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), s), sessions_.end());
        delete s;
        if (sessions_.empty()) {
            for (auto* c : clients_) if (!c->dead) c->enqueue(msg_build(MSG_SERVER_EXIT, nullptr, 0));
            running_ = false;
        }
    }

    void pane_died(Pane* p) {
        if (!p->alive) return;
        Session* s = find_session(p->session_id);
        p->cleanup();
        if (!s) return;
        int old_focus = s->focus;
        s->panes.erase(std::remove(s->panes.begin(), s->panes.end(), p), s->panes.end());
        if (s->focus == p->id) {
            auto* nx = next_alive(s, p->id);
            s->focus = nx ? nx->id : -1;
        }
        broadcast_to_session(s, msg_pane_event(EVT_PANE_DESTROYED, p->id, s->focus));
        if (s->focus != old_focus && s->focus >= 0) {
            broadcast_to_session(s, msg_pane_event(EVT_FOCUS_CHANGED, s->focus, s->focus));
        }
        if (s->panes.empty()) {
            destroy_session(s);
            return;
        }
        recalc_layout(s);
    }

    void kill_pane(Session* s, int id) {
        Pane* p = find_pane(s, id);
        if (!p || !p->alive) return;
        if (p->child_pid > 0) {
            kill(-p->child_pid, SIGHUP);
            kill(-p->child_pid, SIGKILL);
        }
        pane_died(p);
    }

    // ---- client management ----
    void accept_client() {
        while (true) {
            int cfd = accept(lfd_, nullptr, nullptr);
            if (cfd < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) return;
                return;
            }
            set_nonblock(cfd);
            set_cloexec(cfd);
            auto* c = new ClientConn();
            c->fd = cfd;
            clients_.push_back(c);
        }
    }

    void drop_client(ClientConn* c) {
        if (c->dead) return;
        int sid = c->session_id;
        c->dead = true;
        if (c->fd >= 0) { close(c->fd); c->fd = -1; }
        Session* s = find_session(sid);
        if (s) recalc_layout(s);
    }

    void gc_clients() {
        for (auto it = clients_.begin(); it != clients_.end();) {
            if ((*it)->dead) { delete *it; it = clients_.erase(it); }
            else ++it;
        }
    }

    // ---- handle data from client ----
    void on_client_read(ClientConn* c) {
        char buf[65536];
        ssize_t n = read(c->fd, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) return;
            drop_client(c);
            return;
        }
        if (n == 0) { drop_client(c); return; }
        c->rbuf.append(buf, n);
        while (c->rbuf.size() >= HDR_SZ) {
            uint32_t type, len;
            memcpy(&type, c->rbuf.data(), 4);
            memcpy(&len, c->rbuf.data() + 4, 4);
            if (c->rbuf.size() < HDR_SZ + len) break;
            std::string payload(c->rbuf.data() + HDR_SZ, len);
            c->rbuf.erase(0, HDR_SZ + len);
            dispatch(c, (MsgType)type, payload);
            if (c->dead) break;
        }
    }

    void dispatch(ClientConn* c, MsgType t, const std::string& pl) {
        switch (t) {
        case MSG_HELLO: {
            if (pl.size() >= 10) {
                c->readonly = pl[0] != 0;
                c->hello_mode = static_cast<HelloMode>(static_cast<uint8_t>(pl[1]));
                memcpy(&c->target_session, pl.data() + 2, 4);
                memcpy(&c->rows, pl.data() + 6, 2);
                memcpy(&c->cols, pl.data() + 8, 2);
            } else if (pl.size() >= 5) {
                c->readonly = pl[0] != 0;
                memcpy(&c->rows, pl.data() + 1, 2);
                memcpy(&c->cols, pl.data() + 3, 2);
                c->hello_mode = HELLO_ATTACH;
                c->target_session = 0;
            }
            if (c->session_id < 0) {
                Session* s = nullptr;
                if (c->hello_mode == HELLO_NEW_SESSION) s = create_session();
                else {
                    s = find_session(c->target_session);
                    if (!s) s = default_session();
                    if (!s) s = create_session();
                }
                if (!s) {
                    c->enqueue(msg_build(MSG_SERVER_EXIT, nullptr, 0));
                    drop_client(c);
                    break;
                }
                c->session_id = s->id;
                recalc_layout(s);
                send_snapshot(c);
            } else {
                Session* s = find_session(c->session_id);
                if (s) recalc_layout(s);
            }
            break;
        }
        case MSG_INPUT:
            if (!c->readonly) {
                Session* s = find_session(c->session_id);
                Pane* p = s ? find_pane(s, s->focus) : nullptr;
                if (p && p->alive && p->master_fd >= 0)
                    p->input_buf.append(pl);
            }
            break;
        case MSG_WINSIZE:
            if (pl.size() >= 4) {
                memcpy(&c->rows, pl.data(), 2);
                memcpy(&c->cols, pl.data() + 2, 2);
                Session* s = find_session(c->session_id);
                if (s) recalc_layout(s);
            }
            break;
        case MSG_COMMAND:
            if (!c->readonly) {
                Session* s = find_session(c->session_id);
                if (s) exec_cmd(c, s, pl);
            }
            break;
        case MSG_DETACH:
            drop_client(c);
            break;
        default: break;
        }
    }

    // ---- commands ----
    void exec_cmd(ClientConn* sender, Session* sess, const std::string& raw) {
        std::string line = raw;
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r' || line.back() == ' ')) line.pop_back();
        while (!line.empty() && line.front() == ' ') line.erase(0, 1);
        if (!line.empty() && line.front() == ':') {
            line.erase(0, 1);
            while (!line.empty() && line.front() == ' ') line.erase(0, 1);
        }

        auto sp = line.find(' ');
        std::string cmd = (sp == std::string::npos) ? line : line.substr(0, sp);
        std::string args = (sp == std::string::npos) ? "" : line.substr(sp + 1);
        while (!args.empty() && args.front() == ' ') args.erase(0, 1);

        if (cmd == "new") {
            Pane* p = create_pane(sess);
            if (p) sender->enqueue(msg_build(MSG_CMD_RESULT, std::string("ok")));
            else   sender->enqueue(msg_build(MSG_CMD_RESULT, std::string("error")));
        }
        else if (cmd == "kill") {
            kill_pane(sess, std::atoi(args.c_str()));
        }
        else if (cmd == "focus") {
            int id = std::atoi(args.c_str());
            Pane* p = find_pane(sess, id);
            if (p && p->alive) {
                sess->focus = id;
                broadcast_to_session(sess, msg_pane_event(EVT_FOCUS_CHANGED, id, sess->focus));
            }
        }
        else if (cmd == "next") {
            Pane* nx = next_alive(sess, sess->focus);
            if (nx && nx->id != sess->focus) {
                sess->focus = nx->id;
                broadcast_to_session(sess, msg_pane_event(EVT_FOCUS_CHANGED, sess->focus, sess->focus));
            }
        }
        else if (cmd == "prev") {
            Pane* pv = prev_alive(sess, sess->focus);
            if (pv && pv->id != sess->focus) {
                sess->focus = pv->id;
                broadcast_to_session(sess, msg_pane_event(EVT_FOCUS_CHANGED, sess->focus, sess->focus));
            }
        }
        else if (cmd == "log") {
            auto sp2 = args.find(' ');
            if (sp2 == std::string::npos) return;
            int id = std::atoi(args.substr(0, sp2).c_str());
            std::string path = args.substr(sp2 + 1);
            while (!path.empty() && path.front() == ' ') path.erase(0, 1);
            Pane* p = find_pane(sess, id);
            if (!p || !p->alive) return;
            p->stop_log();
            int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd >= 0) { set_cloexec(fd); p->log_fd = fd; }
        }
        else if (cmd == "log-stop") {
            Pane* p = find_pane(sess, std::atoi(args.c_str()));
            if (p) p->stop_log();
        }
        else if (cmd == "pipeout") {
            auto sp2 = args.find(' ');
            if (sp2 == std::string::npos) return;
            int id = std::atoi(args.substr(0, sp2).c_str());
            std::string sh_cmd = args.substr(sp2 + 1);
            while (!sh_cmd.empty() && sh_cmd.front() == ' ') sh_cmd.erase(0, 1);
            Pane* p = find_pane(sess, id);
            if (!p || !p->alive) return;
            stop_pipeout(p, true);
            int pp[2];
            if (pipe(pp) < 0) return;
            pid_t pid = fork();
            if (pid < 0) { close(pp[0]); close(pp[1]); return; }
            if (pid == 0) {
                close(pp[1]);
                setpgid(0, 0);
                dup2(pp[0], 0);
                close(pp[0]);
                close(lfd_);
                close(g_sigpipe_rd);
                close(g_sigpipe_wr);
                for (auto* op : panes_) {
                    if (op->master_fd >= 0) close(op->master_fd);
                    if (op->log_fd >= 0) close(op->log_fd);
                    if (op->pipeout_wr >= 0) close(op->pipeout_wr);
                }
                for (auto* cl : clients_) if (cl->fd >= 0) close(cl->fd);
                signal(SIGINT, SIG_DFL);
                signal(SIGPIPE, SIG_DFL);
                signal(SIGCHLD, SIG_DFL);
                execl("/bin/sh", "sh", "-c", sh_cmd.c_str(), nullptr);
                _exit(127);
            }
            close(pp[0]);
            set_nonblock(pp[1]);
            set_cloexec(pp[1]);
            p->pipeout_wr = pp[1];
            p->pipeout_pid = pid;
        }
        else if (cmd == "pipeout-stop") {
            Pane* p = find_pane(sess, std::atoi(args.c_str()));
            if (p) stop_pipeout(p, true);
        }
        else if (cmd == "capture") {
            auto sp2 = args.find(' ');
            if (sp2 == std::string::npos) return;
            int id = std::atoi(args.substr(0, sp2).c_str());
            std::string path = args.substr(sp2 + 1);
            while (!path.empty() && path.front() == ' ') path.erase(0, 1);
            Pane* p = find_pane(sess, id);
            if (!p) return;
            std::string content = p->cap_dump();
            int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0) { try_write(fd, content.data(), content.size()); close(fd); }
        }
    }

    void flush_pipeout(Pane* p) {
        while (p->pipeout_wr >= 0 && !p->pipeout_buf.empty()) {
            ssize_t w = write(p->pipeout_wr, p->pipeout_buf.data(), p->pipeout_buf.size());
            if (w < 0) {
                if (errno == EINTR) continue;
                if (errno == EAGAIN || errno == EWOULDBLOCK) return;
                stop_pipeout(p, true);
                return;
            }
            if (w == 0) return;
            p->pipeout_buf.erase(0, w);
        }
    }

    // ---- pane output ----
    void on_pane_read(Pane* p) {
        char buf[65536];
        ssize_t n = read(p->master_fd, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) return;
            pane_died(p);
            return;
        }
        if (n == 0) {
            pane_died(p);
            return;
        }
        p->screen.feed(buf, n);
        if (p->log_fd >= 0) try_write(p->log_fd, buf, n);
        if (p->pipeout_wr >= 0) {
            p->pipeout_buf.append(buf, n);
            flush_pipeout(p);
        }
        broadcast_to_session(find_session(p->session_id), msg_output(p->id, buf, n));
    }

    void flush_pane_input(Pane* p) {
        while (!p->input_buf.empty() && p->master_fd >= 0) {
            ssize_t w = write(p->master_fd, p->input_buf.data(), p->input_buf.size());
            if (w < 0) {
                if (errno == EINTR) continue;
                if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                pane_died(p);
                break;
            }
            p->input_buf.erase(0, w);
        }
    }

    // ---- reap zombies ----
    void reap() {
        int st;
        pid_t pid;
        while ((pid = waitpid(-1, &st, WNOHANG)) > 0) {
            for (auto* p : panes_) {
                if (p->child_pid == pid) {
                    p->child_pid = -1;
                    if (p->alive) pane_died(p);
                    goto next_pid;
                }
                if (p->pipeout_pid == pid) {
                    p->pipeout_pid = -1;
                    if (p->pipeout_wr >= 0) { close(p->pipeout_wr); p->pipeout_wr = -1; }
                    p->pipeout_buf.clear();
                    goto next_pid;
                }
            }
            next_pid:;
        }
    }

    // ---- event loop ----
    void loop() {
        while (running_) {
            std::vector<pollfd> fds;

            fds.push_back({g_sigpipe_rd, POLLIN, 0});  // [0]
            fds.push_back({lfd_, POLLIN, 0});           // [1]

            std::vector<ClientConn*> snap_c(clients_);
            for (auto* c : snap_c) {
                short ev = POLLIN;
                if (!c->wbuf.empty()) ev |= POLLOUT;
                fds.push_back({c->fd, ev, 0});
            }

            std::vector<Pane*> snap_p;
            for (auto* p : panes_) {
                if (!p->alive || p->master_fd < 0) continue;
                short ev = POLLIN;
                if (!p->input_buf.empty()) ev |= POLLOUT;
                fds.push_back({p->master_fd, ev, 0});
                snap_p.push_back(p);
            }

            int ret = poll(fds.data(), fds.size(), 50);
            if (ret < 0 && errno != EINTR) break;

            // signal pipe
            if (fds[0].revents & POLLIN) { drain_sigpipe(); reap(); }

            // listen socket
            if (fds[1].revents & POLLIN) accept_client();

            // pane master fds
            size_t pi = 2 + snap_c.size();
            for (size_t i = 0; i < snap_p.size(); i++) {
                auto& pfd = fds[pi + i];
                Pane* p = snap_p[i];
                if (!p->alive || p->master_fd < 0) continue;
                if (pfd.revents & POLLOUT) flush_pane_input(p);
                if (pfd.revents & (POLLIN | POLLHUP | POLLERR)) on_pane_read(p);
            }

            // client fds
            for (size_t i = 0; i < snap_c.size(); i++) {
                auto& pfd = fds[2 + i];
                ClientConn* c = snap_c[i];
                if (c->dead) continue;
                if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) { drop_client(c); continue; }
                if (pfd.revents & POLLOUT) {
                    if (!flush_buf(c->fd, c->wbuf)) { drop_client(c); continue; }
                }
                if (pfd.revents & POLLIN) on_client_read(c);
            }

            // flush remaining client writes
            for (auto* c : clients_) {
                if (!c->dead && !c->wbuf.empty())
                    if (!flush_buf(c->fd, c->wbuf)) drop_client(c);
            }
            for (auto* p : panes_) {
                if (p->alive && p->pipeout_wr >= 0 && !p->pipeout_buf.empty()) flush_pipeout(p);
            }

            gc_clients();

            if (g_sigchld) { g_sigchld = 0; reap(); }
        }
    }

    // ---- cleanup ----
    void teardown() {
        // Final flush of pending writes to clients
        for (auto* c : clients_) {
            if (c->fd >= 0 && !c->wbuf.empty()) {
                // Temporarily set blocking for final flush
                int flags = fcntl(c->fd, F_GETFL, 0);
                fcntl(c->fd, F_SETFL, flags & ~O_NONBLOCK);
                try_write(c->fd, c->wbuf.data(), c->wbuf.size());
                c->wbuf.clear();
            }
        }
        for (auto* p : panes_) {
            if (p->child_pid > 0) {
                kill(-p->child_pid, SIGKILL);
                waitpid(p->child_pid, nullptr, 0);
            }
            p->cleanup();
            delete p;
        }
        for (auto* s : sessions_) delete s;
        for (auto* c : clients_) { if (c->fd >= 0) close(c->fd); delete c; }
        if (lfd_ >= 0) close(lfd_);
        if (!using_abstract_socket_) unlink(spath_.c_str());
        if (g_sigpipe_rd >= 0) close(g_sigpipe_rd);
        if (g_sigpipe_wr >= 0) close(g_sigpipe_wr);
    }
};

// ============================================================
// Client
// ============================================================

static struct termios g_orig_tios;
static bool g_raw = false;

static void restore_term() {
    if (g_raw) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig_tios);
        g_raw = false;
        const char* rst = "\033[?25h\033[0m\r\n";
        (void)!write(STDOUT_FILENO, rst, strlen(rst));
    }
}

static void client_sigdie(int) { restore_term(); _exit(1); }

class Client {
public:
    void run(const std::string& spath, bool ro, HelloMode mode, int attach_session) {
        readonly_ = ro;
        start_mode_ = mode;
        attach_session_ = attach_session;
        connect_server(spath);
        setup_term();
        setup_signals();
        send_hello();
        loop();
        restore_term();
        close(sfd_);
    }

private:
    int sfd_ = -1;
    bool readonly_ = false;
    HelloMode start_mode_ = HELLO_ATTACH;
    int attach_session_ = 0;
    int focus_ = 0;
    bool running_ = true;

    enum IState { NORMAL, PREFIX, PREFIX_ESC, PREFIX_CSI, PREFIX_SS3, COMMAND } istate_ = NORMAL;
    std::string cmd_buf_;
    std::string rbuf_, wbuf_;

    int term_rows_ = 24, term_cols_ = 80;
    std::map<int, bool> known_panes_;
    int pane_count_ = 0;
    bool layout_mode_ = false;
    bool input_line_start_ = true;
    bool command_echo_ = false;
    bool direct_colon_mode_ = false;
    std::map<int, ScreenBuffer> pane_bufs_;
    std::vector<int> pane_order_;

    // ---- connection ----
    void connect_server(const std::string& spath) {
        sfd_ = socket(AF_UNIX, SOCK_STREAM, 0);
        set_cloexec(sfd_);
        if (!try_connect_socket(sfd_, spath, false) &&
            !try_connect_socket(sfd_, get_socket_abstract_name(), true)) {
            fprintf(stderr, "mini-tmux: cannot connect to %s: %s\n", spath.c_str(), strerror(errno));
            exit(1);
        }
        set_nonblock(sfd_);
    }

    // ---- terminal ----
    void setup_term() {
        tcgetattr(STDIN_FILENO, &g_orig_tios);
        g_raw = true;
        atexit(restore_term);
        struct termios raw = g_orig_tios;
        cfmakeraw(&raw);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        struct winsize ws{};
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0 && ws.ws_col > 0) {
            term_rows_ = ws.ws_row;
            term_cols_ = ws.ws_col;
        }
    }

    void setup_signals() {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGTERM, client_sigdie);
        signal(SIGSEGV, client_sigdie);
        make_sigpipe();
        struct sigaction sa{};
        sa.sa_handler = on_sigwinch;
        sa.sa_flags = SA_RESTART;
        sigaction(SIGWINCH, &sa, nullptr);
    }

    void send_hello() {
        enqueue(msg_hello(readonly_, start_mode_, attach_session_, term_rows_, term_cols_));
    }

    void enqueue(const std::string& m) { wbuf_.append(m); }

    static bool is_server_command(const std::string& line) {
        size_t start = 0;
        while (start < line.size() && line[start] == ' ') start++;
        size_t end = start;
        while (end < line.size() && line[end] != ' ') end++;
        std::string cmd = line.substr(start, end - start);
        return cmd == "new" ||
               cmd == "kill" ||
               cmd == "focus" ||
               cmd == "next" ||
               cmd == "prev" ||
               cmd == "log" ||
               cmd == "log-stop" ||
               cmd == "pipeout" ||
               cmd == "pipeout-stop" ||
               cmd == "capture";
    }

    void enter_command_mode(bool echo_prompt, bool direct_colon_mode) {
        istate_ = COMMAND;
        cmd_buf_.clear();
        command_echo_ = echo_prompt;
        direct_colon_mode_ = direct_colon_mode;
        if (command_echo_) {
            (void)!write(STDOUT_FILENO, "\r\n:", 3);
        }
    }

    // ---- input handling ----
    void on_stdin(const char* buf, ssize_t n) {
        std::string fwd;
        for (ssize_t i = 0; i < n; i++) {
            unsigned char c = buf[i];
            switch (istate_) {
            case NORMAL:
                if (c == 0x02) {
                    if (!fwd.empty()) { enqueue(msg_build(MSG_INPUT, fwd)); fwd.clear(); }
                    istate_ = PREFIX;
                } else if (!readonly_ && input_line_start_ && c == ':') {
                    if (!fwd.empty()) { enqueue(msg_build(MSG_INPUT, fwd)); fwd.clear(); }
                    enter_command_mode(false, true);
                } else {
                    fwd += (char)c;
                    if (c == '\r' || c == '\n') input_line_start_ = true;
                    else input_line_start_ = false;
                }
                break;
            case PREFIX:
                on_prefix_key((char)c);
                break;
            case PREFIX_ESC:
                on_prefix_escape((char)c);
                break;
            case PREFIX_CSI:
            case PREFIX_SS3:
                on_prefix_arrow((char)c);
                break;
            case COMMAND:
                on_cmd_key((char)c);
                break;
            }
        }
        if (!fwd.empty()) enqueue(msg_build(MSG_INPUT, fwd));
    }

    void on_prefix_key(char c) {
        istate_ = NORMAL;
        switch (c) {
        case ':':
            if (readonly_) break;
            enter_command_mode(true, false);
            break;
        case 'd':
            enqueue(msg_build(MSG_DETACH, nullptr, 0));
            running_ = false;
            break;
        case 'n':
            if (readonly_) break;
            enqueue(msg_build(MSG_COMMAND, std::string("next")));
            break;
        case 'p':
            if (readonly_) break;
            enqueue(msg_build(MSG_COMMAND, std::string("prev")));
            break;
        case 0x1b:
            istate_ = PREFIX_ESC;
            break;
        default: break;
        }
    }

    void on_prefix_escape(char c) {
        if (c == '[') istate_ = PREFIX_CSI;
        else if (c == 'O') istate_ = PREFIX_SS3;
        else istate_ = NORMAL;
    }

    void on_prefix_arrow(char c) {
        istate_ = NORMAL;
        if (readonly_) return;
        if (c == 'A' || c == 'D') {
            enqueue(msg_build(MSG_COMMAND, std::string("prev")));
        } else if (c == 'B' || c == 'C') {
            enqueue(msg_build(MSG_COMMAND, std::string("next")));
        }
    }

    void on_cmd_key(char c) {
        if (c == '\r' || c == '\n') {
            istate_ = NORMAL;
            if (command_echo_) (void)!write(STDOUT_FILENO, "\r\n", 2);
            if (!cmd_buf_.empty()) {
                if (!direct_colon_mode_ || is_server_command(cmd_buf_)) {
                    enqueue(msg_build(MSG_COMMAND, cmd_buf_));
                } else {
                    enqueue(msg_build(MSG_INPUT, std::string(":") + cmd_buf_ + "\r"));
                }
            } else if (direct_colon_mode_) {
                enqueue(msg_build(MSG_INPUT, std::string(":\r")));
            }
            cmd_buf_.clear();
            command_echo_ = false;
            direct_colon_mode_ = false;
            input_line_start_ = true;
        } else if (c == 0x1b) {
            istate_ = NORMAL;
            cmd_buf_.clear();
            if (command_echo_) (void)!write(STDOUT_FILENO, "\r\n", 2);
            command_echo_ = false;
            direct_colon_mode_ = false;
        } else if (c == 0x7f || c == 0x08) {
            if (!cmd_buf_.empty()) {
                cmd_buf_.pop_back();
                if (command_echo_) (void)!write(STDOUT_FILENO, "\b \b", 3);
            }
        } else if ((unsigned char)c >= 32) {
            cmd_buf_ += c;
            if (command_echo_) (void)!write(STDOUT_FILENO, &c, 1);
        }
    }

    // ---- server messages ----
    void on_server_data() {
        char buf[65536];
        ssize_t n = read(sfd_, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) return;
            running_ = false;
            return;
        }
        if (n == 0) { running_ = false; return; }
        rbuf_.append(buf, n);
        while (rbuf_.size() >= HDR_SZ) {
            uint32_t type, len;
            memcpy(&type, rbuf_.data(), 4);
            memcpy(&len, rbuf_.data() + 4, 4);
            if (rbuf_.size() < HDR_SZ + len) break;
            std::string payload(rbuf_.data() + HDR_SZ, len);
            rbuf_.erase(0, HDR_SZ + len);
            handle_msg((MsgType)type, payload);
        }
    }

    void update_layout_mode() {
        bool want_layout = (pane_count_ >= 2);
        if (want_layout == layout_mode_) return;
        layout_mode_ = want_layout;
        // Clear screen on mode transition
        (void)!write(STDOUT_FILENO, "\033[2J\033[H", 7);
        if (layout_mode_) {
            pane_order_.clear();
            for (auto& kv : known_panes_)
                if (kv.second) { pane_bufs_[kv.first]; pane_order_.push_back(kv.first); }
            std::sort(pane_order_.begin(), pane_order_.end());
        }
    }

    void render_single() {
        auto it = pane_bufs_.find(focus_);
        if (it == pane_bufs_.end()) return;

        const auto& pb = it->second;
        auto lines = pb.visible_lines();
        int max_lines = std::max(1, term_rows_);
        int start_line = std::max(0, (int)lines.size() - max_lines);

        std::string out = "\033[2J\033[H";
        for (int r = 0; r < max_lines; r++) {
            if (r > 0) out += "\r\n";
            int li = start_line + r;
            if (li < (int)lines.size()) {
                out.append(lines[li], 0, std::min((int)lines[li].size(), term_cols_));
            }
        }
        (void)!write(STDOUT_FILENO, out.data(), out.size());
    }

    std::string build_separator(int upper_pid, int lower_pid) const {
        auto label = [this](int pid) {
            std::string text = "pane " + std::to_string(pid);
            if (pid == focus_) text += " [*]";
            return text;
        };

        std::string sep = "--- " + label(upper_pid) + " --- | --- " + label(lower_pid) + " ---";
        if ((int)sep.size() > term_cols_) sep.resize(term_cols_);
        return sep;
    }

    void render_layout() {
        if (pane_order_.empty()) return;
        int np = (int)pane_order_.size();
        int seps = np - 1;
        int avail = std::max(np, term_rows_ - seps);
        int base = avail / np, extra = avail % np;

        std::string out;
        out.reserve(term_rows_ * (term_cols_ + 20));
        out += "\033[?25l"; // hide cursor

        int row = 1;
        for (int i = 0; i < np; i++) {
            int pid = pane_order_[i];
            int pr = base + (i < extra ? 1 : 0);

            // Separator (before each pane except the first)
            if (i > 0) {
                char seq[128];
                snprintf(seq, sizeof(seq), "\033[%d;1H\033[7m", row);
                out += seq;
                int prev_pid = pane_order_[i - 1];
                std::string sep = build_separator(prev_pid, pid);
                out += sep;
                int slen = (int)sep.size();
                for (int c = slen; c < term_cols_; c++) out += ' ';
                out += "\033[0m";
                row++;
            }

            // Pane content
            auto it = pane_bufs_.find(pid);
            if (it != pane_bufs_.end()) {
                auto lines = it->second.visible_lines();
                int start_line = std::max(0, (int)lines.size() - pr);
                for (int r = 0; r < pr; r++) {
                    char seq[32];
                    snprintf(seq, sizeof(seq), "\033[%d;1H\033[K", row + r);
                    out += seq;
                    int li = start_line + r;
                    if (li < (int)lines.size()) {
                        const std::string& line = lines[li];
                        out.append(line, 0, std::min((int)line.size(), term_cols_));
                    }
                }
            }
            row += pr;
        }
        out += "\033[?25h"; // show cursor
        (void)!write(STDOUT_FILENO, out.data(), out.size());
    }

    void handle_msg(MsgType t, const std::string& pl) {
        switch (t) {
        case MSG_OUTPUT: {
            if (pl.size() < 4) break;
            int32_t pid;
            memcpy(&pid, pl.data(), 4);
            const char* d = pl.data() + 4;
            size_t l = pl.size() - 4;

            // Always buffer for layout mode
            pane_bufs_[pid].feed(d, l);

            if (layout_mode_) {
                render_layout();
            } else {
                if (pid == focus_) {
                    (void)!write(STDOUT_FILENO, d, l);
                }
            }
            break;
        }
        case MSG_PANE_EVENT: {
            if (pl.size() < 9) break;
            uint8_t ev = pl[0];
            int32_t pid, fid;
            memcpy(&pid, pl.data() + 1, 4);
            memcpy(&fid, pl.data() + 5, 4);
            focus_ = fid;
            if (ev == EVT_PANE_CREATED) {
                bool was_known = known_panes_[pid];
                known_panes_[pid] = true;
                if (!was_known) pane_count_++;
                if (layout_mode_) {
                    pane_bufs_[pid];
                    if (std::find(pane_order_.begin(), pane_order_.end(), pid) == pane_order_.end()) {
                        pane_order_.push_back(pid);
                        std::sort(pane_order_.begin(), pane_order_.end());
                    }
                }
            } else if (ev == EVT_PANE_DESTROYED) {
                if (known_panes_[pid]) pane_count_ = std::max(0, pane_count_ - 1);
                known_panes_[pid] = false;
                pane_bufs_.erase(pid);
                pane_order_.erase(
                    std::remove(pane_order_.begin(), pane_order_.end(), pid),
                    pane_order_.end());
            }
            update_layout_mode();
            if (layout_mode_) render_layout();
            else render_single();
            break;
        }
        case MSG_SERVER_EXIT:
            running_ = false;
            break;
        default: break;
        }
    }

    // ---- event loop ----
    void loop() {
        while (running_) {
            pollfd fds[3];
            fds[0] = {g_sigpipe_rd, POLLIN, 0};
            fds[1] = {STDIN_FILENO, POLLIN, 0};
            short sev = POLLIN;
            if (!wbuf_.empty()) sev |= POLLOUT;
            fds[2] = {sfd_, sev, 0};

            int ret = poll(fds, 3, 50);
            if (ret < 0 && errno != EINTR) break;

            if (fds[0].revents & POLLIN) {
                drain_sigpipe();
            }
            // Check for terminal size changes (SIGWINCH or polling fallback)
            {
                struct winsize ws{};
                if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == 0 &&
                    ws.ws_row > 0 && ws.ws_col > 0 &&
                    (ws.ws_row != term_rows_ || ws.ws_col != term_cols_)) {
                    term_rows_ = ws.ws_row;
                    term_cols_ = ws.ws_col;
                    enqueue(msg_winsize(ws.ws_row, ws.ws_col));
                    if (layout_mode_) render_layout();
                    else render_single();
                }
                g_sigwinch = 0;
            }

            if (fds[1].revents & POLLIN) {
                char buf[4096];
                ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
                if (n > 0) on_stdin(buf, n);
            }

            if (fds[2].revents & (POLLERR | POLLHUP | POLLNVAL)) { running_ = false; break; }
            if (fds[2].revents & POLLOUT) {
                if (!flush_buf(sfd_, wbuf_)) { running_ = false; break; }
            }
            if (fds[2].revents & POLLIN) on_server_data();

            if (!wbuf_.empty()) flush_buf(sfd_, wbuf_);
        }
    }
};

// ============================================================
// Main
// ============================================================

int main(int argc, char* argv[]) {
    std::string spath = get_socket_path();
    bool do_attach = false, readonly = false, run_server = false;
    int ready_fd = -1;

    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "attach") do_attach = true;
        else if (a == "-r") readonly = true;
        else if (a == "--server") run_server = true;
        else if (a == "--ready-fd" && i + 1 < argc) ready_fd = std::atoi(argv[++i]);
    }

    if (run_server) {
        Server srv;
        srv.run(spath, ready_fd);
        return 0;
    }

    if (do_attach) {
        Client c;
        c.run(spath, readonly, HELLO_ATTACH, 0);
        return 0;
    }

    if (socket_available(spath)) {
        Client c;
        c.run(spath, readonly, HELLO_NEW_SESSION, -1);
        return 0;
    }

    // Create server + auto-attach client
    int sync_pipe[2];
    if (pipe(sync_pipe) < 0) { perror("pipe"); return 1; }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        // --- server process ---
        close(sync_pipe[0]);
        setsid();
        int dn = open("/dev/null", O_RDWR);
        if (dn >= 0) {
            dup2(dn, STDIN_FILENO);
            dup2(dn, STDOUT_FILENO);
            dup2(dn, STDERR_FILENO);
            if (dn > 2) close(dn);
        }
        char fd_buf[32];
        snprintf(fd_buf, sizeof(fd_buf), "%d", sync_pipe[1]);
        execl("/proc/self/exe", argv[0], "--server", "--ready-fd", fd_buf, nullptr);
        _exit(127);
    }

    // --- client process ---
    close(sync_pipe[1]);
    char rdy;
    (void)!read(sync_pipe[0], &rdy, 1);
    close(sync_pipe[0]);

    Client c;
    c.run(spath, readonly, HELLO_ATTACH, 0);
    return 0;
}
