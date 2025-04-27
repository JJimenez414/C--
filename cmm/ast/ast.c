
// ast.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ast.h"

const char* AstKindNames[] = {
    /* AST_INT        */ "AST_INT",        // integer literal number
    /* AST_BOOL       */ "AST_BOOL",       // boolean literal: 1->true|0->false 
    /* AST_BINOP      */ "AST_BINOP",      // binary operator with two operands
    /* AST_UNOP       */ "AST_UNOP",       // unary operator with a single operand
    /* AST_ES         */ "AST_ES",         // Expression Statement: f(12) ; 
    /* AST_IF         */ "AST_IF",         // If statement
    /* AST_WHILE      */ "AST_WHILE",      // while statement
    /* AST_RETURN     */ "AST_RETURN",     // return statement
    /* AST_FUNCALL    */ "AST_FUNCALL",    // function call 
    /* AST_FUNDEF     */ "AST_FUNDEF",     // Function Definition
    /* AST_VAR_DECL   */ "AST_VAR_DECL",   // variable declaration statement
    /* AST_VD_ITEM    */ "AST_VD_ITEM",    // an item in the variable declaration (name, arr[#], or name=expr)
    /* AST_PARAM      */ "AST_PARAM",      // a single parameter declartion 
    /* AST_SCALAR     */ "AST_SCALAR",     // scalar variable : x 
    /* AST_AE         */ "AST_AE",         // (array element) arr[index]
    /* AST_AL         */ "AST_AL",         // (array length) #arr
    /* ALIST_STMTS    */ "ALIST_STMTS",    // list of statements 
    /* ALIST_PARAMS   */ "ALIST_PARAMS",   // list of parameters (function definition)
    /* ALIST_ARGS     */ "ALIST_ARGS",     // list of arguments (function call)
    /* ALIST_TOP_DECL */ "ALIST_TOP_DECL", // list of top-level declarations (functions and variables)
    /* ALIST_VAR_DECL */ "ALIST_VAR_DECL", // list of AST_VD_ITEM's (part of the AST_VAR_DECL statement)

    /* TERNARY        */ "AST_TERNARY",        // TERNARY STATEMENT
};


const char * BONames[] = {
    "**",
    "+",
    "-",
    "*",
    "/",
    "%%",
    "<",
    "<=",
    ">",
    ">=",
    "==",
    "!=",
    "&&",
    "||",
    "=",
}  ; 

const char * UONames[] = {
    "-",
    "!",
    "++",
    "--",
    "++",
    "--",
}  ; 


int evaluate_expression(AST node) {

    // if the node is a number return its value
    if (node->kind == AST_INT) {
        return node->ival;
    }  else if(node->kind == AST_BOOL) {
        return node->ival ? 1 : 0;
    } else if(node->kind == AST_SCALAR) {
        return -1;
    }

    // evalutate the expression and return its result
    switch (node->binop.op) {
        case BOP_PLUS: return evaluate_expression(node->binop.a) + evaluate_expression(node->binop.b);
        case BOP_MINUS: return evaluate_expression(node->binop.a) - evaluate_expression(node->binop.b);
        case BOP_MUL: return evaluate_expression(node->binop.a) * evaluate_expression(node->binop.b);
        // case BOP_EXP: return (int)pow(evaluate_expression(node->binop.a), evaluate_expression(node->binop.b));
        case BOP_DIV: return evaluate_expression(node->binop.b) != 0 ? evaluate_expression(node->binop.a) / evaluate_expression(node->binop.b) : 0; break;
        case BOP_MOD: return evaluate_expression(node->binop.b) != 0 ? evaluate_expression(node->binop.a) % evaluate_expression(node->binop.b) : 0; break;
        // comparison exceptions
        case BOP_LT:
            return evaluate_expression(node->binop.a) < evaluate_expression(node->binop.b); break;
        case BOP_LE:
            return evaluate_expression(node->binop.a) <= evaluate_expression(node->binop.b); break;
        case BOP_GT:
            return evaluate_expression(node->binop.a) > evaluate_expression(node->binop.b); break;
        case BOP_GE:
            return evaluate_expression(node->binop.a) >= evaluate_expression(node->binop.b); break;
        case BOP_EQ:
            return evaluate_expression(node->binop.a) == evaluate_expression(node->binop.b); break;
        case BOP_NE:
            return evaluate_expression(node->binop.a) != evaluate_expression(node->binop.b); break;
        case BOP_AND:
            return evaluate_expression(node->binop.a) && evaluate_expression(node->binop.b); break;
        case BOP_OR:
            return evaluate_expression(node->binop.a) || evaluate_expression(node->binop.b); break;
        default: return 0; // return 0 if for unknown binop
    }
    return -1 ;
}

AST ternary_check(AST node) {
 
    AST cond = node->ternary.cond ; // get condition

    AST then_branch = node->ternary.then_branch ; // store then_branch node
    AST else_branch = node->ternary.else_branch ; // store else_branch node

    bool result; // store the result for the condition

    // Don't apply constant folding to AND, OR, ASSIGN operations.
    if(cond->binop.op == BOP_AND || cond->binop.op == BOP_OR || cond->binop.op == BOP_ASSIGN) {
        return node;
    }
    else {
        result = evaluate_expression(cond);
    }
    
    // based on the result return the node we are going to traverse.
    return result ? then_branch : else_branch ;
}


AST newAST(AstKind _kind) {
    AST node = (AST) malloc(sizeof(struct __AST)) ; 
    node->kind = _kind ; 
    return node ; 
}

AST ast_int(int val) {
    AST n = newAST(AST_INT) ;
    n->ival = val;
    return n;
}

AST ast_bool(int val) {
    AST n = newAST(AST_BOOL) ; 
    n->ival = val;
    return n;
}

AST ast_var_decl(char *type, AST vars) {
    AST n = newAST(AST_VAR_DECL) ; 
    n->vdecl.type = type ; // int
    n->vdecl.vars = vars ; // a = 2;
    return n;
}

AST ast_vd_item(char *name, AST asize, AST init) {
    AST n = newAST(AST_VD_ITEM) ;
    n->vd_item.name = name ; // a
    n->vd_item.asize = asize ;  // null
    n->vd_item.init = init ;  // 2
    return n;
    
}


AST ast_unop(u_op op, AST a) {
    AST n = newAST(AST_UNOP) ; 
    n->unop.op = op;
    n->unop.a = a;
    return n;
}

// cond
AST ast_binop(b_op op, AST a, AST b) {
    AST n = newAST(AST_BINOP) ; 
    n->binop.op = op;
    n->binop.a = a;
    n->binop.b = b;
    return n;
}

AST ast_es(AST expr) {
    AST n = newAST(AST_ES) ; 
    n->ast = expr ; 
    return n;
}

AST ast_if(AST cond, AST then_branch, AST else_branch) {
    AST n = newAST(AST_IF) ; 
    n->if_stmt.cond = cond;
    n->if_stmt.then_branch = then_branch;
    n->if_stmt.else_branch = else_branch;
    return n;
}

AST ast_ternary(AST cond, AST then_branch, AST else_branch) { // handle ternary node

    AST n = newAST(AST_TERNARY);
    n->ternary.cond = cond;
    n->ternary.then_branch = then_branch;
    n->ternary.else_branch = else_branch;

    if (cond->kind == AST_BINOP) { // if the condition is a binary operation evalutate
        AST sub =  ternary_check(n) ;
        return sub ; // return the node that we are going to substitute the ternary for or the ternary node
    }
}

AST ast_while(AST cond, AST body) {
    AST n = newAST(AST_WHILE) ; 
    n->while_stmt.cond = cond;
    n->while_stmt.body = body;
    return n;
}

AST ast_funcall(char *name, AST args) {
    AST n = newAST(AST_FUNCALL) ; 
    n->funcall.name = name;
    n->funcall.args = args;
    return n;
}

AST ast_fundef(char *type, char *name, AST params, AST body) {
    AST n = newAST(AST_FUNDEF) ; 
    n->fundef.type = type;
    n->fundef.name = name;
    n->fundef.params = params;
    n->fundef.body = body;
    return n;
}

AST ast_return(AST expr) {
    AST n = newAST(AST_RETURN) ; 
    n->ast = expr;
    return n;
}

AST ast_list(AstKind kind, AST item) {
    AST n = newAST(kind) ; 
    n->list.nodes = malloc(sizeof(AST_TABLE));
    n->list.nodes[0] = item ; 
    n->list.length = 1;
    return n;
}

AST append_to_list(AST list, AST item) {
    list->list.length++;
    list->list.nodes = realloc(list->list.nodes , sizeof(AST) * (list->list.length));
    list->list.nodes[list->list.length-1] = item;
    return list ; 
}

AST ast_param(char *type, char *name, int isarray) {
    AST n = newAST(AST_PARAM) ;
    n->param.type = type ; 
    n->param.name = name ; 
    n->param.isarray = isarray ;  
    return n;
}

AST ast_scalar(char *name) {
    AST n = newAST(AST_SCALAR) ;
    n->name = name ; 
    return n;
}

AST ast_ae(char *array, AST index) {
    AST n = newAST(AST_AE) ;
    n->ae.array = array ; 
    n->ae.index = index ; 
    return n;
}

AST ast_al(char *name) {
    AST n = newAST(AST_AL) ;
    n->name = name ; 
    return n;
}
