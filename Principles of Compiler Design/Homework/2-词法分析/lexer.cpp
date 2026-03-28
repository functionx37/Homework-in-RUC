#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

const bool OUTPUT_TO_FILE = true;

map<string, string> keywords = {
    {"const",    "CONSTTK"},
    {"int",      "INTTK"},
    {"void",     "VOIDTK"},
    {"if",       "IFTK"},
    {"else",     "ELSETK"},
    {"while",    "WHILETK"},
    {"break",    "BREAKTK"},
    {"continue", "CONTINUETK"},
    {"return",   "RETURNTK"}
};

struct Token {
    string type;
    string value;
    int line;
};

vector<Token> tokens;

void tokenize(const string &src) {
    int len = src.length();
    int pos = 0;
    int line = 1;

    while (pos < len) {
        char ch = src[pos];

        if (ch == '\n') {
            line++;
            pos++;
            continue;
        }
        if (ch == '\r' || ch == ' ' || ch == '\t') {
            pos++;
            continue;
        }

        // single-line comment
        if (ch == '/' && pos + 1 < len && src[pos + 1] == '/') {
            pos += 2;
            while (pos < len && src[pos] != '\n') pos++;
            continue;
        }

        // multi-line comment
        if (ch == '/' && pos + 1 < len && src[pos + 1] == '*') {
            pos += 2;
            while (pos + 1 < len && !(src[pos] == '*' && src[pos + 1] == '/')) {
                if (src[pos] == '\n') line++;
                pos++;
            }
            pos += 2; // skip */
            continue;
        }

        // identifier or keyword
        if (isalpha(ch) || ch == '_') {
            string word;
            while (pos < len && (isalnum(src[pos]) || src[pos] == '_')) {
                word += src[pos];
                pos++;
            }
            if (keywords.count(word)) {
                tokens.push_back({keywords[word], word, line});
            } else {
                tokens.push_back({"IDENFR", word, line});
            }
            continue;
        }

        // integer constant (decimal, hex 0x/0X, octal 0)
        if (isdigit(ch)) {
            string num;
            if (ch == '0' && pos + 1 < len && (src[pos + 1] == 'x' || src[pos + 1] == 'X')) {
                num += src[pos];     // '0'
                num += src[pos + 1]; // 'x' or 'X'
                pos += 2;
                while (pos < len && isxdigit(src[pos])) {
                    num += src[pos];
                    pos++;
                }
            } else if (ch == '0' && pos + 1 < len && src[pos + 1] >= '0' && src[pos + 1] <= '7') {
                num += src[pos];
                pos++;
                while (pos < len && src[pos] >= '0' && src[pos] <= '7') {
                    num += src[pos];
                    pos++;
                }
            } else {
                while (pos < len && isdigit(src[pos])) {
                    num += src[pos];
                    pos++;
                }
            }
            tokens.push_back({"INTCON", num, line});
            continue;
        }

        // format string
        if (ch == '"') {
            string str;
            str += ch;
            pos++;
            while (pos < len && src[pos] != '"') {
                str += src[pos];
                pos++;
            }
            str += src[pos]; // closing "
            pos++;
            tokens.push_back({"STRCON", str, line});
            continue;
        }

        // two-character operators
        if (ch == '<' && pos + 1 < len && src[pos + 1] == '=') {
            tokens.push_back({"LEQ", "<=", line});
            pos += 2;
            continue;
        }
        if (ch == '>' && pos + 1 < len && src[pos + 1] == '=') {
            tokens.push_back({"GEQ", ">=", line});
            pos += 2;
            continue;
        }
        if (ch == '=' && pos + 1 < len && src[pos + 1] == '=') {
            tokens.push_back({"EQL", "==", line});
            pos += 2;
            continue;
        }
        if (ch == '&' && pos + 1 < len && src[pos + 1] == '&') {
            tokens.push_back({"AND", "&&", line});
            pos += 2;
            continue;
        }
        if (ch == '|' && pos + 1 < len && src[pos + 1] == '|') {
            tokens.push_back({"OR", "||", line});
            pos += 2;
            continue;
        }
        if (ch == '!' && pos + 1 < len && src[pos + 1] == '=') {
            tokens.push_back({"NEQ", "!=", line});
            pos += 2;
            continue;
        }

        // single-character operators and delimiters
        switch (ch) {
            case '+': tokens.push_back({"PLUS",    "+", line}); break;
            case '-': tokens.push_back({"MINU",    "-", line}); break;
            case '*': tokens.push_back({"MULT",    "*", line}); break;
            case '/': tokens.push_back({"DIV",     "/", line}); break;
            case '%': tokens.push_back({"MOD",     "%", line}); break;
            case '!': tokens.push_back({"NOT",     "!", line}); break;
            case '<': tokens.push_back({"LSS",     "<", line}); break;
            case '>': tokens.push_back({"GRE",     ">", line}); break;
            case '=': tokens.push_back({"ASSIGN",  "=", line}); break;
            case ';': tokens.push_back({"SEMICN",  ";", line}); break;
            case ',': tokens.push_back({"COMMA",   ",", line}); break;
            case '(': tokens.push_back({"LPARENT", "(", line}); break;
            case ')': tokens.push_back({"RPARENT", ")", line}); break;
            case '[': tokens.push_back({"LBRACK",  "[", line}); break;
            case ']': tokens.push_back({"RBRACK",  "]", line}); break;
            case '{': tokens.push_back({"LBRACE",  "{", line}); break;
            case '}': tokens.push_back({"RBRACE",  "}", line}); break;
            default: break;
        }
        pos++;
    }
}

int main() {
    ifstream fin("testfile.txt");
    if (!fin.is_open()) {
        cerr << "Cannot open testfile.txt" << endl;
        return 1;
    }

    stringstream ss;
    ss << fin.rdbuf();
    string source = ss.str();
    fin.close();

    tokenize(source);

    if (OUTPUT_TO_FILE) {
        ofstream fout("output.txt");
        for (const auto &tok : tokens) {
            fout << tok.type << " " << tok.value << endl;
        }
        fout.close();
    }

    return 0;
}
