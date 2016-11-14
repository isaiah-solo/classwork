%{

#include "lyutils.h"
#include "astree.h"
#include "assert.h"

%}

%debug
%defines
%error-verbose
%token-table
%verbose

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING TOK_PROTOTYPE
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT TOK_NEWSTRING
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY TOK_PARAMLIST
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE TOK_FUNCTION
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON TOK_INDEX

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL TOK_RETURNVOID
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD TOK_DECLID
%token TOK_ORD TOK_CHR TOK_ROOT TOK_PTR TOK_GOTO TOK_SIZEOF TOK_VARDECL

%right TOK_IF TOK_ELSE
%right '='
%left TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left '+' '-'
%left '*' '/' '%'
%right TOK_POS TOK_NEG '!' TOK_ORD TOK_CHR
%left '[' '.' TOK_CALL
%nonassoc TOK_NEW

%start start

%%

start           : program               { yyparse_astree = $1; }
                ;

program         : program structdef     { $$ = adopt1 ($1, $2); }
                | program function      { $$ = adopt1 ($1, $2); }
                | program statement     { $$ = adopt1 ($1, $2); }
                | program error '}'     { $$ = $1; free_ast ($2); }
                | program error ';'     { $$ = $1; free_ast ($2); }
                |                       { $$ = new_parseroot (); }
                ;

structdef       : TOK_STRUCT TOK_IDENT '{' fields '}'
                                        { adopt1sym ($2, NULL,
                                          TOK_TYPEID);
                                          $$ = adopt2 ($1, $2, $4);
                                          free_ast2 ($3, $5); }
                | TOK_STRUCT TOK_IDENT '{' '}'
                                        { adopt1sym ($2, NULL,
                                          TOK_TYPEID);
                                          $$ = adopt1 ($1, $2);
                                          free_ast2 ($3, $4); }
                ;

fields          : fields fielddecl ';'  { $$ = adopt1 ($1, $2);
                                          free_ast ($3); }
                | fielddecl ';'         { $$ = $1; }
                ;

fielddecl       : basetype TOK_ARRAY TOK_IDENT
                                        { $$ = adopt2 ($1, $2,
                                          adopt1sym($3, NULL,
                                          TOK_FIELD)); }
                | basetype TOK_IDENT    { $$ = adopt1 ($1, 
                                          adopt1sym($2, NULL,
                                          TOK_FIELD)); }
                ;

basetype        : TOK_VOID              { $$ = $1; }
                | TOK_BOOL              { $$ = $1; }
                | TOK_CHAR              { $$ = $1; }
                | TOK_INT               { $$ = $1; }
                | TOK_STRING            { $$ = $1; }
                | TOK_IDENT             { adopt1sym ($1, NULL,
                                          TOK_TYPEID);
                                          $$ = $1; }
                ;

function        : identdecl '(' funcparams ')' block
                                        { astree* fn = new astree (
                                          TOK_FUNCTION, $1->filenr,
                                          $1->linenr, $1->offset, "");
                                          adopt1sym ($2, NULL, 
                                          TOK_PARAMLIST); 
                                          adopt1 ($2, $3);
                                          $$ = adopt3 (fn, $1, $2, 
                                          $5); 
                                          free_ast ($4); }
                | identdecl '(' ')' block
                                        { astree* fn = new astree (
                                          TOK_FUNCTION, $1->filenr,
                                          $1->linenr, $1->offset, "");
                                          adopt1sym ($2, NULL,
                                          TOK_PARAMLIST);
                                          $$ = adopt3 (fn, $1, $2,
                                          $4);
                                          free_ast ($3); }
                | identdecl '(' funcparams ')' ';'
                                        { astree* fn = new astree (
                                          TOK_PROTOTYPE, $1->filenr,
                                          $1->linenr, $1->offset, "");
                                          adopt1sym ($2, NULL,
                                          TOK_PARAMLIST);
                                          adopt1 ($2, $3);
                                          $$ = adopt2 (fn, $1, $2);
                                          free_ast2 ($4, $5); }
                | identdecl '(' ')' ';' { astree* fn = new astree (
                                          TOK_PROTOTYPE, $1->filenr,
                                          $1->linenr, $1->offset, "");
                                          adopt1sym ($2, NULL, 
                                          TOK_PARAMLIST);
                                          $$ = adopt2 (fn, $1, $2);
                                          free_ast2 ($3, $4); }
                ;

funcparams      : funcparams ',' identdecl
                                        { $$ = adopt1 ($1, $3);
                                          free_ast ($2); }
                | identdecl             { $$ = $1; }
                ;

identdecl       : basetype TOK_ARRAY TOK_IDENT
                                        { $$ = adopt2 ($1, $2,
                                          adopt1sym($3, NULL,
                                          TOK_DECLID)); }
                | basetype TOK_IDENT    { $$ = adopt1 ($1,
                                          adopt1sym($2, NULL,
                                          TOK_DECLID)); }
                ;

block           : '{' statements '}'    { $$ = adopt1 (adopt1sym(
                                          $1, NULL, TOK_BLOCK), $2);
                                          free_ast ($3); }
                | '{' '}'               { $$ = adopt1sym ($1, NULL,
                                          TOK_BLOCK);
                                          free_ast ($2); }
                | ';'                   { $$ = $1; }
                ;

statements      : statements statement  { $$ = adopt1 ($1, $2); }
                | statement             { $$ = $1; }
                ;

statement       : block                 { $$ = $1; }
                | vardecl               { $$ = $1; }
                | while                 { $$ = $1; }
                | ifelse                { $$ = $1; }
                | return                { $$ = $1; }
                | expr ';'              { $$ = $1; free_ast ($2); }
                ;

vardecl         : identdecl '=' expr ';'
                                        { adopt1sym ($2, NULL,
                                          TOK_VARDECL);
                                          free_ast ($4);
                                          $$ = adopt2 ($2, $1, $3); }
                ;

while           : TOK_WHILE '(' expr ')' statement
                                        { $$ = adopt2 ($1, $3, $5);
                                          free_ast2 ($2, $4); }
                ;

ifelse          : TOK_IF '(' expr ')' statement else
                                        { adopt1sym ($1, NULL,
                                          TOK_IFELSE);
                                          $$ = adopt3 ($1, $3, $5, $6);
                                          free_ast2 ($2, $4); }
                | TOK_IF '(' expr ')' statement
                                        { $$ = adopt2 ($1, $3, $5); 
                                          free_ast2 ($2, $4); }
                ;

else            : TOK_ELSE statement    { $$ = $2; 
                                          free_ast ($1); }
                ;

return          : TOK_RETURN expr ';'   { $$ = adopt1 ($1, $2);
                                          free_ast ($3); }
                | TOK_RETURN ';'        { adopt1sym ($1, NULL,
                                          TOK_RETURNVOID); }
                ;

expr            : BINOP                 { $$ = $1; }
                | UNOP                  { $$ = $1; }
                | allocator             { $$ = $1; }
                | call                  { $$ = $1; }
                | '(' expr ')'          { $$ = $2; 
                                          free_ast2 ($1, $3); }
                | variable              { $$ = $1; }
                | constant              { $$ = $1; }
                ;

allocator       : TOK_NEW TOK_IDENT '(' ')'
                                        { adopt1sym ($2, NULL,
                                          TOK_TYPEID);
                                          $$ = adopt1 ($1, $2);
                                          free_ast2 ($3, $4); }
                | TOK_NEW TOK_STRING '(' expr ')'
                                        { adopt1sym ($2, NULL,
                                          TOK_NEWSTRING);
                                          $$ = adopt1 ($2, $4);
                                          free_ast2 ($3, $5); }
                | TOK_NEW basetype '[' expr ']'
                                        { adopt1sym ($1, NULL,
                                          TOK_NEWARRAY);
                                          $$ = adopt2 ($1, $2, $4);
                                          free_ast2 ($3, $5); }
                ;

call            : TOK_IDENT '(' ')'     { adopt1sym ($2, NULL,
                                          TOK_CALL); 
                                          $$ = adopt1 ($2, $1);
                                          free_ast ($3); }
                | TOK_IDENT '(' callparams ')'
                                        { adopt1sym ($2, NULL,
                                          TOK_CALL);
                                          $$ = adopt2 ($2, $1, $3); 
                                          free_ast ($4); }
                ;

callparams      : callparams ',' expr   { $$ = adopt1 ($1, $3);
                                          free_ast ($2); }
                | expr                  { $$ = $1; }
                ;

variable        : TOK_IDENT             { $$ = $1; }
                | expr '[' expr ']'     { adopt1sym ($2, NULL, 
                                          TOK_INDEX); 
                                          $$ = adopt2 ($2, $1, $3); 
                                          free_ast ($4); }
                | expr '.' TOK_IDENT    { adopt1sym ($3, NULL,
                                          TOK_FIELD);
                                          $$ = adopt2 ($2, $1, $3); }
                ;

constant        : TOK_INTCON            { $$ = $1; }
                | TOK_CHARCON           { $$ = $1; }
                | TOK_STRINGCON         { $$ = $1; }
                | TOK_FALSE             { $$ = $1; }
                | TOK_TRUE              { $$ = $1; }
                | TOK_NULL              { $$ = $1; }
                ;

BINOP           : expr '=' expr         { $$ = adopt2 ($2, $1, $3); }
                | expr '+' expr         { $$ = adopt2 ($2, $1, $3); }
                | expr '-' expr         { $$ = adopt2 ($2, $1, $3); }
                | expr '*' expr         { $$ = adopt2 ($2, $1, $3); }
                | expr '/' expr         { $$ = adopt2 ($2, $1, $3); }
                | expr TOK_EQ expr      { $$ = adopt2 ($2, $1, $3); }
                | expr TOK_NE expr      { $$ = adopt2 ($2, $1, $3); }
                | expr TOK_LT expr      { $$ = adopt2 ($2, $1, $3); }
                | expr TOK_LE expr      { $$ = adopt2 ($2, $1, $3); }
                | expr TOK_GT expr      { $$ = adopt2 ($2, $1, $3); }
                | expr TOK_GE expr      { $$ = adopt2 ($2, $1, $3); }
                ;

UNOP            : '+' expr %prec TOK_POS
                                        { $$ = adopt1sym ($1,
                                          $2, TOK_POS); }
                | '-' expr %prec TOK_NEG
                                        { $$ = adopt1sym ($1,
                                          $2, TOK_NEG); }
                | '!' expr %prec '!'    { $$ = adopt1 ($1, $2); }
                | TOK_ORD expr %prec TOK_ORD
                                        { $$ = adopt1 ($1, $2); }
                | TOK_CHR expr %prec TOK_CHR
                                        { $$ = adopt1 ($1, $2); }
                ;

%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}

