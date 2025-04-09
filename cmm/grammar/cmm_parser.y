%{
#include <stdio.h>
void yyerror(const char *s);
extern int yylex();
%}


%union {
    int ival;
    char *sval;
}

%token <ival> NUM BOOLVAL
%token <sval> IDENT
%type program decl_list decl var_decl var_list var_item func_decl proc_decl param_list_opt param_list param stmt_list stmt expr expr_stmt if_stmt while_stmt return_stmt block l_expr arg_list  type

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

program : decl_list ;

decl_list   : decl              
            | decl_list decl    
            ;

decl : var_decl | func_decl | proc_decl;

var_decl : type var_list SEMICOLON 

type : INT     
     | BOOLEAN 
     ;

var_list    : var_item                  
            | var_list COMMA var_item   
            ;

var_item    : IDENT                         
            | IDENT ASSIGN expr             
            | IDENT LBRACKET expr RBRACKET  
            ;

func_decl   : type IDENT LPAREN param_list_opt RPAREN LBRACE stmt_list RBRACE 
            ;

proc_decl   : VOID IDENT LPAREN param_list_opt RPAREN LBRACE stmt_list RBRACE 
            ;

param_list_opt  : param_list    
                | /* empty */   
                ;

param_list  : param                     
            | param_list COMMA param    
            ;

param   : type IDENT                    
        | type IDENT LBRACKET RBRACKET  
        ;

stmt_list   : stmt              
            | stmt_list stmt    
            ;

stmt : var_decl | expr_stmt | if_stmt | while_stmt | return_stmt | block;

expr_stmt : expr SEMICOLON 

if_stmt : IF LPAREN expr RPAREN stmt            
        | IF LPAREN expr RPAREN stmt ELSE stmt  
        ;

while_stmt : WHILE LPAREN expr RPAREN stmt 

return_stmt: RETURN expr SEMICOLON 

block : LBRACE stmt_list RBRACE  

expr : l_expr ASSIGN expr        
     | expr PLUS expr            
     | expr MINUS expr           
     | expr MUL expr             
     | expr DIV expr             
     | expr MOD expr             
     | expr EXP expr             
     | MINUS expr %prec UMINUS   
     | LPAREN expr RPAREN        
     | expr LT expr              
     | expr LE expr              
     | expr GT expr              
     | expr GE expr              
     | expr EQ expr              
     | expr NE expr              
     | expr AND expr             
     | expr OR expr              
     | NOT expr                  
     | IDENT LPAREN arg_list RPAREN 
     | l_expr                    
     | NUM                       
     | BOOLVAL                   
     | l_expr INC                
     | INC l_expr                
     | l_expr DEC                
     | DEC l_expr                
     | COUNT IDENT               
     ;

l_expr  : IDENT                         
        | IDENT LBRACKET expr RBRACKET  
        ;

arg_list    : expr                  
            | arg_list COMMA expr
            | /* empty */
            ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    if (yyparse() == 0) {
        fprintf(stderr,"Valid Program\n") ;         
    } else {
        fprintf(stderr,"parsing failed!\n") ; 
    }
    return 0;
}
