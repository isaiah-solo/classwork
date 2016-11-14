%{

#include <ctype.h>

#include "lyutils.h"
#include "auxlib.h"

#define IGNORE(THING) {}
#define YY_USER_ACTION { lexer_useraction(); }

//goto          { return yylval_token (TOK_GOTO); }
//.             { lexer_badchar(*yytext); }

%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn
%option yylineno

CHARCON       ('([^\\'\n]|\\[\\'"0nt])')
STRINGCON     (\"([^\\"\n]|\\[\\'"0nt])*\")
INTCON        (0|[1-9][0-9]*)
IDENT         ([[:alpha:]_][[:alnum:]_]*)

/**/
%%

"!"           { return yylval_token ('!'); }
"%"           { return yylval_token ('%'); }
"("           { return yylval_token ('('); }
")"           { return yylval_token (')'); }
"*"           { return yylval_token ('*'); }
"+"           { return yylval_token ('+'); }
","           { return yylval_token (','); }
"-"           { return yylval_token ('-'); }
"/"           { return yylval_token ('/'); }
"&"           { return yylval_token ('&'); }
";"           { return yylval_token (';'); }
":"           { return yylval_token (':'); }
"<"           { return yylval_token (TOK_LT); }
"="           { return yylval_token ('='); }
">"           { return yylval_token (TOK_GT); }
"{"           { return yylval_token ('{'); }
"}"           { return yylval_token ('}'); }
"[]"          { return yylval_token (TOK_ARRAY); }
"["           { return yylval_token ('['); }
"]"           { return yylval_token (']'); }
"'"           { return yylval_token ('\''); }
"."           { return yylval_token ('.'); }
"#".*         { lexer_include(); }

"->"          { return yylval_token (TOK_PTR); }
"!="          { return yylval_token (TOK_NE); }
"<="          { return yylval_token (TOK_LE); }
"=="          { return yylval_token (TOK_EQ); }
">="          { return yylval_token (TOK_GE); }

new           { return yylval_token (TOK_NEW); }
char          { return yylval_token (TOK_CHAR); }
if            { return yylval_token (TOK_IF); }
else          { return yylval_token (TOK_ELSE); }
ord           { return yylval_token (TOK_ORD); }
chr           { return yylval_token (TOK_CHR); }
int           { return yylval_token (TOK_INT); }
bool          { return yylval_token (TOK_BOOL); }
string        { return yylval_token (TOK_STRING); }
struct        { return yylval_token (TOK_STRUCT); }
return        { return yylval_token (TOK_RETURN); }
sizeof        { return yylval_token (TOK_SIZEOF); }
void          { return yylval_token (TOK_VOID); }
while         { return yylval_token (TOK_WHILE); }

{CHARCON}     { return yylval_token (TOK_CHARCON); }
{STRINGCON}   { return yylval_token (TOK_STRINGCON); }
{INTCON}      { return yylval_token (TOK_INTCON); }
{IDENT}       { return yylval_token (TOK_IDENT); }

[ \t]+        { IGNORE(white space); }

\n            { lexer_newline(); }

%%
