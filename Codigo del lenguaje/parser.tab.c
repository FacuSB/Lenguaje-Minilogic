/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interprete.h"

extern int yylex();
void yyerror(const char* s);

NodoAST* raiz_ast = NULL;

/* --- FUNCIONES AUXILIARES PARA EL AST --- */
NodoAST* crear_nodo(TipoNodo tipo, NodoAST* h0, NodoAST* h1, NodoAST* h2, NodoAST* h3) {
    NodoAST* n = (NodoAST*)malloc(sizeof(NodoAST));
    if(!n) { fprintf(stderr, "Error: Sin memoria para el AST.\n"); exit(1); }
    n->tipo = tipo;
    n->hijos[0] = h0;
    n->hijos[1] = h1;
    n->hijos[2] = h2;
    n->hijos[3] = h3;
    n->valor_int = 0;
    n->valor_texto = NULL;
    n->operador = NULL;
    n->siguiente = NULL;
    return n;
}

NodoAST* crear_nodo_literal(int val) {
    NodoAST* n = crear_nodo(N_LITERAL, NULL, NULL, NULL, NULL);
    n->valor_int = val;
    return n;
}

NodoAST* crear_nodo_literal_bool(int val) {
    NodoAST* n = crear_nodo(N_LITERAL_BOOL, NULL, NULL, NULL, NULL);
    n->valor_int = val;
    return n;
}

NodoAST* crear_nodo_literal_char(const char* val) {
    NodoAST* n = crear_nodo(N_LITERAL_CHAR, NULL, NULL, NULL, NULL);
    n->valor_texto = strdup(val);
    return n;
}

NodoAST* crear_nodo_literal_string(const char* val) {
    NodoAST* n = crear_nodo(N_LITERAL_STRING, NULL, NULL, NULL, NULL);
    n->valor_texto = strdup(val);
    return n;
}

NodoAST* crear_nodo_identificador(const char* id) {
    NodoAST* n = crear_nodo(N_IDENTIFICADOR, NULL, NULL, NULL, NULL);
    n->valor_texto = strdup(id);
    return n;
}

NodoAST* crear_nodo_binario(const char* op, NodoAST* h0, NodoAST* h1) {
    NodoAST* n = crear_nodo(N_EXPRESION_BINARIA, h0, h1, NULL, NULL);
    n->operador = strdup(op);
    return n;
}

NodoAST* crear_nodo_unario(const char* op, NodoAST* h0) {
    NodoAST* n = crear_nodo(N_EXPRESION_UNARIA, h0, NULL, NULL, NULL);
    n->operador = strdup(op);
    return n;
}

NodoAST* crear_nodo_asignacion(const char* id, NodoAST* expr) {
    NodoAST* n = crear_nodo(N_ASIGNACION, expr, NULL, NULL, NULL);
    n->valor_texto = strdup(id);
    return n;
}

NodoAST* crear_nodo_asignacion_array(const char* id, NodoAST* idx, NodoAST* expr) {
    NodoAST* n = crear_nodo(N_ASIGNACION_ARRAY, idx, expr, NULL, NULL);
    n->valor_texto = strdup(id);
    return n;
}

NodoAST* crear_nodo_draw(NodoAST* x, NodoAST* y, const char* color) {
    NodoAST* n = crear_nodo(N_DRAW, x, y, NULL, NULL);
    n->valor_texto = strdup(color);
    return n;
}

NodoAST* enlazar_sentencias(NodoAST* lista, NodoAST* nueva) {
    if (!lista) return nueva;
    if (!nueva) return lista;
    NodoAST* curr = lista;
    while (curr->siguiente != NULL) {
        curr = curr->siguiente;
    }
    curr->siguiente = nueva;
    return lista;
}

void registrar_variable(const char* nombre, int es_array, int tamano, TipoDato tipo) {
    if (!scope_actual) {
        scope_actual = (Scope*)malloc(sizeof(Scope));
        scope_actual->lista_simbolos = NULL;
        scope_actual->padre = NULL;
    }
    Simbolo* sym = (Simbolo*)malloc(sizeof(Simbolo));
    sym->nombre = strdup(nombre);
    sym->es_array = es_array;
    sym->tamano_array = tamano;
    sym->siguiente = scope_actual->lista_simbolos;
    if (es_array) {
        sym->celdas = (Val*)calloc(tamano, sizeof(Val));
        for (int i = 0; i < tamano; i++) {
            sym->celdas[i].tipo = tipo;
            sym->celdas[i].v_int = 0;
            sym->celdas[i].v_str = NULL;
        }
    } else {
        sym->valor.tipo = tipo;
        sym->valor.v_int = 0;
        sym->valor.v_str = NULL;
        sym->celdas = NULL;
    }
    scope_actual->lista_simbolos = sym;
}

#line 197 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOKEN_VOID = 3,                 /* TOKEN_VOID  */
  YYSYMBOL_TOKEN_MAIN = 4,                 /* TOKEN_MAIN  */
  YYSYMBOL_TOKEN_FUNCTION = 5,             /* TOKEN_FUNCTION  */
  YYSYMBOL_TOKEN_INT = 6,                  /* TOKEN_INT  */
  YYSYMBOL_TOKEN_CHAR = 7,                 /* TOKEN_CHAR  */
  YYSYMBOL_TOKEN_STRING_TYPE = 8,          /* TOKEN_STRING_TYPE  */
  YYSYMBOL_TOKEN_BOOL = 9,                 /* TOKEN_BOOL  */
  YYSYMBOL_TOKEN_ARRAY = 10,               /* TOKEN_ARRAY  */
  YYSYMBOL_TOKEN_IF = 11,                  /* TOKEN_IF  */
  YYSYMBOL_TOKEN_ELSE = 12,                /* TOKEN_ELSE  */
  YYSYMBOL_TOKEN_FOR = 13,                 /* TOKEN_FOR  */
  YYSYMBOL_TOKEN_WHILE = 14,               /* TOKEN_WHILE  */
  YYSYMBOL_TOKEN_RETURN = 15,              /* TOKEN_RETURN  */
  YYSYMBOL_TOKEN_INPUT = 16,               /* TOKEN_INPUT  */
  YYSYMBOL_TOKEN_PRINT = 17,               /* TOKEN_PRINT  */
  YYSYMBOL_TOKEN_DRAW = 18,                /* TOKEN_DRAW  */
  YYSYMBOL_TOKEN_TIME = 19,                /* TOKEN_TIME  */
  YYSYMBOL_TOKEN_OPEN_WINDOW = 20,         /* TOKEN_OPEN_WINDOW  */
  YYSYMBOL_TOKEN_IS_KEY_PRESSED = 21,      /* TOKEN_IS_KEY_PRESSED  */
  YYSYMBOL_TOKEN_TRUE = 22,                /* TOKEN_TRUE  */
  YYSYMBOL_TOKEN_FALSE = 23,               /* TOKEN_FALSE  */
  YYSYMBOL_TOKEN_PAR_A = 24,               /* TOKEN_PAR_A  */
  YYSYMBOL_TOKEN_PAR_C = 25,               /* TOKEN_PAR_C  */
  YYSYMBOL_TOKEN_CORCH_A = 26,             /* TOKEN_CORCH_A  */
  YYSYMBOL_TOKEN_CORCH_C = 27,             /* TOKEN_CORCH_C  */
  YYSYMBOL_TOKEN_LLAVE_A = 28,             /* TOKEN_LLAVE_A  */
  YYSYMBOL_TOKEN_LLAVE_C = 29,             /* TOKEN_LLAVE_C  */
  YYSYMBOL_TOKEN_PUNTOYCOMA = 30,          /* TOKEN_PUNTOYCOMA  */
  YYSYMBOL_TOKEN_COMA = 31,                /* TOKEN_COMA  */
  YYSYMBOL_TOKEN_LIT_INT = 32,             /* TOKEN_LIT_INT  */
  YYSYMBOL_TOKEN_ID = 33,                  /* TOKEN_ID  */
  YYSYMBOL_TOKEN_HEX_COLOR = 34,           /* TOKEN_HEX_COLOR  */
  YYSYMBOL_TOKEN_LIT_CHAR = 35,            /* TOKEN_LIT_CHAR  */
  YYSYMBOL_TOKEN_LIT_STRING = 36,          /* TOKEN_LIT_STRING  */
  YYSYMBOL_TOKEN_OP_ASIG = 37,             /* TOKEN_OP_ASIG  */
  YYSYMBOL_TOKEN_OP_OR = 38,               /* TOKEN_OP_OR  */
  YYSYMBOL_TOKEN_OP_AND = 39,              /* TOKEN_OP_AND  */
  YYSYMBOL_TOKEN_OP_IGUAL = 40,            /* TOKEN_OP_IGUAL  */
  YYSYMBOL_TOKEN_OP_DISTINTO = 41,         /* TOKEN_OP_DISTINTO  */
  YYSYMBOL_TOKEN_OP_MENOR = 42,            /* TOKEN_OP_MENOR  */
  YYSYMBOL_TOKEN_OP_MAYOR = 43,            /* TOKEN_OP_MAYOR  */
  YYSYMBOL_TOKEN_OP_MENORIGUAL = 44,       /* TOKEN_OP_MENORIGUAL  */
  YYSYMBOL_TOKEN_OP_MAYORIGUAL = 45,       /* TOKEN_OP_MAYORIGUAL  */
  YYSYMBOL_TOKEN_OP_SUM = 46,              /* TOKEN_OP_SUM  */
  YYSYMBOL_TOKEN_OP_RES = 47,              /* TOKEN_OP_RES  */
  YYSYMBOL_TOKEN_OP_MUL = 48,              /* TOKEN_OP_MUL  */
  YYSYMBOL_TOKEN_OP_DIV = 49,              /* TOKEN_OP_DIV  */
  YYSYMBOL_TOKEN_OP_POT = 50,              /* TOKEN_OP_POT  */
  YYSYMBOL_TOKEN_OP_NOT = 51,              /* TOKEN_OP_NOT  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_programa = 53,                  /* programa  */
  YYSYMBOL_lista_sentencias = 54,          /* lista_sentencias  */
  YYSYMBOL_sentencias = 55,                /* sentencias  */
  YYSYMBOL_sentencia = 56,                 /* sentencia  */
  YYSYMBOL_asignacion_for = 57,            /* asignacion_for  */
  YYSYMBOL_expresion = 58                  /* expresion  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   388

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  7
/* YYNRULES -- Number of rules.  */
#define YYNRULES  47
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  154

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   162,   162,   168,   169,   173,   174,   178,   182,   186,
     190,   194,   198,   202,   208,   211,   214,   217,   220,   223,
     226,   229,   232,   238,   241,   248,   251,   254,   257,   260,
     263,   266,   269,   272,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOKEN_VOID",
  "TOKEN_MAIN", "TOKEN_FUNCTION", "TOKEN_INT", "TOKEN_CHAR",
  "TOKEN_STRING_TYPE", "TOKEN_BOOL", "TOKEN_ARRAY", "TOKEN_IF",
  "TOKEN_ELSE", "TOKEN_FOR", "TOKEN_WHILE", "TOKEN_RETURN", "TOKEN_INPUT",
  "TOKEN_PRINT", "TOKEN_DRAW", "TOKEN_TIME", "TOKEN_OPEN_WINDOW",
  "TOKEN_IS_KEY_PRESSED", "TOKEN_TRUE", "TOKEN_FALSE", "TOKEN_PAR_A",
  "TOKEN_PAR_C", "TOKEN_CORCH_A", "TOKEN_CORCH_C", "TOKEN_LLAVE_A",
  "TOKEN_LLAVE_C", "TOKEN_PUNTOYCOMA", "TOKEN_COMA", "TOKEN_LIT_INT",
  "TOKEN_ID", "TOKEN_HEX_COLOR", "TOKEN_LIT_CHAR", "TOKEN_LIT_STRING",
  "TOKEN_OP_ASIG", "TOKEN_OP_OR", "TOKEN_OP_AND", "TOKEN_OP_IGUAL",
  "TOKEN_OP_DISTINTO", "TOKEN_OP_MENOR", "TOKEN_OP_MAYOR",
  "TOKEN_OP_MENORIGUAL", "TOKEN_OP_MAYORIGUAL", "TOKEN_OP_SUM",
  "TOKEN_OP_RES", "TOKEN_OP_MUL", "TOKEN_OP_DIV", "TOKEN_OP_POT",
  "TOKEN_OP_NOT", "$accept", "programa", "lista_sentencias", "sentencias",
  "sentencia", "asignacion_for", "expresion", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-89)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       8,     9,    14,    -8,   -89,     0,   -11,    81,    -7,    -6,
      -4,    -3,    -1,    10,    11,    12,    13,    16,    18,    19,
     -19,     4,    81,   -89,   -18,    26,    31,    35,    44,    45,
      -5,    45,     2,    45,    45,    45,    45,    45,   -89,   -89,
      39,   -89,   -89,   -89,   -89,    40,    49,    50,   -89,   -89,
      45,   -89,    53,   -89,   -89,    45,    77,    42,    46,    52,
     102,    55,   114,   301,   320,   199,   242,    57,    73,    51,
      45,   139,    45,   -89,   110,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,   136,    45,    45,
     146,   142,   197,    45,    45,   191,   -89,   200,   201,   -89,
     151,   -89,   222,    81,    64,    88,   123,   123,   -26,   -26,
     -26,   -26,   -39,   -39,   -89,   -89,    45,   164,   262,    81,
     204,   -89,   339,   176,    45,   -89,   -89,   -89,   -89,   203,
     164,    -5,   205,   206,   216,   221,   282,   223,   208,   -89,
     -89,   227,   -89,   -89,   225,   226,   228,    81,    81,   -89,
     230,   244,   -89,   -89
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     4,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     3,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     5,
       0,     7,    10,    11,     9,     0,     0,     0,    26,    27,
       0,    25,    30,    28,    29,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,    32,
       0,    47,     0,     4,    45,    44,    38,    39,    40,    41,
      42,    43,    34,    35,    36,    37,     0,    23,     0,     4,
       0,    16,     0,     0,     0,     8,    12,    33,    31,     0,
      24,     0,     0,     0,     0,     0,     0,    17,     0,    19,
      13,     0,    21,    15,     0,     0,     0,     4,     4,    22,
       0,     0,    18,    20
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -89,   -89,   -88,   -89,   233,   125,   -31
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    21,    22,    23,    59,    56
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      60,    57,    62,    63,    64,    65,    66,    36,    40,    85,
      86,     1,    41,     3,     4,   129,     5,     7,    37,    71,
      83,    84,    85,    86,    73,     6,    24,    25,    58,    26,
      27,   132,    28,    38,    29,    30,    31,    32,    61,   100,
      33,   102,    34,    35,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    42,   117,   118,   150,
     151,    43,   122,   123,    46,    44,    47,    48,    49,    50,
      45,    67,    68,    69,    70,    87,    99,    51,    52,    72,
      53,    54,    89,    88,    97,   130,    91,     8,     9,    10,
      11,    12,    13,   136,    14,    15,    55,    16,    17,    18,
      98,    19,    74,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    20,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    90,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,   103,    92,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,   101,    79,    80,    81,    82,    83,
      84,    85,    86,   116,   119,   120,   127,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,   135,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    95,   121,   124,   133,
     125,   126,   137,   145,   139,   144,   140,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,   128,
     141,   142,   146,   147,   148,    39,   138,     0,   149,   152,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    96,   153,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,   131,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,   143,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    93,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    94,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     134,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86
};

static const yytype_int16 yycheck[] =
{
      31,     6,    33,    34,    35,    36,    37,    26,    26,    48,
      49,     3,    30,     4,     0,   103,    24,    28,    37,    50,
      46,    47,    48,    49,    55,    25,    33,    33,    33,    33,
      33,   119,    33,    29,    24,    24,    24,    24,    36,    70,
      24,    72,    24,    24,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    30,    88,    89,   147,
     148,    30,    93,    94,    19,    30,    21,    22,    23,    24,
      26,    32,    32,    24,    24,    33,    25,    32,    33,    26,
      35,    36,    30,    37,    27,   116,    31,     6,     7,     8,
       9,    10,    11,   124,    13,    14,    51,    16,    17,    18,
      27,    20,    25,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    33,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    25,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    28,    25,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    25,    42,    43,    44,    45,    46,
      47,    48,    49,    37,    28,    33,    25,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    25,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    27,    30,    37,    25,
      30,    30,    29,    25,    29,    12,    30,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    27,
      34,    30,    25,    28,    28,    22,   131,    -1,    30,    29,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    30,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    53,     4,     0,    24,    25,    28,     6,     7,
       8,     9,    10,    11,    13,    14,    16,    17,    18,    20,
      33,    54,    55,    56,    33,    33,    33,    33,    33,    24,
      24,    24,    24,    24,    24,    24,    26,    37,    29,    56,
      26,    30,    30,    30,    30,    26,    19,    21,    22,    23,
      24,    32,    33,    35,    36,    51,    58,     6,    33,    57,
      58,    36,    58,    58,    58,    58,    58,    32,    32,    24,
      24,    58,    26,    58,    25,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    33,    37,    30,
      25,    31,    25,    31,    31,    27,    30,    27,    27,    25,
      58,    25,    58,    28,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    37,    58,    58,    28,
      33,    30,    58,    58,    37,    30,    30,    25,    27,    54,
      58,    30,    54,    25,    31,    25,    58,    29,    57,    29,
      30,    34,    30,    30,    12,    25,    25,    28,    28,    30,
      54,    54,    29,    29
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    55,    55,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    57,    57,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     7,     1,     0,     2,     1,     3,     6,     3,
       3,     3,     6,     7,     4,     7,     5,     7,    11,     7,
      11,     7,     9,     3,     4,     1,     1,     1,     1,     1,
       1,     4,     3,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programa: TOKEN_VOID TOKEN_MAIN TOKEN_PAR_A TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C  */
#line 162 "parser.y"
                                                                                               {
        raiz_ast = crear_nodo(N_PROGRAMA, (yyvsp[-1].nodo), NULL, NULL, NULL);
    }
#line 1381 "parser.tab.c"
    break;

  case 3: /* lista_sentencias: sentencias  */
#line 168 "parser.y"
               { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1387 "parser.tab.c"
    break;

  case 4: /* lista_sentencias: %empty  */
#line 169 "parser.y"
                   { (yyval.nodo) = NULL; }
#line 1393 "parser.tab.c"
    break;

  case 5: /* sentencias: sentencias sentencia  */
#line 173 "parser.y"
                         { (yyval.nodo) = enlazar_sentencias((yyvsp[-1].nodo), (yyvsp[0].nodo)); }
#line 1399 "parser.tab.c"
    break;

  case 6: /* sentencias: sentencia  */
#line 174 "parser.y"
                { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1405 "parser.tab.c"
    break;

  case 7: /* sentencia: TOKEN_INT TOKEN_ID TOKEN_PUNTOYCOMA  */
#line 178 "parser.y"
                                        {
        registrar_variable((yyvsp[-1].texto), 0, 0, T_INT);
        (yyval.nodo) = NULL;
    }
#line 1414 "parser.tab.c"
    break;

  case 8: /* sentencia: TOKEN_INT TOKEN_ID TOKEN_CORCH_A TOKEN_LIT_INT TOKEN_CORCH_C TOKEN_PUNTOYCOMA  */
#line 182 "parser.y"
                                                                                    {
        registrar_variable((yyvsp[-4].texto), 1, (yyvsp[-2].valor_int), T_INT);
        (yyval.nodo) = NULL;
    }
#line 1423 "parser.tab.c"
    break;

  case 9: /* sentencia: TOKEN_BOOL TOKEN_ID TOKEN_PUNTOYCOMA  */
#line 186 "parser.y"
                                           {
        registrar_variable((yyvsp[-1].texto), 0, 0, T_BOOL);
        (yyval.nodo) = NULL;
    }
#line 1432 "parser.tab.c"
    break;

  case 10: /* sentencia: TOKEN_CHAR TOKEN_ID TOKEN_PUNTOYCOMA  */
#line 190 "parser.y"
                                           {
        registrar_variable((yyvsp[-1].texto), 0, 0, T_CHAR);
        (yyval.nodo) = NULL;
    }
#line 1441 "parser.tab.c"
    break;

  case 11: /* sentencia: TOKEN_STRING_TYPE TOKEN_ID TOKEN_PUNTOYCOMA  */
#line 194 "parser.y"
                                                  {
        registrar_variable((yyvsp[-1].texto), 0, 0, T_STRING);
        (yyval.nodo) = NULL;
    }
#line 1450 "parser.tab.c"
    break;

  case 12: /* sentencia: TOKEN_ARRAY TOKEN_ID TOKEN_CORCH_A TOKEN_LIT_INT TOKEN_CORCH_C TOKEN_PUNTOYCOMA  */
#line 198 "parser.y"
                                                                                      {
        registrar_variable((yyvsp[-4].texto), 1, (yyvsp[-2].valor_int), T_INT);
        (yyval.nodo) = NULL;
    }
#line 1459 "parser.tab.c"
    break;

  case 13: /* sentencia: TOKEN_INPUT TOKEN_PAR_A TOKEN_LIT_STRING TOKEN_COMA TOKEN_ID TOKEN_PAR_C TOKEN_PUNTOYCOMA  */
#line 202 "parser.y"
                                                                                                {
        NodoAST* n = crear_nodo(N_INPUT, NULL, NULL, NULL, NULL);
        n->valor_texto = strdup((yyvsp[-4].texto));
        n->operador = strdup((yyvsp[-2].texto));
        (yyval.nodo) = n;
    }
#line 1470 "parser.tab.c"
    break;

  case 14: /* sentencia: TOKEN_ID TOKEN_OP_ASIG expresion TOKEN_PUNTOYCOMA  */
#line 208 "parser.y"
                                                        {
        (yyval.nodo) = crear_nodo_asignacion((yyvsp[-3].texto), (yyvsp[-1].nodo));
    }
#line 1478 "parser.tab.c"
    break;

  case 15: /* sentencia: TOKEN_ID TOKEN_CORCH_A expresion TOKEN_CORCH_C TOKEN_OP_ASIG expresion TOKEN_PUNTOYCOMA  */
#line 211 "parser.y"
                                                                                              {
        (yyval.nodo) = crear_nodo_asignacion_array((yyvsp[-6].texto), (yyvsp[-4].nodo), (yyvsp[-1].nodo));
    }
#line 1486 "parser.tab.c"
    break;

  case 16: /* sentencia: TOKEN_PRINT TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_PUNTOYCOMA  */
#line 214 "parser.y"
                                                                     {
        (yyval.nodo) = crear_nodo(N_PRINT, (yyvsp[-2].nodo), NULL, NULL, NULL);
    }
#line 1494 "parser.tab.c"
    break;

  case 17: /* sentencia: TOKEN_IF TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C  */
#line 217 "parser.y"
                                                                                              {
        (yyval.nodo) = crear_nodo(N_IF, (yyvsp[-4].nodo), (yyvsp[-1].nodo), NULL, NULL);
    }
#line 1502 "parser.tab.c"
    break;

  case 18: /* sentencia: TOKEN_IF TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C TOKEN_ELSE TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C  */
#line 220 "parser.y"
                                                                                                                                                      {
        (yyval.nodo) = crear_nodo(N_IF, (yyvsp[-8].nodo), (yyvsp[-5].nodo), (yyvsp[-1].nodo), NULL);
    }
#line 1510 "parser.tab.c"
    break;

  case 19: /* sentencia: TOKEN_WHILE TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C  */
#line 223 "parser.y"
                                                                                                 {
        (yyval.nodo) = crear_nodo(N_WHILE, (yyvsp[-4].nodo), (yyvsp[-1].nodo), NULL, NULL);
    }
#line 1518 "parser.tab.c"
    break;

  case 20: /* sentencia: TOKEN_FOR TOKEN_PAR_A asignacion_for TOKEN_PUNTOYCOMA expresion TOKEN_PUNTOYCOMA asignacion_for TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C  */
#line 226 "parser.y"
                                                                                                                                                               {
        (yyval.nodo) = crear_nodo(N_FOR, (yyvsp[-8].nodo), (yyvsp[-6].nodo), (yyvsp[-4].nodo), (yyvsp[-1].nodo));
    }
#line 1526 "parser.tab.c"
    break;

  case 21: /* sentencia: TOKEN_OPEN_WINDOW TOKEN_PAR_A expresion TOKEN_COMA expresion TOKEN_PAR_C TOKEN_PUNTOYCOMA  */
#line 229 "parser.y"
                                                                                                {
        (yyval.nodo) = crear_nodo(N_OPEN_WINDOW, (yyvsp[-4].nodo), (yyvsp[-2].nodo), NULL, NULL);
    }
#line 1534 "parser.tab.c"
    break;

  case 22: /* sentencia: TOKEN_DRAW TOKEN_PAR_A expresion TOKEN_COMA expresion TOKEN_COMA TOKEN_HEX_COLOR TOKEN_PAR_C TOKEN_PUNTOYCOMA  */
#line 232 "parser.y"
                                                                                                                    {
        (yyval.nodo) = crear_nodo_draw((yyvsp[-6].nodo), (yyvsp[-4].nodo), (yyvsp[-2].texto));
    }
#line 1542 "parser.tab.c"
    break;

  case 23: /* asignacion_for: TOKEN_ID TOKEN_OP_ASIG expresion  */
#line 238 "parser.y"
                                     {
        (yyval.nodo) = crear_nodo_asignacion((yyvsp[-2].texto), (yyvsp[0].nodo));
    }
#line 1550 "parser.tab.c"
    break;

  case 24: /* asignacion_for: TOKEN_INT TOKEN_ID TOKEN_OP_ASIG expresion  */
#line 241 "parser.y"
                                                 {
        registrar_variable((yyvsp[-2].texto), 0, 0, T_INT);
        (yyval.nodo) = crear_nodo_asignacion((yyvsp[-2].texto), (yyvsp[0].nodo));
    }
#line 1559 "parser.tab.c"
    break;

  case 25: /* expresion: TOKEN_LIT_INT  */
#line 248 "parser.y"
                  { 
        (yyval.nodo) = crear_nodo_literal((yyvsp[0].valor_int)); 
    }
#line 1567 "parser.tab.c"
    break;

  case 26: /* expresion: TOKEN_TRUE  */
#line 251 "parser.y"
                 {
        (yyval.nodo) = crear_nodo_literal_bool(1);
    }
#line 1575 "parser.tab.c"
    break;

  case 27: /* expresion: TOKEN_FALSE  */
#line 254 "parser.y"
                  {
        (yyval.nodo) = crear_nodo_literal_bool(0);
    }
#line 1583 "parser.tab.c"
    break;

  case 28: /* expresion: TOKEN_LIT_CHAR  */
#line 257 "parser.y"
                     {
        (yyval.nodo) = crear_nodo_literal_char((yyvsp[0].texto));
    }
#line 1591 "parser.tab.c"
    break;

  case 29: /* expresion: TOKEN_LIT_STRING  */
#line 260 "parser.y"
                       {
        (yyval.nodo) = crear_nodo_literal_string((yyvsp[0].texto));
    }
#line 1599 "parser.tab.c"
    break;

  case 30: /* expresion: TOKEN_ID  */
#line 263 "parser.y"
               { 
        (yyval.nodo) = crear_nodo_identificador((yyvsp[0].texto)); 
    }
#line 1607 "parser.tab.c"
    break;

  case 31: /* expresion: TOKEN_ID TOKEN_CORCH_A expresion TOKEN_CORCH_C  */
#line 266 "parser.y"
                                                     { 
        (yyval.nodo) = crear_nodo_binario("[]", crear_nodo_identificador((yyvsp[-3].texto)), (yyvsp[-1].nodo)); 
    }
#line 1615 "parser.tab.c"
    break;

  case 32: /* expresion: TOKEN_TIME TOKEN_PAR_A TOKEN_PAR_C  */
#line 269 "parser.y"
                                         { 
        (yyval.nodo) = crear_nodo(N_TIME, NULL, NULL, NULL, NULL); 
    }
#line 1623 "parser.tab.c"
    break;

  case 33: /* expresion: TOKEN_IS_KEY_PRESSED TOKEN_PAR_A expresion TOKEN_PAR_C  */
#line 272 "parser.y"
                                                             { 
        (yyval.nodo) = crear_nodo(N_IS_KEY_PRESSED, (yyvsp[-1].nodo), NULL, NULL, NULL); 
    }
#line 1631 "parser.tab.c"
    break;

  case 34: /* expresion: expresion TOKEN_OP_SUM expresion  */
#line 275 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("+", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1637 "parser.tab.c"
    break;

  case 35: /* expresion: expresion TOKEN_OP_RES expresion  */
#line 276 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("-", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1643 "parser.tab.c"
    break;

  case 36: /* expresion: expresion TOKEN_OP_MUL expresion  */
#line 277 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("*", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1649 "parser.tab.c"
    break;

  case 37: /* expresion: expresion TOKEN_OP_DIV expresion  */
#line 278 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("/", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1655 "parser.tab.c"
    break;

  case 38: /* expresion: expresion TOKEN_OP_IGUAL expresion  */
#line 279 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("==", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1661 "parser.tab.c"
    break;

  case 39: /* expresion: expresion TOKEN_OP_DISTINTO expresion  */
#line 280 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("!=", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1667 "parser.tab.c"
    break;

  case 40: /* expresion: expresion TOKEN_OP_MENOR expresion  */
#line 281 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("<", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1673 "parser.tab.c"
    break;

  case 41: /* expresion: expresion TOKEN_OP_MAYOR expresion  */
#line 282 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario(">", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1679 "parser.tab.c"
    break;

  case 42: /* expresion: expresion TOKEN_OP_MENORIGUAL expresion  */
#line 283 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("<=", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1685 "parser.tab.c"
    break;

  case 43: /* expresion: expresion TOKEN_OP_MAYORIGUAL expresion  */
#line 284 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario(">=", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1691 "parser.tab.c"
    break;

  case 44: /* expresion: expresion TOKEN_OP_AND expresion  */
#line 285 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("&&", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1697 "parser.tab.c"
    break;

  case 45: /* expresion: expresion TOKEN_OP_OR expresion  */
#line 286 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_binario("||", (yyvsp[-2].nodo), (yyvsp[0].nodo)); }
#line 1703 "parser.tab.c"
    break;

  case 46: /* expresion: TOKEN_OP_NOT expresion  */
#line 287 "parser.y"
                                                 { (yyval.nodo) = crear_nodo_unario("!", (yyvsp[0].nodo)); }
#line 1709 "parser.tab.c"
    break;

  case 47: /* expresion: TOKEN_PAR_A expresion TOKEN_PAR_C  */
#line 288 "parser.y"
                                                 { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 1715 "parser.tab.c"
    break;


#line 1719 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 291 "parser.y"


void yyerror(const char* s) {
    extern int yylineno;
    fprintf(stderr, "[ERROR SINTÁCTICO - Línea %d]: %s\n", yylineno, s);
}
