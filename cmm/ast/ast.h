// ast.h
#ifndef AST_H
#define AST_H

typedef enum {
    AST_INT,        // integer literal number
    AST_BOOL,       // boolean literal: 1->true|0->false 
    AST_BINOP,      // binary operator with two operands
    AST_UNOP,       // unary operator with a single operand
    AST_ES,         // Expression Statement: f(12) ; 
    AST_IF,         // If statement
    AST_WHILE,      // while statement
    AST_RETURN,     // return statement
    AST_FUNCALL,    // function call 
    AST_FUNDEF,     // Function Definition
    AST_VAR_DECL,   // variable declaration statement
    AST_VD_ITEM,    // an item in the variable declaration (name, arr[#], or name=expr)
    AST_PARAM,      // a single parameter declartion 
    AST_SCALAR,     // scalar variable : x 
    AST_AE,         // (array element) arr[index]
    AST_AL,         // (array length) #arr
    ALIST_STMTS,    // list of statements 
    ALIST_PARAMS,   // list of parameters (function definition)
    ALIST_ARGS,     // list of arguments (function call)
    ALIST_TOP_DECL, // list of top-level declarations (functions and variables)
    ALIST_VAR_DECL, // list of AST_VD_ITEM's (part of the AST_VAR_DECL statement)
} AstKind;

typedef enum {
    BOP_EXP,
    BOP_PLUS,
    BOP_MINUS,
    BOP_MUL,
    BOP_DIV,
    BOP_MOD,
    BOP_LT,
    BOP_LE,
    BOP_GT,
    BOP_GE,
    BOP_EQ,
    BOP_NE,
    BOP_AND,
    BOP_OR,
    BOP_ASSIGN,
} b_op ; 

typedef enum {
    UOP_UMINUS,
    UOP_NOT,
    UOP_PREPP,
    UOP_PREMM,
    UOP_POSTPP,
    UOP_POSTMM,
} u_op ; 


extern const char* AstKindNames[] ;
extern const char* BONames[] ;
extern const char* UONames[] ;

typedef struct __AST *AST, **AST_TABLE ;

struct __AST {
    AstKind kind;
    union {
        int ival;       // AST_INT AST_BOOL
        char *name;     // AST_SCALAR AST_AL (array length)
        AST ast;        // AST_RETURN AST_ES (expression statement)
        struct { char *array; AST index; } ae ;                     // AST_AE (array element)
        struct { u_op op; AST a; } unop;                            // AST_UNOP
        struct { b_op op; AST a, b; } binop;                        // AST_BINOP
        struct { AST cond, then_branch, else_branch; } if_stmt;     // AST_IF
        struct { AST cond, body; } while_stmt;                      // AST_WHILE
        struct { char *name; AST args; } funcall;                   // AST_FUNCALL
        struct { char *type; char *name; AST params; AST body; } fundef; // AST_FUNDEF
        struct { AST_TABLE nodes; int length; } list;               // ALIST_TOP_DECL ALIST_VAR_DECL ALIST_PARAMS ALIST_ARGS ALIST_STMT
        struct { char *type; AST vars;} vdecl ;                     // AST_VAR_DECL
        struct { char *name; AST asize; AST init ; } vd_item ;      // AST_VD_ITEM 
        struct { char *type; char *name; int isarray ;} param ;     // AST_PARAM
    };
} ;

AST ast_int(int val);
AST ast_bool(int val);
AST ast_unop(u_op op, AST a);
AST ast_binop(b_op op, AST a, AST b);
AST ast_es(AST expr);
AST ast_if(AST cond, AST then_branch, AST else_branch);
AST ast_while(AST cond, AST body);
AST ast_funcall(char *name, AST args);
AST ast_fundef(char *type, char *name, AST params, AST body);
AST ast_return(AST expr);

AST ast_list(AstKind, AST node);
AST append_to_list(AST ast, AST item) ;

AST ast_var_decl(char *name, AST vars) ;
AST ast_vd_item(char *name, AST asize, AST init) ; 
AST ast_param(char *type, char *name, int isarray) ; 

AST ast_scalar(char *name) ; 
AST ast_ae(char *array, AST index) ;
AST ast_al(char *name) ; 

#endif
