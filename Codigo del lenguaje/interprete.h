#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Enum de nodos del AST perfectamente ordenados */
typedef enum {
    N_PROGRAMA, 
    N_ASIGNACION, 
    N_ASIGNACION_ARRAY, 
    N_PRINT, 
    N_IF, 
    N_WHILE, 
    N_FOR,
    N_LITERAL, 
    N_IDENTIFICADOR, 
    N_EXPRESION_BINARIA, 
    N_EXPRESION_UNARIA,
    N_OPEN_WINDOW, 
    N_DRAW, 
    N_TIME,
    N_IS_KEY_PRESSED,
    N_LITERAL_BOOL,
    N_LITERAL_CHAR,
    N_LITERAL_STRING,
    N_INPUT
} TipoNodo;

/* Enum de Tipos de Datos */
typedef enum {
    T_INT,
    T_BOOL,
    T_CHAR,
    T_STRING
} TipoDato;

/* Definición del tipo contenedor Val */
typedef struct {
    TipoDato tipo;
    int v_int;
    char* v_str;
} Val;

/* Estructura de la Tabla de Símbolos */
typedef struct Simbolo {
    char* nombre;
    int es_array;
    int tamano_array;
    Val valor;
    Val* celdas;
    struct Simbolo* siguiente;
} Simbolo;

/* Definición de Ámbitos (Scopes) */
typedef struct Scope {
    Simbolo* lista_simbolos;
    struct Scope* padre;
} Scope;

extern Scope* scope_actual;

/* Estructura del Nodo del Árbol de Sintaxis Abstracta (AST) */
typedef struct NodoAST {
    TipoNodo tipo;
    struct NodoAST* hijos[4];
    int valor_int;
    char* valor_texto;
    char* operador;
    struct NodoAST* siguiente;
} NodoAST;

extern NodoAST* raiz_ast;

/* Prototipos de funciones del motor */
void evaluar_AST(NodoAST* nodo);
Val evaluar_expresion(NodoAST* nodo);

#endif