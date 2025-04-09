%{
#include "cmm_parser.tab.h"
%}

%%
    /* Keywords */
"int"           { return INT; }
"boolean"       { return BOOLEAN; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }
"void"          { return VOID; }

    /* Boolean Literals */
"true"          { yylval.ival = 1; return BOOLVAL; }
"false"         { yylval.ival = 0; return BOOLVAL; }

    /* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = yytext; return IDENT; }

    /* Numbers */
[0-9]+        { yylval.ival = atoi(yytext); return NUM; }

    /* Operators */
"+"            { return PLUS; }
"-"            { return MINUS; }
"*"            { return MUL; }
"/"            { return DIV; }
"%"            { return MOD; }
"**"           { return EXP; }
"=="           { return EQ; }
"!="           { return NE; }
"<="           { return LE; }
"<"            { return LT; }
">"            { return GT; }
">="           { return GE; }
"&&"           { return AND; }
"||"           { return OR; }
"!"            { return NOT; }
"="            { return ASSIGN; }
"#"             { return COUNT; }

    /* Increment and Decrement */
"--"            { return DEC; }
"++"            { return INC; }

    /* Delimiters */
"("            { return LPAREN; }
")"            { return RPAREN; }
"{"            { return LBRACE; }
"}"            { return RBRACE; }
"["            { return LBRACKET; }
"]"            { return RBRACKET; }
";"            { return SEMICOLON; }
","            { return COMMA; }

[ \t\n]+      ; /* Ignore whitespace */
"//".*        ; /* Ignore single-line comments */
"/*"([^*]|\*+[^*/])*"*/" ; /* Ignore multi-line comments */

    /* Error Handling */
.             { printf("Unexpected character: %s\n", yytext); }
%%

int yywrap() { return 1; }
