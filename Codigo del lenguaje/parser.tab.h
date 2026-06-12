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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOKEN_VOID = 258,              /* TOKEN_VOID  */
    TOKEN_MAIN = 259,              /* TOKEN_MAIN  */
    TOKEN_FUNCTION = 260,          /* TOKEN_FUNCTION  */
    TOKEN_INT = 261,               /* TOKEN_INT  */
    TOKEN_CHAR = 262,              /* TOKEN_CHAR  */
    TOKEN_STRING_TYPE = 263,       /* TOKEN_STRING_TYPE  */
    TOKEN_BOOL = 264,              /* TOKEN_BOOL  */
    TOKEN_ARRAY = 265,             /* TOKEN_ARRAY  */
    TOKEN_IF = 266,                /* TOKEN_IF  */
    TOKEN_ELSE = 267,              /* TOKEN_ELSE  */
    TOKEN_FOR = 268,               /* TOKEN_FOR  */
    TOKEN_WHILE = 269,             /* TOKEN_WHILE  */
    TOKEN_RETURN = 270,            /* TOKEN_RETURN  */
    TOKEN_INPUT = 271,             /* TOKEN_INPUT  */
    TOKEN_PRINT = 272,             /* TOKEN_PRINT  */
    TOKEN_DRAW = 273,              /* TOKEN_DRAW  */
    TOKEN_TIME = 274,              /* TOKEN_TIME  */
    TOKEN_OPEN_WINDOW = 275,       /* TOKEN_OPEN_WINDOW  */
    TOKEN_IS_KEY_PRESSED = 276,    /* TOKEN_IS_KEY_PRESSED  */
    TOKEN_TRUE = 277,              /* TOKEN_TRUE  */
    TOKEN_FALSE = 278,             /* TOKEN_FALSE  */
    TOKEN_PAR_A = 279,             /* TOKEN_PAR_A  */
    TOKEN_PAR_C = 280,             /* TOKEN_PAR_C  */
    TOKEN_CORCH_A = 281,           /* TOKEN_CORCH_A  */
    TOKEN_CORCH_C = 282,           /* TOKEN_CORCH_C  */
    TOKEN_LLAVE_A = 283,           /* TOKEN_LLAVE_A  */
    TOKEN_LLAVE_C = 284,           /* TOKEN_LLAVE_C  */
    TOKEN_PUNTOYCOMA = 285,        /* TOKEN_PUNTOYCOMA  */
    TOKEN_COMA = 286,              /* TOKEN_COMA  */
    TOKEN_LIT_INT = 287,           /* TOKEN_LIT_INT  */
    TOKEN_ID = 288,                /* TOKEN_ID  */
    TOKEN_HEX_COLOR = 289,         /* TOKEN_HEX_COLOR  */
    TOKEN_LIT_CHAR = 290,          /* TOKEN_LIT_CHAR  */
    TOKEN_LIT_STRING = 291,        /* TOKEN_LIT_STRING  */
    TOKEN_OP_ASIG = 292,           /* TOKEN_OP_ASIG  */
    TOKEN_OP_OR = 293,             /* TOKEN_OP_OR  */
    TOKEN_OP_AND = 294,            /* TOKEN_OP_AND  */
    TOKEN_OP_IGUAL = 295,          /* TOKEN_OP_IGUAL  */
    TOKEN_OP_DISTINTO = 296,       /* TOKEN_OP_DISTINTO  */
    TOKEN_OP_MENOR = 297,          /* TOKEN_OP_MENOR  */
    TOKEN_OP_MAYOR = 298,          /* TOKEN_OP_MAYOR  */
    TOKEN_OP_MENORIGUAL = 299,     /* TOKEN_OP_MENORIGUAL  */
    TOKEN_OP_MAYORIGUAL = 300,     /* TOKEN_OP_MAYORIGUAL  */
    TOKEN_OP_SUM = 301,            /* TOKEN_OP_SUM  */
    TOKEN_OP_RES = 302,            /* TOKEN_OP_RES  */
    TOKEN_OP_MUL = 303,            /* TOKEN_OP_MUL  */
    TOKEN_OP_DIV = 304,            /* TOKEN_OP_DIV  */
    TOKEN_OP_POT = 305,            /* TOKEN_OP_POT  */
    TOKEN_OP_NOT = 306             /* TOKEN_OP_NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 127 "parser.y"

    int valor_int;
    char* texto;
    struct NodoAST* nodo;

#line 121 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
