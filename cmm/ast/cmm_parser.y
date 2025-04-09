%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strpool.h"
#include "ast.h"
#include "process.h"
void yyerror(const char *s);
extern int yylex();
AST PROGRAM; // make it global, so main() can get it.
%}


%code requires {
    #include "ast.h"
}

%union {
    int ival;
    char *sval;
    AST node;
}

%token <ival> NUM BOOLVAL
%token <sval> IDENT
%type <node> program decl_list decl var_decl var_list var_item func_decl proc_decl param_list_opt param_list param stmt_list stmt expr expr_stmt if_stmt while_stmt return_stmt block l_expr arg_list
%type <sval> type

/* Tokens */
%token INT BOOLEAN IF ELSE WHILE RETURN VOID
%token SEMICOLON COMMA COUNT LBRACKET RBRACKET LBRACE RBRACE LPAREN RPAREN
%token PLUS MINUS MUL DIV MOD EXP ASSIGN AND OR NOT LT LE GT GE EQ NE INC DEC

/* Operator Precedence */
%right ASSIGN
%left OR
%left AND
%nonassoc EQ NE
%nonassoc LT LE GT GE
%left PLUS MINUS
%left MUL DIV MOD
%right EXP
%right NOT
%precedence UMINUS
%nonassoc INC DEC

%%

program : decl_list { PROGRAM = $1 ; } ;

decl_list   : decl              { $$ = ast_list(ALIST_TOP_DECL, $1);}
            | decl_list decl    { $$ = append_to_list($1,$2) ; }
            ;

decl : var_decl | func_decl | proc_decl;

var_decl : type var_list SEMICOLON { $$ = ast_var_decl($1,$2);};

type : INT     { $$ = "int"; }  
     | BOOLEAN { $$ = "bool"; }
     ;

var_list    : var_item                  { $$ = ast_list(ALIST_VAR_DECL, $1) ; }
            | var_list COMMA var_item   { $$ = append_to_list($1,$3) ; }
            ;

var_item    : IDENT                         { $$ = ast_vd_item($1,NULL,NULL) ; }
            | IDENT ASSIGN expr             { $$ = ast_vd_item($1,NULL,$3) ; }
            | IDENT LBRACKET expr RBRACKET  { $$ = ast_vd_item($1,$3,NULL) ;}
            ;

func_decl   : type IDENT LPAREN param_list_opt RPAREN LBRACE stmt_list RBRACE 
              { $$ = ast_fundef($1, $2, $4, $7); }
            ;

proc_decl   : VOID IDENT LPAREN param_list_opt RPAREN LBRACE stmt_list RBRACE 
              { $$ = ast_fundef(NULL, $2, $4, $7); }
            ;

param_list_opt  : param_list    { $$ = $1;  }
                | /* empty */   { $$ = NULL; }
                ;

param_list  : param                     { $$ = ast_list(ALIST_PARAMS, $1) ; }
            | param_list COMMA param    { $$ = append_to_list($1,$3) ; }
            ;

param   : type IDENT                    {$$ = ast_param($1,$2,0) ; }
        | type IDENT LBRACKET RBRACKET  {$$ = ast_param($1,$2,1) ; }
        ;

stmt_list   : stmt              { $$ = ast_list(ALIST_STMTS, $1) ; }
            | stmt_list stmt    { $$ = append_to_list($1,$2) ; }
            ;

stmt : var_decl | expr_stmt | if_stmt | while_stmt | return_stmt | block;

expr_stmt : expr SEMICOLON { $$ = ast_es($1); };

if_stmt : IF LPAREN expr RPAREN stmt            { $$ = ast_if($3, $5, NULL); }
        | IF LPAREN expr RPAREN stmt ELSE stmt  { $$ = ast_if($3, $5, $7); }
        ;

while_stmt : WHILE LPAREN expr RPAREN stmt { $$ = ast_while($3, $5); } ;

return_stmt: RETURN expr SEMICOLON { $$ = ast_return($2); } ;

block : LBRACE stmt_list RBRACE  { $$ = $2; } ;

expr : l_expr ASSIGN expr        { $$ = ast_binop(BOP_ASSIGN, $1, $3); }
     | expr PLUS expr            { $$ = ast_binop(BOP_PLUS, $1, $3); }
     | expr MINUS expr           { $$ = ast_binop(BOP_MINUS, $1, $3); }
     | expr MUL expr             { $$ = ast_binop(BOP_MUL, $1, $3); }
     | expr DIV expr             { $$ = ast_binop(BOP_DIV, $1, $3); }
     | expr MOD expr             { $$ = ast_binop(BOP_MOD, $1, $3); }
     | expr EXP expr             { $$ = ast_binop(BOP_EXP, $1, $3); }
     | MINUS expr %prec UMINUS   { $$ = ast_unop(UOP_UMINUS, $2); }
     | LPAREN expr RPAREN        { $$ = $2; }
     | expr LT expr              { $$ = ast_binop(BOP_LT, $1, $3); }
     | expr LE expr              { $$ = ast_binop(BOP_LE, $1, $3); }
     | expr GT expr              { $$ = ast_binop(BOP_GT, $1, $3); }
     | expr GE expr              { $$ = ast_binop(BOP_GE, $1, $3); }
     | expr EQ expr              { $$ = ast_binop(BOP_EQ, $1, $3); }
     | expr NE expr              { $$ = ast_binop(BOP_NE, $1, $3); }
     | expr AND expr             { $$ = ast_binop(BOP_AND, $1, $3); }
     | expr OR expr              { $$ = ast_binop(BOP_OR, $1, $3); }
     | NOT expr                  { $$ = ast_unop(UOP_NOT, $2); }
     | IDENT LPAREN arg_list RPAREN { $$ = ast_funcall($1, $3); }
     | l_expr                    { $$ = $1; }
     | NUM                       { $$ = ast_int($1); }
     | BOOLVAL                   { $$ = ast_bool($1); }
     | l_expr INC                { $$ = ast_unop(UOP_POSTPP, $1); }
     | INC l_expr                { $$ = ast_unop(UOP_PREPP, $2); }
     | l_expr DEC                { $$ = ast_unop(UOP_POSTMM, $1); }
     | DEC l_expr                { $$ = ast_unop(UOP_PREMM, $2); }
     | COUNT IDENT               { $$ = ast_al($2) ;}
     ;

l_expr  : IDENT                         { $$ = ast_scalar($1) ; }
        | IDENT LBRACKET expr RBRACKET  { $$ = ast_ae($1,$3) ;  } 
        ;

arg_list    : expr                  { $$ = ast_list(ALIST_ARGS, $1) ; }
            | arg_list COMMA expr   { $$ = append_to_list($1,$3) ; }
            ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    if (yyparse() == 0) {
        process(PROGRAM);
    } else {
        fprintf(stderr,"parsing failed!\n") ; 
    }
    free_strpool() ;
    return 0;
}
