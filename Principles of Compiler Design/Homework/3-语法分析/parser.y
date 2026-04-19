%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
extern FILE* yyin;

FILE* fout;

void yyerror(const char* s);

#define P(s) fprintf(fout, "%s\n", s)
%}

%token CONSTTK INTTK VOIDTK
%token IFTK ELSETK WHILETK BREAKTK CONTINUETK RETURNTK
%token PRINTFTK
%token IDENFR INTCON STRCON
%token LEQ GEQ EQL NEQ AND OR
%token PLUS MINU MULT DIV MOD NOT
%token LSS GRE ASSIGN
%token SEMICN COMMA
%token LPARENT RPARENT LBRACK RBRACK LBRACE RBRACE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSETK

%start CompUnit

%%

/* ================================================================
   CompUnit → {Decl} {FuncDef} MainFuncDef
   Restructured: extract INTTK IDENFR prefix to resolve LALR(1)
   ambiguity between VarDecl and FuncDef.
   ================================================================ */

CompUnit
    : CompUnitItem                          { P("<CompUnit>"); }
    | CompUnit CompUnitItem                 { P("<CompUnit>"); }
    ;

CompUnitItem
    : ConstDecl
    | INTTK IDENFR IntIdentRest
    | VOIDTK { P("<FuncType>"); } IDENFR LPARENT FuncFParamsOpt RPARENT Block
                                            { P("<FuncDef>"); }
    ;

IntIdentRest
    : VarDefSuffix { P("<VarDef>"); } VarDefListTail SEMICN
                                            { P("<VarDecl>"); }
    | LPARENT { P("<FuncType>"); } FuncFParamsOpt RPARENT Block
                                            { P("<FuncDef>"); }
    ;

VarDefSuffix
    : VarDefDims
    | VarDefDims ASSIGN InitVal
    ;

VarDefListTail
    : /* empty */
    | VarDefListTail COMMA VarDef
    ;

/* ================================================================
   ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';'
   ================================================================ */

ConstDecl
    : CONSTTK INTTK ConstDefList SEMICN     { P("<ConstDecl>"); }
    ;

ConstDefList
    : ConstDef
    | ConstDefList COMMA ConstDef
    ;

ConstDef
    : IDENFR ConstDims ASSIGN ConstInitVal  { P("<ConstDef>"); }
    ;

ConstDims
    : /* empty */
    | ConstDims LBRACK ConstExp RBRACK
    ;

/* ConstInitVal → ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}' */
ConstInitVal
    : ConstExp                              { P("<ConstInitVal>"); }
    | LBRACE RBRACE                         { P("<ConstInitVal>"); }
    | LBRACE ConstInitValList RBRACE        { P("<ConstInitVal>"); }
    ;

ConstInitValList
    : ConstInitVal
    | ConstInitValList COMMA ConstInitVal
    ;

/* ================================================================
   VarDecl → BType VarDef { ',' VarDef } ';'
   (used inside Block; CompUnit uses the restructured path above)
   ================================================================ */

VarDecl
    : INTTK VarDefList SEMICN              { P("<VarDecl>"); }
    ;

VarDefList
    : VarDef
    | VarDefList COMMA VarDef
    ;

VarDef
    : IDENFR VarDefDims                     { P("<VarDef>"); }
    | IDENFR VarDefDims ASSIGN InitVal      { P("<VarDef>"); }
    ;

VarDefDims
    : /* empty */
    | VarDefDims LBRACK ConstExp RBRACK
    ;

/* InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}' */
InitVal
    : Exp                                   { P("<InitVal>"); }
    | LBRACE RBRACE                         { P("<InitVal>"); }
    | LBRACE InitValList RBRACE             { P("<InitVal>"); }
    ;

InitValList
    : InitVal
    | InitValList COMMA InitVal
    ;

/* ================================================================
   FuncFParams → FuncFParam { ',' FuncFParam }
   ================================================================ */

FuncFParamsOpt
    : /* empty */
    | FuncFParamsList                       { P("<FuncFParams>"); }
    ;

FuncFParamsList
    : FuncFParam
    | FuncFParamsList COMMA FuncFParam
    ;

FuncFParam
    : INTTK IDENFR                          { P("<FuncFParam>"); }
    | INTTK IDENFR LBRACK RBRACK FuncFParamDims
                                            { P("<FuncFParam>"); }
    ;

FuncFParamDims
    : /* empty */
    | FuncFParamDims LBRACK Exp RBRACK
    ;

/* ================================================================
   Block → '{' { BlockItem } '}'
   ================================================================ */

Block
    : LBRACE BlockItems RBRACE              { P("<Block>"); }
    ;

BlockItems
    : /* empty */
    | BlockItems BlockItem
    ;

BlockItem
    : ConstDecl
    | VarDecl
    | Stmt
    ;

/* ================================================================
   Stmt (11 forms)
   ================================================================ */

Stmt
    : LVal ASSIGN Exp SEMICN                { P("<Stmt>"); }
    | Exp SEMICN                            { P("<Stmt>"); }
    | SEMICN                                { P("<Stmt>"); }
    | Block                                 { P("<Stmt>"); }
    | IFTK LPARENT Cond RPARENT Stmt ELSETK Stmt
                                            { P("<Stmt>"); }
    | IFTK LPARENT Cond RPARENT Stmt %prec LOWER_THAN_ELSE
                                            { P("<Stmt>"); }
    | WHILETK LPARENT Cond RPARENT Stmt     { P("<Stmt>"); }
    | BREAKTK SEMICN                        { P("<Stmt>"); }
    | CONTINUETK SEMICN                     { P("<Stmt>"); }
    | RETURNTK SEMICN                       { P("<Stmt>"); }
    | RETURNTK Exp SEMICN                   { P("<Stmt>"); }
    | PRINTFTK LPARENT FormatString PrintfArgsTail RPARENT SEMICN
                                            { P("<Stmt>"); }
    ;

FormatString
    : STRCON                                { P("<FormatString>"); }
    ;

PrintfArgsTail
    : /* empty */
    | PrintfArgsTail COMMA Exp
    ;

/* ================================================================
   Expression hierarchy (all left-recursive)
   ================================================================ */

Exp
    : AddExp                                { P("<Exp>"); }
    ;

Cond
    : LOrExp                                { P("<Cond>"); }
    ;

ConstExp
    : AddExp                                { P("<ConstExp>"); }
    ;

/* LOrExp → LAndExp | LOrExp '||' LAndExp */
LOrExp
    : LAndExp                               { P("<LOrExp>"); }
    | LOrExp OR LAndExp                     { P("<LOrExp>"); }
    ;

/* LAndExp → EqExp | LAndExp '&&' EqExp */
LAndExp
    : EqExp                                 { P("<LAndExp>"); }
    | LAndExp AND EqExp                     { P("<LAndExp>"); }
    ;

/* EqExp → RelExp | EqExp ('==' | '!=') RelExp */
EqExp
    : RelExp                                { P("<EqExp>"); }
    | EqExp EQL RelExp                      { P("<EqExp>"); }
    | EqExp NEQ RelExp                      { P("<EqExp>"); }
    ;

/* RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp */
RelExp
    : AddExp                                { P("<RelExp>"); }
    | RelExp LSS AddExp                     { P("<RelExp>"); }
    | RelExp GRE AddExp                     { P("<RelExp>"); }
    | RelExp LEQ AddExp                     { P("<RelExp>"); }
    | RelExp GEQ AddExp                     { P("<RelExp>"); }
    ;

/* AddExp → MulExp | AddExp ('+' | '-') MulExp */
AddExp
    : MulExp                                { P("<AddExp>"); }
    | AddExp PLUS MulExp                    { P("<AddExp>"); }
    | AddExp MINU MulExp                    { P("<AddExp>"); }
    ;

/* MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp */
MulExp
    : UnaryExp                              { P("<MulExp>"); }
    | MulExp MULT UnaryExp                  { P("<MulExp>"); }
    | MulExp DIV UnaryExp                   { P("<MulExp>"); }
    | MulExp MOD UnaryExp                   { P("<MulExp>"); }
    ;

/* UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp */
UnaryExp
    : PrimaryExp                            { P("<UnaryExp>"); }
    | IDENFR LPARENT RPARENT                { P("<UnaryExp>"); }
    | IDENFR LPARENT FuncRParams RPARENT    { P("<UnaryExp>"); }
    | UnaryOp UnaryExp                      { P("<UnaryExp>"); }
    ;

UnaryOp
    : PLUS                                  { P("<UnaryOp>"); }
    | MINU                                  { P("<UnaryOp>"); }
    | NOT                                   { P("<UnaryOp>"); }
    ;

/* PrimaryExp → '(' Exp ')' | LVal | Number */
PrimaryExp
    : LPARENT Exp RPARENT                   { P("<PrimaryExp>"); }
    | LVal                                  { P("<PrimaryExp>"); }
    | Number                                { P("<PrimaryExp>"); }
    ;

Number
    : INTCON                                { P("<Number>"); }
    ;

/* LVal → Ident { '[' Exp ']' } */
LVal
    : IDENFR LValDims                       { P("<LVal>"); }
    ;

LValDims
    : /* empty */
    | LValDims LBRACK Exp RBRACK
    ;

/* FuncRParams → Exp { ',' Exp } */
FuncRParams
    : FuncRParamsList                       { P("<FuncRParams>"); }
    ;

FuncRParamsList
    : Exp
    | FuncRParamsList COMMA Exp
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int main() {
    yyin = fopen("testfile.txt", "r");
    if (!yyin) {
        fprintf(stderr, "Cannot open testfile.txt\n");
        return 1;
    }
    fout = fopen("output.txt", "w");
    if (!fout) {
        fprintf(stderr, "Cannot open output.txt\n");
        return 1;
    }
    yyparse();
    fclose(fout);
    fclose(yyin);
    return 0;
}
