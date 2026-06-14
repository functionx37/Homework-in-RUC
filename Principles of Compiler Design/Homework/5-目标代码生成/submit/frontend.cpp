#include "frontend.h"

#include "utils.h"

#include <cctype>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>

using namespace std;

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
            if (ch == '.' && pos + 1 < src.size() &&
                isdigit(static_cast<unsigned char>(src[pos + 1]))) {
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
            {"float", TokenKind::KwFloat},
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
        bool isFloat = false;
        if (src[pos] == '.' && pos + 1 < src.size() &&
            isdigit(static_cast<unsigned char>(src[pos + 1]))) {
            isFloat = true;
            ++pos;
            while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                ++pos;
            }
            if (pos < src.size() && (src[pos] == 'e' || src[pos] == 'E')) {
                ++pos;
                if (pos < src.size() && (src[pos] == '+' || src[pos] == '-')) {
                    ++pos;
                }
                while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                    ++pos;
                }
            }
        } else if (src[pos] == '0' && pos + 1 < src.size() &&
                   (src[pos + 1] == 'x' || src[pos + 1] == 'X')) {
            pos += 2;
            while (pos < src.size() && isxdigit(static_cast<unsigned char>(src[pos]))) {
                ++pos;
            }
            if (pos < src.size() && src[pos] == '.') {
                isFloat = true;
                ++pos;
                while (pos < src.size() && isxdigit(static_cast<unsigned char>(src[pos]))) {
                    ++pos;
                }
            }
            if (pos < src.size() && (src[pos] == 'p' || src[pos] == 'P')) {
                isFloat = true;
                ++pos;
                if (pos < src.size() && (src[pos] == '+' || src[pos] == '-')) {
                    ++pos;
                }
                while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                    ++pos;
                }
            }
        } else if (src[pos] == '0') {
            ++pos;
            while (pos < src.size() && src[pos] >= '0' && src[pos] <= '7') {
                ++pos;
            }
            if (pos < src.size() && src[pos] == '.') {
                isFloat = true;
                ++pos;
                while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                    ++pos;
                }
            }
            if (pos < src.size() && (src[pos] == 'e' || src[pos] == 'E')) {
                isFloat = true;
                ++pos;
                if (pos < src.size() && (src[pos] == '+' || src[pos] == '-')) {
                    ++pos;
                }
                while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                    ++pos;
                }
            }
        } else {
            while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                ++pos;
            }
            if (pos < src.size() && src[pos] == '.') {
                isFloat = true;
                ++pos;
                while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                    ++pos;
                }
            }
            if (pos < src.size() && (src[pos] == 'e' || src[pos] == 'E')) {
                isFloat = true;
                ++pos;
                if (pos < src.size() && (src[pos] == '+' || src[pos] == '-')) {
                    ++pos;
                }
                while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                    ++pos;
                }
            }
        }
        return {isFloat ? TokenKind::FloatConst : TokenKind::IntConst,
                src.substr(start, pos - start), line};
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
        if (!check(TokenKind::KwInt) && !check(TokenKind::KwFloat)) {
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
        return check(TokenKind::KwConst) || check(TokenKind::KwInt) || check(TokenKind::KwFloat);
    }

    BaseType parseBType() {
        if (match(TokenKind::KwInt)) {
            return BaseType::Int;
        }
        if (match(TokenKind::KwFloat)) {
            return BaseType::Float;
        }
        ostringstream oss;
        oss << "Line " << cur().line << ": expected type specifier";
        throw runtime_error(oss.str());
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
        BaseType baseType = parseBType();
        Decl decl;
        decl.isConst = true;
        decl.baseType = baseType;
        decl.defs.push_back(parseConstDef());
        while (match(TokenKind::Comma)) {
            decl.defs.push_back(parseConstDef());
        }
        expect(TokenKind::Semicolon, "expected ';' after const declaration");
        return decl;
    }

    Decl parseVarDecl() {
        BaseType baseType = parseBType();
        Decl decl;
        decl.isConst = false;
        decl.baseType = baseType;
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
            func.returnType = parseBType();
        } else {
            func.returnType = BaseType::Void;
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
        func.returnType = BaseType::Int;
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
        BaseType baseType = parseBType();
        Param param;
        param.baseType = baseType;
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
        if (check(TokenKind::IntConst)) {
            return shared_ptr<Expr>(new NumberExpr(
                BaseType::Int,
                static_cast<double>(parseIntLiteral(expect(TokenKind::IntConst, "expected integer literal").text))));
        }
        if (check(TokenKind::FloatConst)) {
            return shared_ptr<Expr>(new NumberExpr(
                BaseType::Float,
                parseFloatLiteral(expect(TokenKind::FloatConst, "expected numeric literal").text)));
        }
        expect(TokenKind::IntConst, "expected numeric literal");
        return shared_ptr<Expr>();
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

CompUnit parseSource(const string &source) {
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    return parser.parseCompUnit();
}
