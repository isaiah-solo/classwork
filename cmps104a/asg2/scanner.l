%{

#include <ctype.h>

#include "lyutils.h"
#include "auxlib.h"

#define YY_USER_ACTION { lex_offset += yyleng; }

int lex_linenr = 1;
int lex_offset = 0;

void lex_newline() {
   lex_offset = 0; ++lex_linenr;
}

void lex_error() {
   char message[64];
   sprintf (message, isprint (*yytext)
            ? "stray '%c' in program"
            : "stray '\x02X' in nprogram", *yytext);
   yyerror (message);
}

%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn
%option yylineno

CHARCON    ('([^\\'\n]|\\[\\'"0nt])')
STRINGCON  (\"([^\\"\n]|\\[\\'"0nt])*\")
INTCON     (0|[1-9][0-9]*)
IDENT      ([[:alpha:]_][[:alnum:]_]*)

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
"<"           { return yylval_token ('<'); }
"="           { return yylval_token ('='); }
">"           { return yylval_token ('>'); }
"{"           { return yylval_token ('{'); }
"}"           { return yylval_token ('}'); }
"\n"          { lexer_newline(); return '\n'; }
"#".*         { lexer_include(); }

"->"          { return yylval_token (TOK_PTR); }
"!="          { return yylval_token (TOK_NE); }
"<="          { return yylval_token (TOK_LT); }
"=="          { return yylval_token (TOK_EQ); }
">="          { return yylval_token (TOK_GE); }

char          { return yylval_token (TOK_CHAR); }
goto          { return yylval_token (TOK_GOTO); }
if            { return yylval_token (TOK_IF); }
int           { return yylval_token (TOK_INT); }
return        { return yylval_token (TOK_RETURN); }
sizeof        { return yylval_token (TOK_SIZEOF); }
struct        { return yylval_token (TOK_STRUCT); }
void          { return yylval_token (TOK_VOID); }

{CHARCON}     { return yylval_token (TOK_CHARCON); }
{STRINGCON}   { return yylval_token (TOK_STRINGCON); }
{INTCON}      { return yylval_token (TOK_INTCON); }
{IDENT}       { return yylval_token (TOK_IDENT); }

[ \t]+        { }
.             { lex_error(); }

%%

