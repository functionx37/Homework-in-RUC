#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum class TokenKind {
    End,
    Ident,
    IntConst,
    StringConst,
    KwConst,
    KwInt,
    KwVoid,
    KwIf,
    KwElse,
    KwWhile,
    KwBreak,
    KwContinue,
    KwReturn,
    KwGetInt,
    KwPrintf,
    Plus,
    Minus,
    Mul,
    Div,
    Mod,
    Not,
    And,
    Or,
    Lt,
    Gt,
    Le,
    Ge,
    Eq,
    Ne,
    Assign,
    Semicolon,
    Comma,
    LParen,
    RParen,
    LBracket,
    RBracket,
    LBrace,
    RBrace
};

struct Token {
    TokenKind kind;
    string text;
    int line;
};

class Lexer {
public:
    explicit Lexer(const string &source) : src(source), pos(0), line(1) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (true) {
            skipWhitespaceAndComments();
            if (pos >= src.size()) {
                tokens.push_back({TokenKind::End, "", line});
                break;
            }
            char ch = src[pos];
            if (isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
                tokens.push_back(readIdentifier());
                continue;
            }
            if (isdigit(static_cast<unsigned char>(ch))) {
                tokens.push_back(readNumber());
                continue;
            }
            if (ch == '"') {
                tokens.push_back(readString());
                continue;
            }
            tokens.push_back(readSymbol());
        }
        return tokens;
    }

private:
    string src;
    size_t pos;
    int line;

    void skipWhitespaceAndComments() {
        while (pos < src.size()) {
            if (src[pos] == '\n') {
                ++line;
                ++pos;
                continue;
            }
            if (src[pos] == ' ' || src[pos] == '\r' || src[pos] == '\t') {
                ++pos;
                continue;
            }
            if (src[pos] == '/' && pos + 1 < src.size() && src[pos + 1] == '/') {
                pos += 2;
                while (pos < src.size() && src[pos] != '\n') {
                    ++pos;
                }
                continue;
            }
            if (src[pos] == '/' && pos + 1 < src.size() && src[pos + 1] == '*') {
                pos += 2;
                while (pos + 1 < src.size() && !(src[pos] == '*' && src[pos + 1] == '/')) {
                    if (src[pos] == '\n') {
                        ++line;
                    }
                    ++pos;
                }
                if (pos + 1 >= src.size()) {
                    throw runtime_error("Unterminated block comment");
                }
                pos += 2;
                continue;
            }
            break;
        }
    }

    Token readIdentifier() {
        size_t start = pos;
        while (pos < src.size() &&
               (isalnum(static_cast<unsigned char>(src[pos])) || src[pos] == '_')) {
            ++pos;
        }
        string word = src.substr(start, pos - start);
        static const map<string, TokenKind> keywords = {
            {"const", TokenKind::KwConst},
            {"int", TokenKind::KwInt},
            {"void", TokenKind::KwVoid},
            {"if", TokenKind::KwIf},
            {"else", TokenKind::KwElse},
            {"while", TokenKind::KwWhile},
            {"break", TokenKind::KwBreak},
            {"continue", TokenKind::KwContinue},
            {"return", TokenKind::KwReturn},
            {"getint", TokenKind::KwGetInt},
            {"printf", TokenKind::KwPrintf},
        };
        map<string, TokenKind>::const_iterator it = keywords.find(word);
        if (it != keywords.end()) {
            return {it->second, word, line};
        }
        return {TokenKind::Ident, word, line};
    }

    static int hexValue(char ch) {
        if (ch >= '0' && ch <= '9') {
            return ch - '0';
        }
        if (ch >= 'a' && ch <= 'f') {
            return ch - 'a' + 10;
        }
        if (ch >= 'A' && ch <= 'F') {
            return ch - 'A' + 10;
        }
        return -1;
    }

    Token readNumber() {
        size_t start = pos;
        if (src[pos] == '0' && pos + 1 < src.size() && (src[pos + 1] == 'x' || src[pos + 1] == 'X')) {
            pos += 2;
            while (pos < src.size() && isxdigit(static_cast<unsigned char>(src[pos]))) {
                ++pos;
            }
        } else if (src[pos] == '0') {
            ++pos;
            while (pos < src.size() && src[pos] >= '0' && src[pos] <= '7') {
                ++pos;
            }
        } else {
            while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                ++pos;
            }
        }
        return {TokenKind::IntConst, src.substr(start, pos - start), line};
    }

    Token readString() {
        size_t start = pos;
        ++pos;
        while (pos < src.size() && src[pos] != '"') {
            if (src[pos] == '\\') {
                if (pos + 1 >= src.size()) {
                    throw runtime_error("Invalid escape sequence in string");
                }
                pos += 2;
                continue;
            }
            if (src[pos] == '\n') {
                throw runtime_error("String literal cannot contain newline");
            }
            ++pos;
        }
        if (pos >= src.size()) {
            throw runtime_error("Unterminated string literal");
        }
        ++pos;
        return {TokenKind::StringConst, src.substr(start, pos - start), line};
    }

    Token readSymbol() {
        char ch = src[pos];
        if (pos + 1 < src.size()) {
            string two = src.substr(pos, 2);
            if (two == "&&") {
                pos += 2;
                return {TokenKind::And, "&&", line};
            }
            if (two == "||") {
                pos += 2;
                return {TokenKind::Or, "||", line};
            }
            if (two == "<=") {
                pos += 2;
                return {TokenKind::Le, "<=", line};
            }
            if (two == ">=") {
                pos += 2;
                return {TokenKind::Ge, ">=", line};
            }
            if (two == "==") {
                pos += 2;
                return {TokenKind::Eq, "==", line};
            }
            if (two == "!=") {
                pos += 2;
                return {TokenKind::Ne, "!=", line};
            }
        }
        ++pos;
        switch (ch) {
            case '+': return {TokenKind::Plus, "+", line};
            case '-': return {TokenKind::Minus, "-", line};
            case '*': return {TokenKind::Mul, "*", line};
            case '/': return {TokenKind::Div, "/", line};
            case '%': return {TokenKind::Mod, "%", line};
            case '!': return {TokenKind::Not, "!", line};
            case '<': return {TokenKind::Lt, "<", line};
            case '>': return {TokenKind::Gt, ">", line};
            case '=': return {TokenKind::Assign, "=", line};
            case ';': return {TokenKind::Semicolon, ";", line};
            case ',': return {TokenKind::Comma, ",", line};
            case '(': return {TokenKind::LParen, "(", line};
            case ')': return {TokenKind::RParen, ")", line};
            case '[': return {TokenKind::LBracket, "[", line};
            case ']': return {TokenKind::RBracket, "]", line};
            case '{': return {TokenKind::LBrace, "{", line};
            case '}': return {TokenKind::RBrace, "}", line};
            default: {
                ostringstream oss;
                oss << "Unexpected character '" << ch << "' at line " << line;
                throw runtime_error(oss.str());
            }
        }
    }
};

struct Expr {
    virtual ~Expr() {}
};

struct LVal {
    string name;
    vector<shared_ptr<Expr> > indices;
};

struct NumberExpr : Expr {
    explicit NumberExpr(int v) : value(v) {}
    int value;
};

struct LValExpr : Expr {
    explicit LValExpr(const LVal &v) : lval(v) {}
    LVal lval;
};

struct UnaryExpr : Expr {
    UnaryExpr(TokenKind o, const shared_ptr<Expr> &rhs) : op(o), operand(rhs) {}
    TokenKind op;
    shared_ptr<Expr> operand;
};

struct BinaryExpr : Expr {
    BinaryExpr(TokenKind o, const shared_ptr<Expr> &a, const shared_ptr<Expr> &b)
        : op(o), lhs(a), rhs(b) {}
    TokenKind op;
    shared_ptr<Expr> lhs;
    shared_ptr<Expr> rhs;
};

struct CallExpr : Expr {
    string callee;
    vector<shared_ptr<Expr> > args;
};

struct InitVal {
    bool isExpr;
    shared_ptr<Expr> expr;
    vector<shared_ptr<InitVal> > list;
};

struct VarDef {
    string name;
    vector<shared_ptr<Expr> > dims;
    shared_ptr<InitVal> init;
};

struct Decl {
    bool isConst;
    vector<VarDef> defs;
};

struct Param {
    string name;
    bool isArray;
    vector<shared_ptr<Expr> > dims;
};

struct Stmt {
    virtual ~Stmt() {}
};

struct BlockItem {
    bool isDecl;
    Decl decl;
    shared_ptr<Stmt> stmt;
};

struct BlockStmt : Stmt {
    vector<BlockItem> items;
};

struct AssignStmt : Stmt {
    LVal lval;
    shared_ptr<Expr> expr;
};

struct ExprStmt : Stmt {
    shared_ptr<Expr> expr;
};

struct IfStmt : Stmt {
    shared_ptr<Expr> cond;
    shared_ptr<Stmt> thenStmt;
    shared_ptr<Stmt> elseStmt;
};

struct WhileStmt : Stmt {
    shared_ptr<Expr> cond;
    shared_ptr<Stmt> body;
};

struct BreakStmt : Stmt {};

struct ContinueStmt : Stmt {};

struct ReturnStmt : Stmt {
    shared_ptr<Expr> expr;
};

struct GetIntStmt : Stmt {
    LVal lval;
};

struct PrintfStmt : Stmt {
    string format;
    vector<shared_ptr<Expr> > args;
};

struct FuncDef {
    string name;
    bool isVoid;
    bool isMain;
    vector<Param> params;
    shared_ptr<BlockStmt> block;
};

struct CompUnit {
    vector<Decl> globals;
    vector<FuncDef> funcs;
};

static int parseIntLiteral(const string &text) {
    if (text.size() > 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        return static_cast<int>(strtol(text.c_str(), NULL, 16));
    }
    if (text.size() > 1 && text[0] == '0') {
        return static_cast<int>(strtol(text.c_str(), NULL, 8));
    }
    return static_cast<int>(strtol(text.c_str(), NULL, 10));
}

class Parser {
public:
    explicit Parser(const vector<Token> &tokens) : toks(tokens), pos(0) {}

    CompUnit parseCompUnit() {
        CompUnit unit;
        while (!check(TokenKind::End)) {
            if (check(TokenKind::KwConst)) {
                unit.globals.push_back(parseConstDecl());
                continue;
            }
            if (isMainFuncDef()) {
                unit.funcs.push_back(parseMainFuncDef());
                continue;
            }
            if (isFuncDef()) {
                unit.funcs.push_back(parseFuncDef());
                continue;
            }
            unit.globals.push_back(parseVarDecl());
        }
        return unit;
    }

private:
    const vector<Token> &toks;
    size_t pos;

    const Token &cur() const {
        return toks[pos];
    }

    const Token &peek(size_t offset) const {
        size_t idx = pos + offset;
        if (idx >= toks.size()) {
            return toks.back();
        }
        return toks[idx];
    }

    bool check(TokenKind kind) const {
        return cur().kind == kind;
    }

    bool match(TokenKind kind) {
        if (check(kind)) {
            ++pos;
            return true;
        }
        return false;
    }

    const Token &expect(TokenKind kind, const string &message) {
        if (!check(kind)) {
            ostringstream oss;
            oss << "Line " << cur().line << ": " << message;
            throw runtime_error(oss.str());
        }
        return toks[pos++];
    }

    bool isFuncDef() const {
        if (check(TokenKind::KwVoid)) {
            return true;
        }
        if (!check(TokenKind::KwInt)) {
            return false;
        }
        return peek(1).kind == TokenKind::Ident && peek(2).kind == TokenKind::LParen &&
               peek(1).text != "main";
    }

    bool isMainFuncDef() const {
        return check(TokenKind::KwInt) && peek(1).kind == TokenKind::Ident &&
               peek(1).text == "main" && peek(2).kind == TokenKind::LParen;
    }

    bool isDeclStart() const {
        return check(TokenKind::KwConst) || check(TokenKind::KwInt);
    }

    bool looksLikeAssignStmt() const {
        if (!check(TokenKind::Ident)) {
            return false;
        }
        size_t i = pos + 1;
        int depth = 0;
        while (i < toks.size()) {
            TokenKind kind = toks[i].kind;
            if (kind == TokenKind::LBracket) {
                ++depth;
            } else if (kind == TokenKind::RBracket) {
                --depth;
            } else if (depth == 0) {
                if (kind == TokenKind::Assign) {
                    return true;
                }
                if (kind == TokenKind::LParen || kind == TokenKind::Semicolon ||
                    kind == TokenKind::Comma) {
                    return false;
                }
            }
            ++i;
        }
        return false;
    }

    Decl parseConstDecl() {
        expect(TokenKind::KwConst, "expected 'const'");
        expect(TokenKind::KwInt, "expected 'int' after 'const'");
        Decl decl;
        decl.isConst = true;
        decl.defs.push_back(parseConstDef());
        while (match(TokenKind::Comma)) {
            decl.defs.push_back(parseConstDef());
        }
        expect(TokenKind::Semicolon, "expected ';' after const declaration");
        return decl;
    }

    Decl parseVarDecl() {
        expect(TokenKind::KwInt, "expected 'int'");
        Decl decl;
        decl.isConst = false;
        decl.defs.push_back(parseVarDef());
        while (match(TokenKind::Comma)) {
            decl.defs.push_back(parseVarDef());
        }
        expect(TokenKind::Semicolon, "expected ';' after variable declaration");
        return decl;
    }

    VarDef parseConstDef() {
        VarDef def;
        def.name = expect(TokenKind::Ident, "expected identifier").text;
        while (match(TokenKind::LBracket)) {
            def.dims.push_back(parseConstExp());
            expect(TokenKind::RBracket, "expected ']'");
        }
        expect(TokenKind::Assign, "expected '=' in const definition");
        def.init = parseConstInitVal();
        return def;
    }

    VarDef parseVarDef() {
        VarDef def;
        def.name = expect(TokenKind::Ident, "expected identifier").text;
        while (match(TokenKind::LBracket)) {
            def.dims.push_back(parseConstExp());
            expect(TokenKind::RBracket, "expected ']'");
        }
        if (match(TokenKind::Assign)) {
            def.init = parseInitVal();
        }
        return def;
    }

    shared_ptr<InitVal> parseConstInitVal() {
        if (!match(TokenKind::LBrace)) {
            shared_ptr<InitVal> val(new InitVal());
            val->isExpr = true;
            val->expr = parseConstExp();
            return val;
        }
        shared_ptr<InitVal> val(new InitVal());
        val->isExpr = false;
        if (!check(TokenKind::RBrace)) {
            val->list.push_back(parseConstInitVal());
            while (match(TokenKind::Comma)) {
                val->list.push_back(parseConstInitVal());
            }
        }
        expect(TokenKind::RBrace, "expected '}'");
        return val;
    }

    shared_ptr<InitVal> parseInitVal() {
        if (!match(TokenKind::LBrace)) {
            shared_ptr<InitVal> val(new InitVal());
            val->isExpr = true;
            val->expr = parseExp();
            return val;
        }
        shared_ptr<InitVal> val(new InitVal());
        val->isExpr = false;
        if (!check(TokenKind::RBrace)) {
            val->list.push_back(parseInitVal());
            while (match(TokenKind::Comma)) {
                val->list.push_back(parseInitVal());
            }
        }
        expect(TokenKind::RBrace, "expected '}'");
        return val;
    }

    FuncDef parseFuncDef() {
        FuncDef func;
        func.isVoid = match(TokenKind::KwVoid);
        if (!func.isVoid) {
            expect(TokenKind::KwInt, "expected function return type");
        }
        func.isMain = false;
        func.name = expect(TokenKind::Ident, "expected function name").text;
        expect(TokenKind::LParen, "expected '('");
        if (!check(TokenKind::RParen)) {
            func.params = parseFuncFParams();
        }
        expect(TokenKind::RParen, "expected ')'");
        func.block = parseBlock();
        return func;
    }

    FuncDef parseMainFuncDef() {
        FuncDef func;
        expect(TokenKind::KwInt, "expected 'int'");
        func.isVoid = false;
        func.isMain = true;
        func.name = expect(TokenKind::Ident, "expected 'main'").text;
        expect(TokenKind::LParen, "expected '('");
        expect(TokenKind::RParen, "expected ')'");
        func.block = parseBlock();
        return func;
    }

    vector<Param> parseFuncFParams() {
        vector<Param> params;
        params.push_back(parseFuncFParam());
        while (match(TokenKind::Comma)) {
            params.push_back(parseFuncFParam());
        }
        return params;
    }

    Param parseFuncFParam() {
        expect(TokenKind::KwInt, "expected 'int' in parameter");
        Param param;
        param.name = expect(TokenKind::Ident, "expected parameter name").text;
        param.isArray = false;
        if (match(TokenKind::LBracket)) {
            param.isArray = true;
            expect(TokenKind::RBracket, "expected ']'");
            while (match(TokenKind::LBracket)) {
                param.dims.push_back(parseConstExp());
                expect(TokenKind::RBracket, "expected ']'");
            }
        }
        return param;
    }

    shared_ptr<BlockStmt> parseBlock() {
        expect(TokenKind::LBrace, "expected '{'");
        shared_ptr<BlockStmt> block(new BlockStmt());
        while (!check(TokenKind::RBrace)) {
            BlockItem item;
            if (isDeclStart()) {
                item.isDecl = true;
                item.decl = check(TokenKind::KwConst) ? parseConstDecl() : parseVarDecl();
            } else {
                item.isDecl = false;
                item.stmt = parseStmt();
            }
            block->items.push_back(item);
        }
        expect(TokenKind::RBrace, "expected '}'");
        return block;
    }

    shared_ptr<Stmt> parseStmt() {
        if (match(TokenKind::LBrace)) {
            --pos;
            return parseBlock();
        }
        if (match(TokenKind::KwIf)) {
            shared_ptr<IfStmt> stmt(new IfStmt());
            expect(TokenKind::LParen, "expected '(' after if");
            stmt->cond = parseCond();
            expect(TokenKind::RParen, "expected ')'");
            stmt->thenStmt = parseStmt();
            if (match(TokenKind::KwElse)) {
                stmt->elseStmt = parseStmt();
            }
            return stmt;
        }
        if (match(TokenKind::KwWhile)) {
            shared_ptr<WhileStmt> stmt(new WhileStmt());
            expect(TokenKind::LParen, "expected '(' after while");
            stmt->cond = parseCond();
            expect(TokenKind::RParen, "expected ')'");
            stmt->body = parseStmt();
            return stmt;
        }
        if (match(TokenKind::KwBreak)) {
            expect(TokenKind::Semicolon, "expected ';' after break");
            return shared_ptr<Stmt>(new BreakStmt());
        }
        if (match(TokenKind::KwContinue)) {
            expect(TokenKind::Semicolon, "expected ';' after continue");
            return shared_ptr<Stmt>(new ContinueStmt());
        }
        if (match(TokenKind::KwReturn)) {
            shared_ptr<ReturnStmt> stmt(new ReturnStmt());
            if (!check(TokenKind::Semicolon)) {
                stmt->expr = parseExp();
            }
            expect(TokenKind::Semicolon, "expected ';' after return");
            return stmt;
        }
        if (match(TokenKind::KwPrintf)) {
            shared_ptr<PrintfStmt> stmt(new PrintfStmt());
            expect(TokenKind::LParen, "expected '(' after printf");
            stmt->format = expect(TokenKind::StringConst, "expected format string").text;
            while (match(TokenKind::Comma)) {
                stmt->args.push_back(parseExp());
            }
            expect(TokenKind::RParen, "expected ')'");
            expect(TokenKind::Semicolon, "expected ';' after printf");
            return stmt;
        }
        if (looksLikeAssignStmt()) {
            LVal lval = parseLVal();
            expect(TokenKind::Assign, "expected '='");
            if (match(TokenKind::KwGetInt)) {
                expect(TokenKind::LParen, "expected '(' after getint");
                expect(TokenKind::RParen, "expected ')'");
                expect(TokenKind::Semicolon, "expected ';' after getint");
                shared_ptr<GetIntStmt> stmt(new GetIntStmt());
                stmt->lval = lval;
                return stmt;
            }
            shared_ptr<AssignStmt> stmt(new AssignStmt());
            stmt->lval = lval;
            stmt->expr = parseExp();
            expect(TokenKind::Semicolon, "expected ';' after assignment");
            return stmt;
        }
        shared_ptr<ExprStmt> stmt(new ExprStmt());
        if (!check(TokenKind::Semicolon)) {
            stmt->expr = parseExp();
        }
        expect(TokenKind::Semicolon, "expected ';'");
        return stmt;
    }

    shared_ptr<Expr> parseExp() {
        return parseAddExp();
    }

    shared_ptr<Expr> parseCond() {
        return parseLOrExp();
    }

    shared_ptr<Expr> parseConstExp() {
        return parseAddExp();
    }

    shared_ptr<Expr> parseLOrExp() {
        shared_ptr<Expr> lhs = parseLAndExp();
        while (match(TokenKind::Or)) {
            lhs = shared_ptr<Expr>(new BinaryExpr(TokenKind::Or, lhs, parseLAndExp()));
        }
        return lhs;
    }

    shared_ptr<Expr> parseLAndExp() {
        shared_ptr<Expr> lhs = parseEqExp();
        while (match(TokenKind::And)) {
            lhs = shared_ptr<Expr>(new BinaryExpr(TokenKind::And, lhs, parseEqExp()));
        }
        return lhs;
    }

    shared_ptr<Expr> parseEqExp() {
        shared_ptr<Expr> lhs = parseRelExp();
        while (check(TokenKind::Eq) || check(TokenKind::Ne)) {
            TokenKind op = cur().kind;
            ++pos;
            lhs = shared_ptr<Expr>(new BinaryExpr(op, lhs, parseRelExp()));
        }
        return lhs;
    }

    shared_ptr<Expr> parseRelExp() {
        shared_ptr<Expr> lhs = parseAddExp();
        while (check(TokenKind::Lt) || check(TokenKind::Gt) ||
               check(TokenKind::Le) || check(TokenKind::Ge)) {
            TokenKind op = cur().kind;
            ++pos;
            lhs = shared_ptr<Expr>(new BinaryExpr(op, lhs, parseAddExp()));
        }
        return lhs;
    }

    shared_ptr<Expr> parseAddExp() {
        shared_ptr<Expr> lhs = parseMulExp();
        while (check(TokenKind::Plus) || check(TokenKind::Minus)) {
            TokenKind op = cur().kind;
            ++pos;
            lhs = shared_ptr<Expr>(new BinaryExpr(op, lhs, parseMulExp()));
        }
        return lhs;
    }

    shared_ptr<Expr> parseMulExp() {
        shared_ptr<Expr> lhs = parseUnaryExp();
        while (check(TokenKind::Mul) || check(TokenKind::Div) || check(TokenKind::Mod)) {
            TokenKind op = cur().kind;
            ++pos;
            lhs = shared_ptr<Expr>(new BinaryExpr(op, lhs, parseUnaryExp()));
        }
        return lhs;
    }

    shared_ptr<Expr> parseUnaryExp() {
        if (check(TokenKind::Plus) || check(TokenKind::Minus) || check(TokenKind::Not)) {
            TokenKind op = cur().kind;
            ++pos;
            return shared_ptr<Expr>(new UnaryExpr(op, parseUnaryExp()));
        }
        if ((check(TokenKind::Ident) || check(TokenKind::KwGetInt)) &&
            peek(1).kind == TokenKind::LParen) {
            shared_ptr<CallExpr> call(new CallExpr());
            if (check(TokenKind::Ident)) {
                call->callee = expect(TokenKind::Ident, "expected function name").text;
            } else {
                call->callee = expect(TokenKind::KwGetInt, "expected function name").text;
            }
            expect(TokenKind::LParen, "expected '('");
            if (!check(TokenKind::RParen)) {
                call->args.push_back(parseExp());
                while (match(TokenKind::Comma)) {
                    call->args.push_back(parseExp());
                }
            }
            expect(TokenKind::RParen, "expected ')'");
            return call;
        }
        return parsePrimaryExp();
    }

    shared_ptr<Expr> parsePrimaryExp() {
        if (match(TokenKind::LParen)) {
            shared_ptr<Expr> expr = parseExp();
            expect(TokenKind::RParen, "expected ')'");
            return expr;
        }
        if (check(TokenKind::Ident)) {
            return shared_ptr<Expr>(new LValExpr(parseLVal()));
        }
        int value = parseIntLiteral(expect(TokenKind::IntConst, "expected integer literal").text);
        return shared_ptr<Expr>(new NumberExpr(value));
    }

    LVal parseLVal() {
        LVal lval;
        lval.name = expect(TokenKind::Ident, "expected identifier").text;
        while (match(TokenKind::LBracket)) {
            lval.indices.push_back(parseExp());
            expect(TokenKind::RBracket, "expected ']'");
        }
        return lval;
    }
};

static string stripQuotes(const string &text) {
    if (text.size() >= 2 && text.front() == '"' && text.back() == '"') {
        return text.substr(1, text.size() - 2);
    }
    return text;
}

static string join(const vector<string> &parts, const string &sep) {
    string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) {
            out += sep;
        }
        out += parts[i];
    }
    return out;
}

static int product(const vector<int> &dims, size_t start = 0) {
    int result = 1;
    for (size_t i = start; i < dims.size(); ++i) {
        result *= dims[i];
    }
    return result;
}

struct ConstValue {
    bool isArray;
    int scalar;
    vector<int> dims;
    vector<int> values;
};

struct ParamSig {
    bool isArray;
    vector<int> dims;
};

struct FuncSig {
    bool isVoid;
    vector<ParamSig> params;
};

struct Symbol {
    bool isConst;
    bool isGlobal;
    bool isArrayParam;
    string addr;
    vector<int> dims;
    int totalRank;
    string paramPointeeType;
    bool hasConstScalar;
    int constScalar;
    vector<int> constArrayValues;
};

struct AddrInfo {
    string ptr;
    bool isScalar;
    string pointeeType;
};

class CodeGen {
public:
    explicit CodeGen(const CompUnit &program) : prog(program), tempId(0), labelId(0) {
        pushScope();
        collectBuiltinFunctionSignatures();
        primeGlobalConstScope();
        collectFunctionSignatures();
        collectPrintfStrings();
        collectBuiltinUsages();
        ensureRuntimeStrings();
    }

    string generate() {
        vector<string> lines;
        lines.push_back("declare i32 @printf(i8*, ...)");
        lines.push_back("declare i8* @memcpy(i8*, i8*, i64)");
        lines.push_back("declare i8* @memset(i8*, i32, i64)");
        emitRuntimeBuiltinDecls(lines);
        lines.push_back("");
        lines.push_back(internalHasOutputName() + " = internal global i32 0");
        lines.push_back(internalLastNewlineName() + " = internal global i32 0");
        lines.push_back("");
        emitStringGlobals(lines);
        emitGlobalDecls(lines);
        emitFunctions(lines);
        emitConstArrayTemplateGlobals(lines);
        emitMainWrapper(lines);
        return join(lines, "\n") + "\n";
    }

private:
    const CompUnit &prog;
    map<string, FuncSig> funcSigs;
    map<string, pair<string, string> > stringPool;
    map<string, string> constArrayTemplatePool;
    vector<string> constArrayTemplateDefs;
    map<string, bool> userFunctionNames;
    map<string, bool> usedRuntimeBuiltins;
    vector<map<string, Symbol> > symbolScopes;
    vector<map<string, ConstValue> > constScopes;
    vector<string> allocas;
    vector<string> code;
    vector<pair<string, string> > loopLabels;
    string currentReturnLabel;
    string currentRetSlot;
    bool currentFuncIsVoid;
    int tempId;
    int labelId;

    void collectBuiltinFunctionSignatures() {
        FuncSig getIntSig;
        getIntSig.isVoid = false;
        funcSigs["getint"] = getIntSig;

        FuncSig getChSig;
        getChSig.isVoid = false;
        funcSigs["getch"] = getChSig;

        FuncSig getArraySig;
        getArraySig.isVoid = false;
        ParamSig getArrayParam;
        getArrayParam.isArray = true;
        getArraySig.params.push_back(getArrayParam);
        funcSigs["getarray"] = getArraySig;

        FuncSig putIntSig;
        putIntSig.isVoid = true;
        ParamSig putIntParam;
        putIntParam.isArray = false;
        putIntSig.params.push_back(putIntParam);
        funcSigs["putint"] = putIntSig;

        FuncSig putChSig;
        putChSig.isVoid = true;
        ParamSig putChParam;
        putChParam.isArray = false;
        putChSig.params.push_back(putChParam);
        funcSigs["putch"] = putChSig;

        FuncSig putArraySig;
        putArraySig.isVoid = true;
        ParamSig putArrayParam0;
        putArrayParam0.isArray = false;
        putArraySig.params.push_back(putArrayParam0);
        ParamSig putArrayParam1;
        putArrayParam1.isArray = true;
        putArraySig.params.push_back(putArrayParam1);
        funcSigs["putarray"] = putArraySig;

        FuncSig startTimeSig;
        startTimeSig.isVoid = true;
        funcSigs["starttime"] = startTimeSig;

        FuncSig stopTimeSig;
        stopTimeSig.isVoid = true;
        funcSigs["stoptime"] = stopTimeSig;

        FuncSig rawStartTimeSig;
        rawStartTimeSig.isVoid = true;
        ParamSig rawStartTimeParam;
        rawStartTimeParam.isArray = false;
        rawStartTimeSig.params.push_back(rawStartTimeParam);
        funcSigs["_sysy_starttime"] = rawStartTimeSig;

        FuncSig rawStopTimeSig;
        rawStopTimeSig.isVoid = true;
        ParamSig rawStopTimeParam;
        rawStopTimeParam.isArray = false;
        rawStopTimeSig.params.push_back(rawStopTimeParam);
        funcSigs["_sysy_stoptime"] = rawStopTimeSig;
    }

    void primeGlobalConstScope() {
        for (size_t i = 0; i < prog.globals.size(); ++i) {
            if (!prog.globals[i].isConst) {
                continue;
            }
            for (size_t j = 0; j < prog.globals[i].defs.size(); ++j) {
                const VarDef &def = prog.globals[i].defs[j];
                vector<int> dims = evalDims(def.dims);
                ConstValue cv;
                cv.isArray = !dims.empty();
                if (dims.empty()) {
                    cv.scalar = evalConstExpr(def.init->expr);
                } else {
                    cv.dims = dims;
                    cv.values = buildConstArrayValues(dims, def.init);
                }
                addConstBinding(def.name, cv);
            }
        }
    }

    void collectFunctionSignatures() {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            userFunctionNames[prog.funcs[i].name] = true;
            FuncSig sig;
            sig.isVoid = prog.funcs[i].isVoid;
            for (size_t j = 0; j < prog.funcs[i].params.size(); ++j) {
                ParamSig param;
                param.isArray = prog.funcs[i].params[j].isArray;
                param.dims = evalDims(prog.funcs[i].params[j].dims);
                sig.params.push_back(param);
            }
            funcSigs[prog.funcs[i].name] = sig;
        }
    }

    void collectPrintfStrings() {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            collectPrintfStringsInBlock(prog.funcs[i].block);
        }
    }

    bool hasUserFunction(const string &name) const {
        return userFunctionNames.find(name) != userFunctionNames.end();
    }

    static bool isRuntimeBuiltinName(const string &name) {
        return name == "getint" || name == "getch" || name == "getarray" ||
               name == "putint" || name == "putch" || name == "putarray" ||
               name == "starttime" || name == "stoptime" ||
               name == "_sysy_starttime" || name == "_sysy_stoptime";
    }

    bool isRuntimeBuiltinCall(const string &name) const {
        return isRuntimeBuiltinName(name) && !hasUserFunction(name);
    }

    void markRuntimeBuiltinUsed(const string &name) {
        if (isRuntimeBuiltinCall(name)) {
            usedRuntimeBuiltins[name] = true;
        }
    }

    void collectBuiltinUsages() {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            collectBuiltinUsagesInBlock(prog.funcs[i].block);
        }
    }

    void collectBuiltinUsagesInBlock(const shared_ptr<BlockStmt> &block) {
        for (size_t i = 0; i < block->items.size(); ++i) {
            if (block->items[i].isDecl) {
                collectBuiltinUsagesInDecl(block->items[i].decl);
            } else {
                collectBuiltinUsagesInStmt(block->items[i].stmt);
            }
        }
    }

    void collectBuiltinUsagesInDecl(const Decl &decl) {
        for (size_t i = 0; i < decl.defs.size(); ++i) {
            if (decl.defs[i].init) {
                collectBuiltinUsagesInInit(decl.defs[i].init);
            }
        }
    }

    void collectBuiltinUsagesInInit(const shared_ptr<InitVal> &init) {
        if (!init) {
            return;
        }
        if (init->isExpr) {
            collectBuiltinUsagesInExpr(init->expr);
            return;
        }
        for (size_t i = 0; i < init->list.size(); ++i) {
            collectBuiltinUsagesInInit(init->list[i]);
        }
    }

    void collectBuiltinUsagesInExpr(const shared_ptr<Expr> &expr) {
        if (!expr) {
            return;
        }
        if (dynamic_cast<LValExpr *>(expr.get()) != NULL) {
            const LVal &lval = static_pointer_cast<LValExpr>(expr)->lval;
            for (size_t i = 0; i < lval.indices.size(); ++i) {
                collectBuiltinUsagesInExpr(lval.indices[i]);
            }
            return;
        }
        if (dynamic_cast<UnaryExpr *>(expr.get()) != NULL) {
            collectBuiltinUsagesInExpr(static_pointer_cast<UnaryExpr>(expr)->operand);
            return;
        }
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            collectBuiltinUsagesInExpr(b->lhs);
            collectBuiltinUsagesInExpr(b->rhs);
            return;
        }
        if (dynamic_cast<CallExpr *>(expr.get()) != NULL) {
            shared_ptr<CallExpr> c = static_pointer_cast<CallExpr>(expr);
            markRuntimeBuiltinUsed(c->callee);
            for (size_t i = 0; i < c->args.size(); ++i) {
                collectBuiltinUsagesInExpr(c->args[i]);
            }
        }
    }

    void collectBuiltinUsagesInStmt(const shared_ptr<Stmt> &stmt) {
        if (!stmt) {
            return;
        }
        if (dynamic_cast<BlockStmt *>(stmt.get()) != NULL) {
            collectBuiltinUsagesInBlock(static_pointer_cast<BlockStmt>(stmt));
            return;
        }
        if (dynamic_cast<AssignStmt *>(stmt.get()) != NULL) {
            shared_ptr<AssignStmt> s = static_pointer_cast<AssignStmt>(stmt);
            for (size_t i = 0; i < s->lval.indices.size(); ++i) {
                collectBuiltinUsagesInExpr(s->lval.indices[i]);
            }
            collectBuiltinUsagesInExpr(s->expr);
            return;
        }
        if (dynamic_cast<ExprStmt *>(stmt.get()) != NULL) {
            collectBuiltinUsagesInExpr(static_pointer_cast<ExprStmt>(stmt)->expr);
            return;
        }
        if (dynamic_cast<IfStmt *>(stmt.get()) != NULL) {
            shared_ptr<IfStmt> s = static_pointer_cast<IfStmt>(stmt);
            collectBuiltinUsagesInExpr(s->cond);
            collectBuiltinUsagesInStmt(s->thenStmt);
            collectBuiltinUsagesInStmt(s->elseStmt);
            return;
        }
        if (dynamic_cast<WhileStmt *>(stmt.get()) != NULL) {
            shared_ptr<WhileStmt> s = static_pointer_cast<WhileStmt>(stmt);
            collectBuiltinUsagesInExpr(s->cond);
            collectBuiltinUsagesInStmt(s->body);
            return;
        }
        if (dynamic_cast<ReturnStmt *>(stmt.get()) != NULL) {
            collectBuiltinUsagesInExpr(static_pointer_cast<ReturnStmt>(stmt)->expr);
            return;
        }
        if (dynamic_cast<GetIntStmt *>(stmt.get()) != NULL) {
            markRuntimeBuiltinUsed("getint");
            shared_ptr<GetIntStmt> s = static_pointer_cast<GetIntStmt>(stmt);
            for (size_t i = 0; i < s->lval.indices.size(); ++i) {
                collectBuiltinUsagesInExpr(s->lval.indices[i]);
            }
            return;
        }
        if (dynamic_cast<PrintfStmt *>(stmt.get()) != NULL) {
            shared_ptr<PrintfStmt> s = static_pointer_cast<PrintfStmt>(stmt);
            for (size_t i = 0; i < s->args.size(); ++i) {
                collectBuiltinUsagesInExpr(s->args[i]);
            }
        }
    }

    void emitRuntimeBuiltinDecls(vector<string> &lines) const {
        if (usedRuntimeBuiltins.find("getint") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare i32 @getint()");
        }
        if (usedRuntimeBuiltins.find("getch") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare i32 @getch()");
        }
        if (usedRuntimeBuiltins.find("getarray") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare i32 @getarray(i32*)");
        }
        if (usedRuntimeBuiltins.find("putint") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putint(i32)");
        }
        if (usedRuntimeBuiltins.find("putch") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putch(i32)");
        }
        if (usedRuntimeBuiltins.find("putarray") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putarray(i32, i32*)");
        }
        if (usedRuntimeBuiltins.find("starttime") != usedRuntimeBuiltins.end() ||
            usedRuntimeBuiltins.find("_sysy_starttime") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @_sysy_starttime(i32)");
        }
        if (usedRuntimeBuiltins.find("stoptime") != usedRuntimeBuiltins.end() ||
            usedRuntimeBuiltins.find("_sysy_stoptime") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @_sysy_stoptime(i32)");
        }
    }

    void collectPrintfStringsInStmt(const shared_ptr<Stmt> &stmt) {
        if (!stmt) {
            return;
        }
        if (dynamic_cast<BlockStmt *>(stmt.get()) != NULL) {
            collectPrintfStringsInBlock(static_pointer_cast<BlockStmt>(stmt));
            return;
        }
        if (dynamic_cast<IfStmt *>(stmt.get()) != NULL) {
            shared_ptr<IfStmt> ifs = static_pointer_cast<IfStmt>(stmt);
            collectPrintfStringsInStmt(ifs->thenStmt);
            collectPrintfStringsInStmt(ifs->elseStmt);
            return;
        }
        if (dynamic_cast<WhileStmt *>(stmt.get()) != NULL) {
            collectPrintfStringsInStmt(static_pointer_cast<WhileStmt>(stmt)->body);
            return;
        }
        if (dynamic_cast<PrintfStmt *>(stmt.get()) != NULL) {
            shared_ptr<PrintfStmt> p = static_pointer_cast<PrintfStmt>(stmt);
            getStringName(stripQuotes(p->format));
        }
    }

    void collectPrintfStringsInBlock(const shared_ptr<BlockStmt> &block) {
        for (size_t i = 0; i < block->items.size(); ++i) {
            if (!block->items[i].isDecl) {
                collectPrintfStringsInStmt(block->items[i].stmt);
            }
        }
    }

    void ensureRuntimeStrings() {
        getStringName("%d\\n");
        getStringName("\\n");
    }

    string newTemp() {
        ostringstream oss;
        oss << "%t" << tempId++;
        return oss.str();
    }

    string newLabel(const string &prefix) {
        ostringstream oss;
        oss << prefix << labelId++;
        return oss.str();
    }

    void emit(const string &line) {
        code.push_back("  " + line);
    }

    void emitLabel(const string &label) {
        code.push_back(label + ":");
    }

    void emitAlloca(const string &line) {
        allocas.push_back("  " + line);
    }

    void pushScope() {
        symbolScopes.push_back(map<string, Symbol>());
        constScopes.push_back(map<string, ConstValue>());
    }

    void popScope() {
        symbolScopes.pop_back();
        constScopes.pop_back();
    }

    Symbol *findSymbol(const string &name) {
        for (int i = static_cast<int>(symbolScopes.size()) - 1; i >= 0; --i) {
            map<string, Symbol>::iterator it = symbolScopes[i].find(name);
            if (it != symbolScopes[i].end()) {
                return &it->second;
            }
        }
        return NULL;
    }

    const ConstValue *findConst(const string &name) const {
        for (int i = static_cast<int>(constScopes.size()) - 1; i >= 0; --i) {
            map<string, ConstValue>::const_iterator it = constScopes[i].find(name);
            if (it != constScopes[i].end()) {
                return &it->second;
            }
        }
        return NULL;
    }

    static string intValue(int v) {
        ostringstream oss;
        oss << v;
        return oss.str();
    }

    static string arrayType(const vector<int> &dims, size_t idx = 0) {
        if (idx >= dims.size()) {
            return "i32";
        }
        ostringstream oss;
        oss << "[" << dims[idx] << " x " << arrayType(dims, idx + 1) << "]";
        return oss.str();
    }

    static string pointerType(const string &baseType) {
        return baseType + "*";
    }

    static string arrayParamPointeeType(const vector<int> &dims) {
        return dims.empty() ? "i32" : arrayType(dims);
    }

    static string nestedArrayOrScalarType(const vector<int> &dims, size_t start) {
        return start >= dims.size() ? "i32" : arrayType(dims, start);
    }

    static string internalUserMainName() {
        return "__sysy_user_main";
    }

    static string userFunctionIRName(const string &name) {
        return name == "main" ? internalUserMainName() : "__sysy_user_fn_" + name;
    }

    static string globalIRName(const string &name) {
        return "@__sysy_user_g_" + name;
    }

    static string internalHasOutputName() {
        return "@__sysy_internal_has_output";
    }

    static string internalLastNewlineName() {
        return "@__sysy_internal_last_is_newline";
    }

    string functionIRName(const string &name) const {
        if (name == "starttime" && !hasUserFunction(name)) {
            return "_sysy_starttime";
        }
        if (name == "stoptime" && !hasUserFunction(name)) {
            return "_sysy_stoptime";
        }
        if (hasUserFunction(name)) {
            return userFunctionIRName(name);
        }
        return name;
    }

    string getStringName(const string &raw) {
        map<string, pair<string, string> >::iterator it = stringPool.find(raw);
        if (it != stringPool.end()) {
            return it->second.first;
        }
        ostringstream oss;
        oss << "@.str." << stringPool.size();
        string name = oss.str();
        stringPool[raw] = make_pair(name, llvmStringLiteral(raw));
        return name;
    }

    static string llvmStringLiteral(const string &raw) {
        ostringstream oss;
        for (size_t i = 0; i < raw.size(); ++i) {
            unsigned char ch = static_cast<unsigned char>(raw[i]);
            if (ch == '\\') {
                if (i + 1 >= raw.size()) {
                    throw runtime_error("Invalid string escape");
                }
                if (raw[i + 1] != 'n') {
                    throw runtime_error("Only \\n is allowed in format strings");
                }
                oss << "\\0A";
                ++i;
                continue;
            }
            if (ch == '"') {
                oss << "\\22";
                continue;
            }
            if (ch == '\\') {
                oss << "\\5C";
                continue;
            }
            if (ch < 32 || ch > 126) {
                oss << "\\" << uppercase << hex << setw(2) << setfill('0')
                    << static_cast<int>(ch) << nouppercase << dec << setfill(' ');
                continue;
            }
            oss << static_cast<char>(ch);
        }
        oss << "\\00";
        return oss.str();
    }

    static int llvmStringLength(const string &raw) {
        int len = 1;
        for (size_t i = 0; i < raw.size(); ++i) {
            if (raw[i] == '\\' && i + 1 < raw.size() && raw[i + 1] == 'n') {
                ++len;
                ++i;
            } else {
                ++len;
            }
        }
        return len;
    }

    void emitStringGlobals(vector<string> &lines) {
        for (map<string, pair<string, string> >::const_iterator it = stringPool.begin();
             it != stringPool.end(); ++it) {
            int len = llvmStringLength(it->first);
            ostringstream oss;
            oss << it->second.first << " = private unnamed_addr constant ["
                << len << " x i8] c\"" << it->second.second << "\"";
            lines.push_back(oss.str());
        }
        if (!stringPool.empty()) {
            lines.push_back("");
        }
    }

    void emitConstArrayTemplateGlobals(vector<string> &lines) const {
        for (size_t i = 0; i < constArrayTemplateDefs.size(); ++i) {
            lines.push_back(constArrayTemplateDefs[i]);
        }
        if (!constArrayTemplateDefs.empty()) {
            lines.push_back("");
        }
    }

    string stringPtrExpr(const string &raw) {
        string name = getStringName(raw);
        int len = llvmStringLength(raw);
        ostringstream oss;
        oss << "getelementptr inbounds ([" << len << " x i8], [" << len << " x i8]* " << name
            << ", i32 0, i32 0)";
        return oss.str();
    }

    int evalConstExpr(const shared_ptr<Expr> &expr) const {
        if (dynamic_cast<NumberExpr *>(expr.get()) != NULL) {
            return static_pointer_cast<NumberExpr>(expr)->value;
        }
        if (dynamic_cast<UnaryExpr *>(expr.get()) != NULL) {
            shared_ptr<UnaryExpr> u = static_pointer_cast<UnaryExpr>(expr);
            int rhs = evalConstExpr(u->operand);
            if (u->op == TokenKind::Plus) {
                return rhs;
            }
            if (u->op == TokenKind::Minus) {
                return -rhs;
            }
            return rhs == 0 ? 1 : 0;
        }
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            int lhs = evalConstExpr(b->lhs);
            int rhs = evalConstExpr(b->rhs);
            switch (b->op) {
                case TokenKind::Plus: return lhs + rhs;
                case TokenKind::Minus: return lhs - rhs;
                case TokenKind::Mul: return lhs * rhs;
                case TokenKind::Div: return lhs / rhs;
                case TokenKind::Mod: return lhs % rhs;
                case TokenKind::Lt: return lhs < rhs ? 1 : 0;
                case TokenKind::Gt: return lhs > rhs ? 1 : 0;
                case TokenKind::Le: return lhs <= rhs ? 1 : 0;
                case TokenKind::Ge: return lhs >= rhs ? 1 : 0;
                case TokenKind::Eq: return lhs == rhs ? 1 : 0;
                case TokenKind::Ne: return lhs != rhs ? 1 : 0;
                case TokenKind::And: return (lhs != 0 && rhs != 0) ? 1 : 0;
                case TokenKind::Or: return (lhs != 0 || rhs != 0) ? 1 : 0;
                default: break;
            }
        }
        if (dynamic_cast<LValExpr *>(expr.get()) != NULL) {
            LVal lval = static_pointer_cast<LValExpr>(expr)->lval;
            const ConstValue *cv = findConst(lval.name);
            if (cv == NULL) {
                throw runtime_error("Const expression references non-const identifier: " + lval.name);
            }
            if (!cv->isArray) {
                return cv->scalar;
            }
            if (lval.indices.size() != cv->dims.size()) {
                throw runtime_error("Const array reference needs full indices: " + lval.name);
            }
            int index = 0;
            for (size_t i = 0; i < cv->dims.size(); ++i) {
                int sub = evalConstExpr(lval.indices[i]);
                index = index * cv->dims[i] + sub;
            }
            return cv->values[index];
        }
        throw runtime_error("Unsupported const expression");
    }

    vector<int> evalDims(const vector<shared_ptr<Expr> > &dimsExpr) const {
        vector<int> dims;
        for (size_t i = 0; i < dimsExpr.size(); ++i) {
            dims.push_back(evalConstExpr(dimsExpr[i]));
        }
        return dims;
    }

    size_t fillConstArray(const shared_ptr<InitVal> &init,
                          const vector<int> &dims,
                          size_t level,
                          vector<int> &out,
                          size_t start) const {
        if (level == dims.size()) {
            if (start >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            if (!init->isExpr) {
                if (init->list.empty()) {
                    return start;
                }
                return fillConstArray(init->list[0], dims, dims.size(), out, start);
            }
            out[start] = evalConstExpr(init->expr);
            return start + 1;
        }
        if (init->isExpr) {
            if (start >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            out[start] = evalConstExpr(init->expr);
            return start + 1;
        }
        size_t pos = start;
        size_t end = start + static_cast<size_t>(product(dims, level));
        for (size_t i = 0; i < init->list.size() && pos < end; ++i) {
            if (init->list[i]->isExpr) {
                pos = fillConstArray(init->list[i], dims, dims.size(), out, pos);
            } else {
                size_t targetLevel = level + 1;
                while (targetLevel < dims.size()) {
                    size_t targetSize = static_cast<size_t>(product(dims, targetLevel));
                    if ((pos - start) % targetSize == 0) {
                        break;
                    }
                    ++targetLevel;
                }
                size_t subStart = pos;
                pos = fillConstArray(init->list[i], dims, targetLevel, out, subStart);
                if (targetLevel < dims.size()) {
                    pos = subStart + static_cast<size_t>(product(dims, targetLevel));
                }
            }
        }
        return pos;
    }

    vector<int> buildConstArrayValues(const vector<int> &dims, const shared_ptr<InitVal> &init) const {
        vector<int> values(static_cast<size_t>(product(dims)), 0);
        fillConstArray(init, dims, 0, values, 0);
        return values;
    }

    bool tryBuildConstArrayValues(const vector<int> &dims,
                                  const shared_ptr<InitVal> &init,
                                  vector<int> &values) const {
        if (!init) {
            return false;
        }
        try {
            values = buildConstArrayValues(dims, init);
            return true;
        } catch (const exception &) {
            values.clear();
            return false;
        }
    }

    string constArrayLiteralRec(const vector<int> &dims,
                                const vector<int> &values,
                                size_t level,
                                size_t &offset,
                                bool includeType) const {
        if (level == dims.size()) {
            return "i32 " + intValue(values[offset++]);
        }
        vector<string> parts;
        for (int i = 0; i < dims[level]; ++i) {
            parts.push_back(constArrayLiteralRec(dims, values, level + 1, offset, true));
        }
        string body = "[" + join(parts, ", ") + "]";
        if (includeType) {
            return arrayType(dims, level) + " " + body;
        }
        return body;
    }

    string constArrayLiteral(const vector<int> &dims, const vector<int> &values) const {
        bool allZero = true;
        for (size_t i = 0; i < values.size(); ++i) {
            if (values[i] != 0) {
                allZero = false;
                break;
            }
        }
        if (allZero) {
            return "zeroinitializer";
        }
        size_t offset = 0;
        return constArrayLiteralRec(dims, values, 0, offset, false);
    }

    bool isAllZeroArray(const vector<int> &values) const {
        for (size_t i = 0; i < values.size(); ++i) {
            if (values[i] != 0) {
                return false;
            }
        }
        return true;
    }

    long long arrayByteSize(const vector<int> &dims) const {
        return static_cast<long long>(product(dims)) * 4LL;
    }

    static string int64Value(long long value) {
        ostringstream oss;
        oss << value;
        return oss.str();
    }

    string getConstArrayTemplateName(const vector<int> &dims, const vector<int> &values) {
        string arrTy = arrayType(dims);
        string literal = constArrayLiteral(dims, values);
        string key = arrTy + " " + literal;
        map<string, string>::const_iterator it = constArrayTemplatePool.find(key);
        if (it != constArrayTemplatePool.end()) {
            return it->second;
        }
        ostringstream oss;
        oss << "@.arr.const." << constArrayTemplatePool.size();
        string name = oss.str();
        constArrayTemplatePool[key] = name;
        constArrayTemplateDefs.push_back(name + " = private unnamed_addr constant " + key);
        return name;
    }

    string bitcastToI8Ptr(const string &ptr, const string &pointeeType) {
        string reg = newTemp();
        emit(reg + " = bitcast " + pointerType(pointeeType) + " " + ptr + " to i8*");
        return reg;
    }

    void emitZeroArrayFill(const string &basePtr, const vector<int> &dims) {
        string arrTy = arrayType(dims);
        string dst = bitcastToI8Ptr(basePtr, arrTy);
        emit("call i8* @memset(i8* " + dst + ", i32 0, i64 " + int64Value(arrayByteSize(dims)) + ")");
    }

    void emitConstArrayCopy(const string &basePtr,
                            const vector<int> &dims,
                            const vector<int> &values) {
        string arrTy = arrayType(dims);
        string srcBase = getConstArrayTemplateName(dims, values);
        string dst = bitcastToI8Ptr(basePtr, arrTy);
        string src = bitcastToI8Ptr(srcBase, arrTy);
        emit("call i8* @memcpy(i8* " + dst + ", i8* " + src + ", i64 " + int64Value(arrayByteSize(dims)) + ")");
    }

    void emitOutputStateUpdate(const string &isNewlineValue) {
        emit("store i32 1, i32* " + internalHasOutputName());
        emit("store i32 " + isNewlineValue + ", i32* " + internalLastNewlineName());
    }

    void emitBuiltinOutputTracking(const string &callee, const vector<string> &argValues) {
        if (!isRuntimeBuiltinCall(callee)) {
            return;
        }
        if (callee == "putint") {
            emitOutputStateUpdate("0");
            return;
        }
        if (callee == "putarray") {
            emitOutputStateUpdate("1");
            return;
        }
        if (callee == "putch") {
            string isNl = newTemp();
            emit(isNl + " = icmp eq i32 " + argValues[0] + ", 10");
            string asInt = newTemp();
            emit(asInt + " = zext i1 " + isNl + " to i32");
            emitOutputStateUpdate(asInt);
        }
    }

    void flattenRuntimeInit(const shared_ptr<InitVal> &init,
                            const vector<int> &dims,
                            size_t level,
                            vector<shared_ptr<Expr> > &out,
                            size_t &pos,
                            size_t start) const {
        if (level == dims.size()) {
            if (pos >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            if (!init->isExpr) {
                if (init->list.empty()) {
                    return;
                }
                flattenRuntimeInit(init->list[0], dims, dims.size(), out, pos, pos);
                return;
            }
            out[pos++] = init->expr;
            return;
        }
        if (init->isExpr) {
            if (pos >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            out[pos++] = init->expr;
            return;
        }
        size_t end = start + static_cast<size_t>(product(dims, level));
        for (size_t i = 0; i < init->list.size(); ++i) {
            if (pos >= end) {
                throw runtime_error("Too many initializer elements");
            }
            if (init->list[i]->isExpr) {
                flattenRuntimeInit(init->list[i], dims, dims.size(), out, pos, pos);
            } else {
                size_t targetLevel = level + 1;
                while (targetLevel < dims.size()) {
                    size_t targetSize = static_cast<size_t>(product(dims, targetLevel));
                    if ((pos - start) % targetSize == 0) {
                        break;
                    }
                    ++targetLevel;
                }
                size_t subStart = pos;
                flattenRuntimeInit(init->list[i], dims, targetLevel, out, pos, subStart);
                if (targetLevel < dims.size()) {
                    pos = subStart + static_cast<size_t>(product(dims, targetLevel));
                }
            }
        }
    }

    string emitScalarExpr(const shared_ptr<Expr> &expr) {
        if (dynamic_cast<NumberExpr *>(expr.get()) != NULL) {
            return intValue(static_pointer_cast<NumberExpr>(expr)->value);
        }
        if (dynamic_cast<LValExpr *>(expr.get()) != NULL) {
            AddrInfo addr = emitLValAddress(static_pointer_cast<LValExpr>(expr)->lval, true);
            if (!addr.isScalar) {
                throw runtime_error("Array value used as scalar expression");
            }
            string reg = newTemp();
            emit(reg + " = load i32, i32* " + addr.ptr);
            return reg;
        }
        if (dynamic_cast<UnaryExpr *>(expr.get()) != NULL) {
            shared_ptr<UnaryExpr> u = static_pointer_cast<UnaryExpr>(expr);
            string rhs = emitScalarExpr(u->operand);
            if (u->op == TokenKind::Plus) {
                return rhs;
            }
            if (u->op == TokenKind::Minus) {
                string reg = newTemp();
                emit(reg + " = sub i32 0, " + rhs);
                return reg;
            }
            string cond = emitIcmpNeZero(rhs);
            string asInt = newTemp();
            emit(asInt + " = zext i1 " + cond + " to i32");
            string reg = newTemp();
            emit(reg + " = xor i32 " + asInt + ", 1");
            return reg;
        }
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            if (b->op == TokenKind::And || b->op == TokenKind::Or) {
                return emitLogicalValue(b);
            }
            string lhs = emitScalarExpr(b->lhs);
            string rhs = emitScalarExpr(b->rhs);
            if (b->op == TokenKind::Plus || b->op == TokenKind::Minus || b->op == TokenKind::Mul ||
                b->op == TokenKind::Div || b->op == TokenKind::Mod) {
                string reg = newTemp();
                string op;
                if (b->op == TokenKind::Plus) op = "add";
                if (b->op == TokenKind::Minus) op = "sub";
                if (b->op == TokenKind::Mul) op = "mul";
                if (b->op == TokenKind::Div) op = "sdiv";
                if (b->op == TokenKind::Mod) op = "srem";
                emit(reg + " = " + op + " i32 " + lhs + ", " + rhs);
                return reg;
            }
            string pred;
            if (b->op == TokenKind::Lt) pred = "slt";
            if (b->op == TokenKind::Gt) pred = "sgt";
            if (b->op == TokenKind::Le) pred = "sle";
            if (b->op == TokenKind::Ge) pred = "sge";
            if (b->op == TokenKind::Eq) pred = "eq";
            if (b->op == TokenKind::Ne) pred = "ne";
            string cmp = newTemp();
            emit(cmp + " = icmp " + pred + " i32 " + lhs + ", " + rhs);
            string reg = newTemp();
            emit(reg + " = zext i1 " + cmp + " to i32");
            return reg;
        }
        if (dynamic_cast<CallExpr *>(expr.get()) != NULL) {
            shared_ptr<CallExpr> call = static_pointer_cast<CallExpr>(expr);
            map<string, FuncSig>::const_iterator fit = funcSigs.find(call->callee);
            if (fit == funcSigs.end()) {
                throw runtime_error("Unknown function: " + call->callee);
            }
            vector<string> args;
            vector<string> rawArgs;
            if (isRuntimeBuiltinCall(call->callee) &&
                (call->callee == "starttime" || call->callee == "stoptime")) {
                rawArgs.push_back("0");
                args.push_back("i32 0");
            }
            for (size_t i = 0; i < call->args.size(); ++i) {
                if (i < fit->second.params.size() && fit->second.params[i].isArray) {
                    string ptrArg = emitPointerArg(call->args[i], fit->second.params[i].dims);
                    string pointeeType = arrayParamPointeeType(fit->second.params[i].dims);
                    rawArgs.push_back(ptrArg);
                    args.push_back(pointerType(pointeeType) + " " + ptrArg);
                } else {
                    string scalarArg = emitScalarExpr(call->args[i]);
                    rawArgs.push_back(scalarArg);
                    args.push_back("i32 " + scalarArg);
                }
            }
            if (fit->second.isVoid) {
                emit("call void @" + functionIRName(call->callee) + "(" + join(args, ", ") + ")");
                emitBuiltinOutputTracking(call->callee, rawArgs);
                return "0";
            }
            string reg = newTemp();
            emit(reg + " = call i32 @" + functionIRName(call->callee) + "(" + join(args, ", ") + ")");
            return reg;
        }
        throw runtime_error("Unsupported expression kind");
    }

    string emitPointerArg(const shared_ptr<Expr> &expr, const vector<int> &expectedDims) {
        if (dynamic_cast<LValExpr *>(expr.get()) == NULL) {
            throw runtime_error("Array argument must be an lvalue");
        }
        AddrInfo addr = emitLValAddress(static_pointer_cast<LValExpr>(expr)->lval, true);
        if (addr.isScalar) {
            throw runtime_error("Scalar used where array parameter is required");
        }
        string expectedPointeeType = arrayParamPointeeType(expectedDims);
        if (addr.pointeeType == expectedPointeeType) {
            return addr.ptr;
        }
        string casted = newTemp();
        emit(casted + " = bitcast " + pointerType(addr.pointeeType) + " " + addr.ptr +
             " to " + pointerType(expectedPointeeType));
        return casted;
    }

    string emitIcmpNeZero(const string &value) {
        string reg = newTemp();
        emit(reg + " = icmp ne i32 " + value + ", 0");
        return reg;
    }

    string emitLogicalValue(const shared_ptr<BinaryExpr> &expr) {
        string resultPtr = newTemp();
        emitAlloca(resultPtr + " = alloca i32");
        emit("store i32 0, i32* " + resultPtr);
        string trueLabel = newLabel("logic.true.");
        string falseLabel = newLabel("logic.false.");
        string endLabel = newLabel("logic.end.");
        emitCond(expr, trueLabel, falseLabel);
        emitLabel(trueLabel);
        emit("store i32 1, i32* " + resultPtr);
        emit("br label %" + endLabel);
        emitLabel(falseLabel);
        emit("store i32 0, i32* " + resultPtr);
        emit("br label %" + endLabel);
        emitLabel(endLabel);
        string reg = newTemp();
        emit(reg + " = load i32, i32* " + resultPtr);
        return reg;
    }

    void emitCond(const shared_ptr<Expr> &expr, const string &trueLabel, const string &falseLabel) {
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            if (b->op == TokenKind::And) {
                string mid = newLabel("land.rhs.");
                emitCond(b->lhs, mid, falseLabel);
                emitLabel(mid);
                emitCond(b->rhs, trueLabel, falseLabel);
                return;
            }
            if (b->op == TokenKind::Or) {
                string mid = newLabel("lor.rhs.");
                emitCond(b->lhs, trueLabel, mid);
                emitLabel(mid);
                emitCond(b->rhs, trueLabel, falseLabel);
                return;
            }
        }
        string value = emitScalarExpr(expr);
        string cond = emitIcmpNeZero(value);
        emit("br i1 " + cond + ", label %" + trueLabel + ", label %" + falseLabel);
    }

    AddrInfo emitLValAddress(const LVal &lval, bool allowDecay) {
        Symbol *sym = findSymbol(lval.name);
        if (sym == NULL) {
            throw runtime_error("Unknown identifier: " + lval.name);
        }
        if (!sym->isArrayParam && sym->dims.empty()) {
            if (!lval.indices.empty()) {
                throw runtime_error("Scalar variable indexed like array: " + lval.name);
            }
            AddrInfo info;
            info.ptr = sym->addr;
            info.isScalar = true;
            info.pointeeType = "i32";
            return info;
        }
        if (sym->isArrayParam) {
            int rank = sym->totalRank;
            if (static_cast<int>(lval.indices.size()) > rank) {
                throw runtime_error("Too many indices on array parameter: " + lval.name);
            }
            if (lval.indices.empty()) {
                if (!allowDecay) {
                    throw runtime_error("Array used without full indices: " + lval.name);
                }
                AddrInfo info;
                info.ptr = sym->addr;
                info.isScalar = false;
                info.pointeeType = sym->paramPointeeType;
                return info;
            }
            vector<string> idxRegs;
            for (size_t i = 0; i < lval.indices.size(); ++i) {
                idxRegs.push_back(emitScalarExpr(lval.indices[i]));
            }
            string reg = newTemp();
            string inst = reg + " = getelementptr inbounds " + sym->paramPointeeType + ", " +
                          pointerType(sym->paramPointeeType) + " " + sym->addr;
            for (size_t i = 0; i < idxRegs.size(); ++i) {
                inst += ", i32 " + idxRegs[i];
            }
            emit(inst);
            AddrInfo info;
            info.ptr = reg;
            info.isScalar = (static_cast<int>(lval.indices.size()) == rank);
            size_t typeStart = lval.indices.empty() ? 0 : lval.indices.size() - 1;
            info.pointeeType = info.isScalar ? "i32" : nestedArrayOrScalarType(sym->dims, typeStart);
            if (!info.isScalar && !allowDecay) {
                throw runtime_error("Array used without full indices: " + lval.name);
            }
            return info;
        }
        if (static_cast<int>(lval.indices.size()) > static_cast<int>(sym->dims.size())) {
            throw runtime_error("Too many indices on array: " + lval.name);
        }
        vector<string> idxRegs;
        for (size_t i = 0; i < lval.indices.size(); ++i) {
            idxRegs.push_back(emitScalarExpr(lval.indices[i]));
        }
        string reg = newTemp();
        string arrTy = arrayType(sym->dims);
        string inst = reg + " = getelementptr inbounds " + arrTy + ", " +
                      pointerType(arrTy) + " " + sym->addr + ", i32 0";
        if (idxRegs.empty()) {
            if (!allowDecay) {
                throw runtime_error("Array used without full indices: " + lval.name);
            }
            inst += ", i32 0";
            emit(inst);
            AddrInfo info;
            info.ptr = reg;
            info.isScalar = false;
            info.pointeeType = nestedArrayOrScalarType(sym->dims, 1);
            return info;
        }
        for (size_t i = 0; i < idxRegs.size(); ++i) {
            inst += ", i32 " + idxRegs[i];
        }
        emit(inst);
        AddrInfo info;
        info.ptr = reg;
        info.isScalar = (lval.indices.size() == sym->dims.size());
        info.pointeeType = info.isScalar ? "i32"
                                         : nestedArrayOrScalarType(sym->dims, lval.indices.size());
        if (!info.isScalar && !allowDecay) {
            throw runtime_error("Array used without full indices: " + lval.name);
        }
        return info;
    }

    void emitStoreToLVal(const LVal &lval, const string &value) {
        AddrInfo addr = emitLValAddress(lval, false);
        emit("store i32 " + value + ", i32* " + addr.ptr);
    }

    void addConstBinding(const string &name, const ConstValue &value) {
        constScopes.back()[name] = value;
    }

    void addSymbol(const string &name, const Symbol &sym) {
        symbolScopes.back()[name] = sym;
    }

    void emitGlobalDecls(vector<string> &lines) {
        pushScope();
        for (size_t i = 0; i < prog.globals.size(); ++i) {
            emitOneGlobalDecl(lines, prog.globals[i]);
        }
        if (!prog.globals.empty()) {
            lines.push_back("");
        }
    }

    void emitOneGlobalDecl(vector<string> &lines, const Decl &decl) {
        for (size_t i = 0; i < decl.defs.size(); ++i) {
            vector<int> dims = evalDims(decl.defs[i].dims);
            string name = globalIRName(decl.defs[i].name);
            Symbol sym;
            sym.isConst = decl.isConst;
            sym.isGlobal = true;
            sym.isArrayParam = false;
            sym.addr = name;
            sym.dims = dims;
            sym.totalRank = static_cast<int>(dims.size());
            sym.paramPointeeType = "";
            sym.hasConstScalar = false;
            if (dims.empty()) {
                int initVal = 0;
                if (decl.defs[i].init) {
                    initVal = evalConstExpr(decl.defs[i].init->expr);
                }
                ostringstream oss;
                oss << name << " = " << (decl.isConst ? "constant" : "global")
                    << " i32 " << initVal;
                lines.push_back(oss.str());
                if (decl.isConst) {
                    sym.hasConstScalar = true;
                    sym.constScalar = initVal;
                    ConstValue cv;
                    cv.isArray = false;
                    cv.scalar = initVal;
                    addConstBinding(decl.defs[i].name, cv);
                }
            } else {
                vector<int> values(static_cast<size_t>(product(dims)), 0);
                if (decl.defs[i].init) {
                    values = buildConstArrayValues(dims, decl.defs[i].init);
                }
                ostringstream oss;
                oss << name << " = " << (decl.isConst ? "constant" : "global") << " "
                    << arrayType(dims) << " " << constArrayLiteral(dims, values);
                lines.push_back(oss.str());
                if (decl.isConst) {
                    sym.constArrayValues = values;
                    ConstValue cv;
                    cv.isArray = true;
                    cv.dims = dims;
                    cv.values = values;
                    addConstBinding(decl.defs[i].name, cv);
                }
            }
            addSymbol(decl.defs[i].name, sym);
        }
    }

    void emitFunctions(vector<string> &lines) {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            emitOneFunction(lines, prog.funcs[i]);
            lines.push_back("");
        }
    }

    void emitMainWrapper(vector<string> &lines) {
        lines.push_back("define i32 @main() {");
        lines.push_back("entry:");
        lines.push_back("  %t.main.ret = call i32 @" + internalUserMainName() + "()");
        lines.push_back("  %t.main.has_output = load i32, i32* " + internalHasOutputName());
        lines.push_back("  %t.main.has_output.cond = icmp ne i32 %t.main.has_output, 0");
        lines.push_back("  br i1 %t.main.has_output.cond, label %main.need_check_nl, label %main.print_exit");
        lines.push_back("main.need_check_nl:");
        lines.push_back("  %t.main.last_nl = load i32, i32* " + internalLastNewlineName());
        lines.push_back("  %t.main.last_nl.cond = icmp ne i32 %t.main.last_nl, 0");
        lines.push_back("  br i1 %t.main.last_nl.cond, label %main.print_exit, label %main.insert_nl");
        lines.push_back("main.insert_nl:");
        lines.push_back("  %t.main.nl = call i32 (i8*, ...) @printf(i8* " + stringPtrExpr("\\n") + ")");
        lines.push_back("  br label %main.print_exit");
        lines.push_back("main.print_exit:");
        lines.push_back("  %t.main.exit = and i32 %t.main.ret, 255");
        lines.push_back("  %t.main.printf = call i32 (i8*, ...) @printf(i8* " + stringPtrExpr("%d\\n") + ", i32 %t.main.exit)");
        lines.push_back("  ret i32 0");
        lines.push_back("}");
    }

    void emitOneFunction(vector<string> &lines, const FuncDef &func) {
        allocas.clear();
        code.clear();
        loopLabels.clear();
        currentFuncIsVoid = func.isVoid;
        currentReturnLabel = newLabel("func.ret.");
        currentRetSlot.clear();
        pushScope();

        vector<string> params;
        vector<vector<int> > paramDims;
        for (size_t i = 0; i < func.params.size(); ++i) {
            vector<int> dims = evalDims(func.params[i].dims);
            paramDims.push_back(dims);
            ostringstream oss;
            if (func.params[i].isArray) {
                oss << pointerType(arrayParamPointeeType(dims));
            } else {
                oss << "i32";
            }
            oss << " %p" << i;
            params.push_back(oss.str());
        }

        ostringstream header;
        header << "define " << (func.isVoid ? "void" : "i32") << " @" << functionIRName(func.name)
               << "(" << join(params, ", ") << ") {";
        lines.push_back(header.str());
        if (!func.isVoid) {
            currentRetSlot = newTemp();
            emitAlloca(currentRetSlot + " = alloca i32");
            emit("store i32 0, i32* " + currentRetSlot);
        }

        for (size_t i = 0; i < func.params.size(); ++i) {
            const Param &param = func.params[i];
            Symbol sym;
            sym.isConst = false;
            sym.isGlobal = false;
            sym.isArrayParam = param.isArray;
            sym.dims = paramDims[i];
            sym.hasConstScalar = false;
            if (param.isArray) {
                sym.addr = "%p" + intValue(static_cast<int>(i));
                sym.totalRank = 1 + static_cast<int>(param.dims.size());
                sym.paramPointeeType = arrayParamPointeeType(paramDims[i]);
            } else {
                string slot = newTemp();
                emitAlloca(slot + " = alloca i32");
                emit("store i32 %p" + intValue(static_cast<int>(i)) + ", i32* " + slot);
                sym.addr = slot;
                sym.totalRank = 0;
                sym.paramPointeeType = "";
            }
            addSymbol(param.name, sym);
        }

        emitBlock(func.block, false);
        emit("br label %" + currentReturnLabel);
        emitLabel(currentReturnLabel);
        if (func.isVoid) {
            emit("ret void");
        } else {
            string rv = newTemp();
            emit(rv + " = load i32, i32* " + currentRetSlot);
            emit("ret i32 " + rv);
        }

        lines.push_back("entry:");
        for (size_t i = 0; i < allocas.size(); ++i) {
            lines.push_back(allocas[i]);
        }
        for (size_t i = 0; i < code.size(); ++i) {
            lines.push_back(code[i]);
        }
        lines.push_back("}");
        popScope();
    }

    void emitBlock(const shared_ptr<BlockStmt> &block, bool createScope) {
        if (createScope) {
            pushScope();
        }
        for (size_t i = 0; i < block->items.size(); ++i) {
            if (block->items[i].isDecl) {
                emitLocalDecl(block->items[i].decl);
            } else {
                emitStmt(block->items[i].stmt);
            }
        }
        if (createScope) {
            popScope();
        }
    }

    void emitLocalDecl(const Decl &decl) {
        for (size_t i = 0; i < decl.defs.size(); ++i) {
            vector<int> dims = evalDims(decl.defs[i].dims);
            Symbol sym;
            sym.isConst = decl.isConst;
            sym.isGlobal = false;
            sym.isArrayParam = false;
            sym.dims = dims;
            sym.totalRank = static_cast<int>(dims.size());
            sym.paramPointeeType = "";
            sym.hasConstScalar = false;
            string slot = newTemp();
            if (dims.empty()) {
                emitAlloca(slot + " = alloca i32");
                sym.addr = slot;
                int constInit = 0;
                if (decl.isConst) {
                    constInit = evalConstExpr(decl.defs[i].init->expr);
                    sym.hasConstScalar = true;
                    sym.constScalar = constInit;
                    ConstValue cv;
                    cv.isArray = false;
                    cv.scalar = constInit;
                    addConstBinding(decl.defs[i].name, cv);
                }
                if (decl.defs[i].init) {
                    string value = decl.isConst ? intValue(constInit) : emitScalarExpr(decl.defs[i].init->expr);
                    emit("store i32 " + value + ", i32* " + slot);
                }
            } else {
                emitAlloca(slot + " = alloca " + arrayType(dims));
                sym.addr = slot;
                vector<int> constValues;
                bool canUseConstStore = false;
                if (decl.isConst) {
                    sym.constArrayValues = buildConstArrayValues(dims, decl.defs[i].init);
                    constValues = sym.constArrayValues;
                    canUseConstStore = true;
                    ConstValue cv;
                    cv.isArray = true;
                    cv.dims = dims;
                    cv.values = sym.constArrayValues;
                    addConstBinding(decl.defs[i].name, cv);
                } else if (decl.defs[i].init) {
                    canUseConstStore = tryBuildConstArrayValues(dims, decl.defs[i].init, constValues);
                }
                if (decl.defs[i].init) {
                    if (canUseConstStore) {
                        emitConstArrayStore(slot, dims, constValues);
                    } else {
                        emitInitArray(slot, dims, decl.defs[i].init, false);
                    }
                }
            }
            addSymbol(decl.defs[i].name, sym);
        }
    }

    void emitInitArray(const string &basePtr,
                       const vector<int> &dims,
                       const shared_ptr<InitVal> &init,
                       bool isConstInit) {
        emitZeroArrayFill(basePtr, dims);

        vector<shared_ptr<Expr> > slots(static_cast<size_t>(product(dims)));
        size_t pos = 0;
        flattenRuntimeInit(init, dims, 0, slots, pos, 0);
        for (size_t idx = 0; idx < slots.size(); ++idx) {
            if (!slots[idx]) {
                continue;
            }
            vector<int> indices(dims.size(), 0);
            size_t rem = idx;
            for (int d = static_cast<int>(dims.size()) - 1; d >= 0; --d) {
                indices[static_cast<size_t>(d)] = static_cast<int>(rem % static_cast<size_t>(dims[static_cast<size_t>(d)]));
                rem /= static_cast<size_t>(dims[static_cast<size_t>(d)]);
            }
            string gep = newTemp();
            string arrTy = arrayType(dims);
            string inst = gep + " = getelementptr inbounds " + arrTy + ", " +
                          pointerType(arrTy) + " " + basePtr + ", i32 0";
            for (size_t k = 0; k < indices.size(); ++k) {
                inst += ", i32 " + intValue(indices[k]);
            }
            emit(inst);
            string value = isConstInit ? intValue(evalConstExpr(slots[idx])) : emitScalarExpr(slots[idx]);
            emit("store i32 " + value + ", i32* " + gep);
        }
    }

    void emitConstArrayStore(const string &basePtr,
                             const vector<int> &dims,
                             const vector<int> &values) {
        if (isAllZeroArray(values)) {
            emitZeroArrayFill(basePtr, dims);
            return;
        }
        emitConstArrayCopy(basePtr, dims, values);
    }

    void emitStmt(const shared_ptr<Stmt> &stmt) {
        if (!stmt) {
            return;
        }
        if (dynamic_cast<BlockStmt *>(stmt.get()) != NULL) {
            emitBlock(static_pointer_cast<BlockStmt>(stmt), true);
            return;
        }
        if (dynamic_cast<AssignStmt *>(stmt.get()) != NULL) {
            shared_ptr<AssignStmt> s = static_pointer_cast<AssignStmt>(stmt);
            emitStoreToLVal(s->lval, emitScalarExpr(s->expr));
            return;
        }
        if (dynamic_cast<ExprStmt *>(stmt.get()) != NULL) {
            shared_ptr<ExprStmt> s = static_pointer_cast<ExprStmt>(stmt);
            if (s->expr) {
                emitScalarExpr(s->expr);
            }
            return;
        }
        if (dynamic_cast<IfStmt *>(stmt.get()) != NULL) {
            shared_ptr<IfStmt> s = static_pointer_cast<IfStmt>(stmt);
            string thenLabel = newLabel("if.then.");
            string elseLabel = s->elseStmt ? newLabel("if.else.") : "";
            string endLabel = newLabel("if.end.");
            emitCond(s->cond, thenLabel, s->elseStmt ? elseLabel : endLabel);
            emitLabel(thenLabel);
            emitStmt(s->thenStmt);
            emit("br label %" + endLabel);
            if (s->elseStmt) {
                emitLabel(elseLabel);
                emitStmt(s->elseStmt);
                emit("br label %" + endLabel);
            }
            emitLabel(endLabel);
            return;
        }
        if (dynamic_cast<WhileStmt *>(stmt.get()) != NULL) {
            shared_ptr<WhileStmt> s = static_pointer_cast<WhileStmt>(stmt);
            string condLabel = newLabel("while.cond.");
            string bodyLabel = newLabel("while.body.");
            string endLabel = newLabel("while.end.");
            emit("br label %" + condLabel);
            emitLabel(condLabel);
            emitCond(s->cond, bodyLabel, endLabel);
            loopLabels.push_back(make_pair(endLabel, condLabel));
            emitLabel(bodyLabel);
            emitStmt(s->body);
            emit("br label %" + condLabel);
            loopLabels.pop_back();
            emitLabel(endLabel);
            return;
        }
        if (dynamic_cast<BreakStmt *>(stmt.get()) != NULL) {
            if (loopLabels.empty()) {
                throw runtime_error("break used outside loop");
            }
            emit("br label %" + loopLabels.back().first);
            string next = newLabel("after.break.");
            emitLabel(next);
            return;
        }
        if (dynamic_cast<ContinueStmt *>(stmt.get()) != NULL) {
            if (loopLabels.empty()) {
                throw runtime_error("continue used outside loop");
            }
            emit("br label %" + loopLabels.back().second);
            string next = newLabel("after.continue.");
            emitLabel(next);
            return;
        }
        if (dynamic_cast<ReturnStmt *>(stmt.get()) != NULL) {
            shared_ptr<ReturnStmt> s = static_pointer_cast<ReturnStmt>(stmt);
            if (currentFuncIsVoid) {
                emit("br label %" + currentReturnLabel);
            } else {
                string value = s->expr ? emitScalarExpr(s->expr) : "0";
                emit("store i32 " + value + ", i32* " + currentRetSlot);
                emit("br label %" + currentReturnLabel);
            }
            string next = newLabel("after.return.");
            emitLabel(next);
            return;
        }
        if (dynamic_cast<GetIntStmt *>(stmt.get()) != NULL) {
            shared_ptr<GetIntStmt> s = static_pointer_cast<GetIntStmt>(stmt);
            string reg = newTemp();
            emit(reg + " = call i32 @getint()");
            emitStoreToLVal(s->lval, reg);
            return;
        }
        if (dynamic_cast<PrintfStmt *>(stmt.get()) != NULL) {
            shared_ptr<PrintfStmt> s = static_pointer_cast<PrintfStmt>(stmt);
            vector<string> args;
            args.push_back("i8* " + stringPtrExpr(stripQuotes(s->format)));
            for (size_t i = 0; i < s->args.size(); ++i) {
                args.push_back("i32 " + emitScalarExpr(s->args[i]));
            }
            emit("call i32 (i8*, ...) @printf(" + join(args, ", ") + ")");
            string raw = stripQuotes(s->format);
            bool endsWithNewline = raw.size() >= 2 &&
                                   raw[raw.size() - 2] == '\\' &&
                                   raw[raw.size() - 1] == 'n';
            emitOutputStateUpdate(endsWithNewline ? "1" : "0");
            return;
        }
        throw runtime_error("Unsupported statement kind");
    }
};

int main() {
    try {
        ifstream fin("testfile.txt");
        if (!fin.is_open()) {
            cerr << "Cannot open testfile.txt" << endl;
            return 1;
        }
        stringstream buffer;
        buffer << fin.rdbuf();
        fin.close();

        Lexer lexer(buffer.str());
        vector<Token> tokens = lexer.tokenize();
        Parser parser(tokens);
        CompUnit program = parser.parseCompUnit();

        CodeGen codegen(program);
        ofstream fout("output.ll");
        if (!fout.is_open()) {
            cerr << "Cannot open output.ll" << endl;
            return 1;
        }
        fout << codegen.generate();
        fout.close();
        return 0;
    } catch (const exception &ex) {
        cerr << ex.what() << endl;
        return 1;
    }
}
