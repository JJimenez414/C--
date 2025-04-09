#include <stdio.h>
#include "ast.h"

static int node_count = 0;

void _puts(const char * str) {
    fputs(str, stdout) ; 
}

void traverse(AST node) {
    if (!node) return;
    node_count++;

    switch (node->kind) {
        case AST_INT:
            printf("%d",node->ival) ; 
            break ;

        case AST_BOOL:
            _puts(node->ival ? "true" : "false") ; 
            break ; 

        case AST_SCALAR:
            _puts(node->name) ; 
            break ; 


        case AST_AL:
            putchar('#') ; 
            _puts(node->name) ; 
            break ;

        case AST_AE:
            _puts(node->ae.array) ; 
            putchar('[') ; 
            traverse(node->ae.index);
            putchar(']') ; 
            break ;

        case AST_BINOP:
            putchar('(') ;
            _puts(BONames[node->binop.op]) ; 
            putchar(' ') ; 
            traverse(node->binop.a);
            putchar(' ') ; 
            traverse(node->binop.b);
            putchar(')') ; 
            break;

        case AST_UNOP:
            putchar('(') ; 
            switch (node->unop.op) {
                case UOP_UMINUS:  
                case UOP_NOT:
                case UOP_PREPP:
                case UOP_PREMM:
                    _puts(UONames[node->unop.op]) ; 
                    break ;
            }
            traverse(node->unop.a);
            switch (node->unop.op) {
                case UOP_POSTPP:
                case UOP_POSTMM:
                _puts(UONames[node->unop.op]) ; 
                break ;
            }
            putchar(')') ; 
            break;

        case AST_ES:
            traverse(node->ast);
            putchar(';') ; 
            break;

        case AST_IF:
            _puts("if") ; 
            putchar(' ') ; 
            traverse(node->if_stmt.cond);
            traverse(node->if_stmt.then_branch);
            if (node->if_stmt.else_branch) {
                putchar(' ') ; 
                _puts("else") ; 
                putchar(' ') ; 
                traverse(node->if_stmt.else_branch);
            }
            break;

        case AST_WHILE:
            _puts("while") ; 
            putchar(' ') ; 
            traverse(node->while_stmt.cond);
            putchar(' ') ; 
            traverse(node->while_stmt.body);
            break;

        case ALIST_TOP_DECL:
            for (int i = 0; i < node->list.length; i++)
            traverse(node->list.nodes[i]);
        break;

        case ALIST_PARAMS:
        case ALIST_ARGS:
            if (node->list.length) traverse(node->list.nodes[0]);
            for (int i = 1; i < node->list.length; i++) {
                putchar(',') ; 
                traverse(node->list.nodes[i]);
            }
            break;

        case AST_VAR_DECL:
            _puts(node->vdecl.type) ; 
            putchar(' ') ; 
            traverse(node->vdecl.vars->list.nodes[0]) ; 
            for (int i = 1; i < node->vdecl.vars->list.length; i++) {
                putchar(',') ; 
                putchar(' ') ; 
                traverse(node->vdecl.vars->list.nodes[i]) ; 
            }
            putchar(';') ; 
            break ; 

        case AST_VD_ITEM:
            _puts(node->vd_item.name) ; 
            if (node->vd_item.asize) { 
                putchar('[') ; 
                traverse(node->vd_item.asize) ; 
                putchar(']') ; 
            }
            if (node->vd_item.init) {
                putchar('=') ; 
                traverse(node->vd_item.init) ; 
            }
            break ; 

        case ALIST_STMTS:
            putchar('{') ; 
            for (int i = 0; i < node->list.length; i++)
                traverse(node->list.nodes[i]);
            putchar('}') ; 
            break;

        case AST_RETURN:
            printf("return") ; 
            putchar(' ') ; 
            traverse(node->ast);
            putchar(';') ; 
            break;

        case AST_FUNDEF:
            putchar('\n') ;
            _puts(node->fundef.type ? node->fundef.type : "void") ;
            putchar(' ') ; 
            _puts(node->fundef.name) ; 
            putchar('(') ;
            traverse(node->fundef.params)  ;            
            putchar(')') ;
            putchar(' ') ;
            traverse(node->fundef.body) ; 
            break ; 

        case AST_FUNCALL:
            _puts(node->funcall.name) ;
            putchar('(') ;
            traverse(node->funcall.args) ; 
            putchar(')') ;
            break ; 

        case AST_PARAM:
            _puts(node->param.type) ; 
            putchar(' ') ;
            _puts(node->param.name) ; 
            if (node->param.isarray) _puts("[]") ; 
            break ; 

        default:
            printf("don't know how to process %s\n",AstKindNames[node->kind]);
            break;
    }
}

void process(AST program) {
    traverse(program);
    printf("\nTotal AST nodes: %d\n", node_count);
}
