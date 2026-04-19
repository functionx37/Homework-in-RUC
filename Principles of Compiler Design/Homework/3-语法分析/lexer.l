%top{
#define _POSIX_C_SOURCE 200809L
}

%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
%}

%option noyywrap
%option yylineno

%x BLOCK_COMMENT

%%

 /* ========== 注释 ========== */
"//"[^\n]*              { /* single-line comment */ }
"/*"                    { BEGIN(BLOCK_COMMENT); }
<BLOCK_COMMENT>"*/"    { BEGIN(INITIAL); }
<BLOCK_COMMENT>\n      { /* newline in comment */ }
<BLOCK_COMMENT>.       { /* skip */ }

 /* ========== 关键字 ========== */
"const"                 { return CONSTTK; }
"int"                   { return INTTK; }
"void"                  { return VOIDTK; }
"if"                    { return IFTK; }
"else"                  { return ELSETK; }
"while"                 { return WHILETK; }
"break"                 { return BREAKTK; }
"continue"              { return CONTINUETK; }
"return"                { return RETURNTK; }
"printf"                { return PRINTFTK; }

 /* ========== 标识符 ========== */
[a-zA-Z_][a-zA-Z0-9_]* { return IDENFR; }

 /* ========== 整数常量（与 lexer.cpp 一致：十六进制、八进制、十进制） ========== */
0[xX][0-9a-fA-F]+       { return INTCON; }
0[0-7]+                 { return INTCON; }
0[0-9]+                 { return INTCON; }
[1-9][0-9]*             { return INTCON; }
0                       { return INTCON; }

 /* ========== 格式化字符串 ========== */
\"[^\"]*\"              { return STRCON; }

 /* ========== 双字符运算符（必须在单字符之前） ========== */
"<="                    { return LEQ; }
">="                    { return GEQ; }
"=="                    { return EQL; }
"!="                    { return NEQ; }
"&&"                    { return AND; }
"||"                    { return OR; }

 /* ========== 单字符运算符与分隔符 ========== */
"+"                     { return PLUS; }
"-"                     { return MINU; }
"*"                     { return MULT; }
"/"                     { return DIV; }
"%"                     { return MOD; }
"!"                     { return NOT; }
"<"                     { return LSS; }
">"                     { return GRE; }
"="                     { return ASSIGN; }
";"                     { return SEMICN; }
","                     { return COMMA; }
"("                     { return LPARENT; }
")"                     { return RPARENT; }
"["                     { return LBRACK; }
"]"                     { return RBRACK; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }

 /* ========== 空白与换行 ========== */
\n                      { /* newline */ }
[ \t\r]+                { /* skip whitespace */ }

 /* ========== 其他字符 ========== */
.                       { /* skip unknown */ }

%%
