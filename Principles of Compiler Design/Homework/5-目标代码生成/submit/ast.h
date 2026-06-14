#pragma once

#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;
using std::string;
using std::vector;

enum class TokenKind {
    End,
    Ident,
    IntConst,
    FloatConst,
    StringConst,
    KwConst,
    KwInt,
    KwFloat,
    KwVoid,
    KwIf,
    KwElse,
    KwWhile,
    KwBreak,
    KwContinue,
    KwReturn,
    KwGetInt,
    KwGetFloat,
    KwGetFArray,
    KwPrintf,
    KwPutFloat,
    KwPutFArray,
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

enum class BaseType {
    Int,
    Float,
    Void
};

struct Expr {
    virtual ~Expr() {}
};

struct LVal {
    string name;
    vector<shared_ptr<Expr> > indices;
};

struct NumberExpr : Expr {
    NumberExpr(BaseType t, double v) : type(t), value(v) {}
    BaseType type;
    double value;
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
    BaseType baseType;
    vector<VarDef> defs;
};

struct Param {
    string name;
    BaseType baseType;
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
    BaseType returnType;
    bool isMain;
    vector<Param> params;
    shared_ptr<BlockStmt> block;
};

struct CompUnit {
    vector<Decl> globals;
    vector<FuncDef> funcs;
};
