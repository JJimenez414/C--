/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_CMM_PARSER_TAB_H_INCLUDED
# define YY_YY_CMM_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 14 "cmm_parser.y"

    #include "ast.h"

#line 53 "cmm_parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    BOOLVAL = 259,                 /* BOOLVAL  */
    IDENT = 260,                   /* IDENT  */
    INT = 261,                     /* INT  */
    BOOLEAN = 262,                 /* BOOLEAN  */
    IF = 263,                      /* IF  */
    ELSE = 264,                    /* ELSE  */
    WHILE = 265,                   /* WHILE  */
    RETURN = 266,                  /* RETURN  */
    VOID = 267,                    /* VOID  */
    SEMICOLON = 268,               /* SEMICOLON  */
    COMMA = 269,                   /* COMMA  */
    COUNT = 270,                   /* COUNT  */
    LBRACKET = 271,                /* LBRACKET  */
    RBRACKET = 272,                /* RBRACKET  */
    LBRACE = 273,                  /* LBRACE  */
    RBRACE = 274,                  /* RBRACE  */
    LPAREN = 275,                  /* LPAREN  */
    RPAREN = 276,                  /* RPAREN  */
    PLUS = 277,                    /* PLUS  */
    MINUS = 278,                   /* MINUS  */
    MUL = 279,                     /* MUL  */
    DIV = 280,                     /* DIV  */
    MOD = 281,                     /* MOD  */
    EXP = 282,                     /* EXP  */
    ASSIGN = 283,                  /* ASSIGN  */
    AND = 284,                     /* AND  */
    OR = 285,                      /* OR  */
    NOT = 286,                     /* NOT  */
    LT = 287,                      /* LT  */
    LE = 288,                      /* LE  */
    GT = 289,                      /* GT  */
    GE = 290,                      /* GE  */
    EQ = 291,                      /* EQ  */
    NE = 292,                      /* NE  */
    INC = 293,                     /* INC  */
    DEC = 294,                     /* DEC  */
    TERNARY_IF = 295,              /* TERNARY_IF  */
    TERNARY_ELSE = 296,            /* TERNARY_ELSE  */
    UMINUS = 297                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "cmm_parser.y"

    int ival;
    char *sval;
    AST node;

#line 118 "cmm_parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_CMM_PARSER_TAB_H_INCLUDED  */
