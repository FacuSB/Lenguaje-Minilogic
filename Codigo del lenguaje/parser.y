%{
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
%}

%union {
    int valor_int;
    char* texto;
    struct NodoAST* nodo;
}

/* Mapeo de Palabras Clave y Tipos */
%token TOKEN_VOID TOKEN_MAIN TOKEN_FUNCTION TOKEN_INT TOKEN_CHAR TOKEN_STRING_TYPE TOKEN_BOOL TOKEN_ARRAY
%token TOKEN_IF TOKEN_ELSE TOKEN_FOR TOKEN_WHILE TOKEN_RETURN
%token TOKEN_INPUT TOKEN_PRINT TOKEN_DRAW TOKEN_TIME TOKEN_OPEN_WINDOW TOKEN_IS_KEY_PRESSED
%token TOKEN_TRUE TOKEN_FALSE

/* Signos y Símbolos */
%token TOKEN_PAR_A TOKEN_PAR_C TOKEN_CORCH_A TOKEN_CORCH_C TOKEN_LLAVE_A TOKEN_LLAVE_C TOKEN_PUNTOYCOMA TOKEN_COMA

/* Valores con contenido */
%token <valor_int> TOKEN_LIT_INT
%token <texto> TOKEN_ID TOKEN_HEX_COLOR TOKEN_LIT_CHAR TOKEN_LIT_STRING

/* Operadores y Precedencias (Bajas a Altas) */
%token TOKEN_OP_ASIG
%left TOKEN_OP_OR
%left TOKEN_OP_AND
%left TOKEN_OP_IGUAL TOKEN_OP_DISTINTO
%left TOKEN_OP_MENOR TOKEN_OP_MAYOR TOKEN_OP_MENORIGUAL TOKEN_OP_MAYORIGUAL
%left TOKEN_OP_SUM TOKEN_OP_RES
%left TOKEN_OP_MUL TOKEN_OP_DIV
%right TOKEN_OP_POT
%right TOKEN_OP_NOT

%type <nodo> programa lista_sentencias sentencias sentencia expresion asignacion_for

%%

programa:
    TOKEN_VOID TOKEN_MAIN TOKEN_PAR_A TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C {
        raiz_ast = crear_nodo(N_PROGRAMA, $6, NULL, NULL, NULL);
    }
    ;

lista_sentencias:
    sentencias { $$ = $1; }
    | /* vacío */ { $$ = NULL; }
    ;

sentencias:
    sentencias sentencia { $$ = enlazar_sentencias($1, $2); }
    | sentencia { $$ = $1; }
    ;

sentencia:
    TOKEN_INT TOKEN_ID TOKEN_PUNTOYCOMA {
        registrar_variable($2, 0, 0, T_INT);
        $$ = NULL;
    }
    | TOKEN_INT TOKEN_ID TOKEN_CORCH_A TOKEN_LIT_INT TOKEN_CORCH_C TOKEN_PUNTOYCOMA {
        registrar_variable($2, 1, $4, T_INT);
        $$ = NULL;
    }
    | TOKEN_BOOL TOKEN_ID TOKEN_PUNTOYCOMA {
        registrar_variable($2, 0, 0, T_BOOL);
        $$ = NULL;
    }
    | TOKEN_CHAR TOKEN_ID TOKEN_PUNTOYCOMA {
        registrar_variable($2, 0, 0, T_CHAR);
        $$ = NULL;
    }
    | TOKEN_STRING_TYPE TOKEN_ID TOKEN_PUNTOYCOMA {
        registrar_variable($2, 0, 0, T_STRING);
        $$ = NULL;
    }
    | TOKEN_ARRAY TOKEN_ID TOKEN_CORCH_A TOKEN_LIT_INT TOKEN_CORCH_C TOKEN_PUNTOYCOMA {
        registrar_variable($2, 1, $4, T_INT);
        $$ = NULL;
    }
    | TOKEN_INPUT TOKEN_PAR_A TOKEN_LIT_STRING TOKEN_COMA TOKEN_ID TOKEN_PAR_C TOKEN_PUNTOYCOMA {
        NodoAST* n = crear_nodo(N_INPUT, NULL, NULL, NULL, NULL);
        n->valor_texto = strdup($3);
        n->operador = strdup($5);
        $$ = n;
    }
    | TOKEN_ID TOKEN_OP_ASIG expresion TOKEN_PUNTOYCOMA {
        $$ = crear_nodo_asignacion($1, $3);
    }
    | TOKEN_ID TOKEN_CORCH_A expresion TOKEN_CORCH_C TOKEN_OP_ASIG expresion TOKEN_PUNTOYCOMA {
        $$ = crear_nodo_asignacion_array($1, $3, $6);
    }
    | TOKEN_PRINT TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_PUNTOYCOMA {
        $$ = crear_nodo(N_PRINT, $3, NULL, NULL, NULL);
    }
    | TOKEN_IF TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C {
        $$ = crear_nodo(N_IF, $3, $6, NULL, NULL);
    }
    | TOKEN_IF TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C TOKEN_ELSE TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C {
        $$ = crear_nodo(N_IF, $3, $6, $10, NULL);
    }
    | TOKEN_WHILE TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C {
        $$ = crear_nodo(N_WHILE, $3, $6, NULL, NULL);
    }
    | TOKEN_FOR TOKEN_PAR_A asignacion_for TOKEN_PUNTOYCOMA expresion TOKEN_PUNTOYCOMA asignacion_for TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C {
        $$ = crear_nodo(N_FOR, $3, $5, $7, $10);
    }
    | TOKEN_OPEN_WINDOW TOKEN_PAR_A expresion TOKEN_COMA expresion TOKEN_PAR_C TOKEN_PUNTOYCOMA {
        $$ = crear_nodo(N_OPEN_WINDOW, $3, $5, NULL, NULL);
    }
    | TOKEN_DRAW TOKEN_PAR_A expresion TOKEN_COMA expresion TOKEN_COMA TOKEN_HEX_COLOR TOKEN_PAR_C TOKEN_PUNTOYCOMA {
        $$ = crear_nodo_draw($3, $5, $7);
    }
    ;

asignacion_for:
    TOKEN_ID TOKEN_OP_ASIG expresion {
        $$ = crear_nodo_asignacion($1, $3);
    }
    | TOKEN_INT TOKEN_ID TOKEN_OP_ASIG expresion {
        registrar_variable($2, 0, 0, T_INT);
        $$ = crear_nodo_asignacion($2, $4);
    }
    ;

expresion:
    TOKEN_LIT_INT { 
        $$ = crear_nodo_literal($1); 
    }
    | TOKEN_TRUE {
        $$ = crear_nodo_literal_bool(1);
    }
    | TOKEN_FALSE {
        $$ = crear_nodo_literal_bool(0);
    }
    | TOKEN_LIT_CHAR {
        $$ = crear_nodo_literal_char($1);
    }
    | TOKEN_LIT_STRING {
        $$ = crear_nodo_literal_string($1);
    }
    | TOKEN_ID { 
        $$ = crear_nodo_identificador($1); 
    }
    | TOKEN_ID TOKEN_CORCH_A expresion TOKEN_CORCH_C { 
        $$ = crear_nodo_binario("[]", crear_nodo_identificador($1), $3); 
    }
    | TOKEN_TIME TOKEN_PAR_A TOKEN_PAR_C { 
        $$ = crear_nodo(N_TIME, NULL, NULL, NULL, NULL); 
    }
    | TOKEN_IS_KEY_PRESSED TOKEN_PAR_A expresion TOKEN_PAR_C { 
        $$ = crear_nodo(N_IS_KEY_PRESSED, $3, NULL, NULL, NULL); 
    }
    | expresion TOKEN_OP_SUM expresion           { $$ = crear_nodo_binario("+", $1, $3); }
    | expresion TOKEN_OP_RES expresion           { $$ = crear_nodo_binario("-", $1, $3); }
    | expresion TOKEN_OP_MUL expresion           { $$ = crear_nodo_binario("*", $1, $3); }
    | expresion TOKEN_OP_DIV expresion           { $$ = crear_nodo_binario("/", $1, $3); }
    | expresion TOKEN_OP_IGUAL expresion         { $$ = crear_nodo_binario("==", $1, $3); }
    | expresion TOKEN_OP_DISTINTO expresion      { $$ = crear_nodo_binario("!=", $1, $3); }
    | expresion TOKEN_OP_MENOR expresion         { $$ = crear_nodo_binario("<", $1, $3); }
    | expresion TOKEN_OP_MAYOR expresion         { $$ = crear_nodo_binario(">", $1, $3); }
    | expresion TOKEN_OP_MENORIGUAL expresion    { $$ = crear_nodo_binario("<=", $1, $3); }
    | expresion TOKEN_OP_MAYORIGUAL expresion    { $$ = crear_nodo_binario(">=", $1, $3); }
    | expresion TOKEN_OP_AND expresion           { $$ = crear_nodo_binario("&&", $1, $3); }
    | expresion TOKEN_OP_OR expresion            { $$ = crear_nodo_binario("||", $1, $3); }
    | TOKEN_OP_NOT expresion                     { $$ = crear_nodo_unario("!", $2); }
    | TOKEN_PAR_A expresion TOKEN_PAR_C          { $$ = $2; }
    ;

%%

void yyerror(const char* s) {
    extern int yylineno;
    fprintf(stderr, "[ERROR SINTÁCTICO - Línea %d]: %s\n", yylineno, s);
}