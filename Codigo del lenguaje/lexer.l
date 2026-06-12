%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Este header es generado automáticamente por Bison (-d) y 
   contiene las definiciones numéricas de todos los tokens. */
#include "parser.tab.h" 

%}

/* Opción para activar el contador de líneas automático que usa yyerror */
%option yylineno
/* Desactiva la necesidad de linkear con -lfl o implementar yywrap */
%option noyywrap

/* Expresiones regulares básicas */
LETRA           [a-zA-Z_]
DIGITO          [0-9]
IDENTIFICADOR   {LETRA}({LETRA}|{DIGITO})*
ENTERO          -?{DIGITO}+
CARACTER        '[^'\n]'
STRING          \"[^\"\n]*\"

/* Definición de estado exclusivo para comentarios multilínea */
%x COMENTARIO_MULTI

%%

[ \t\r]+                    { /* Ignorar espacios en blanco, tabulaciones y retornos de carro */ }
\n                          { /* Ignorar saltos de línea (yylineno se incrementa solo) */ }

"//".* { /* Ignorar comentarios de una sola línea */ }

"/*"                        { BEGIN(COMENTARIO_MULTI); }
<COMENTARIO_MULTI>"*/"      { BEGIN(INITIAL); }
<COMENTARIO_MULTI>\n        { /* Ignorar saltos de línea dentro del bloque de comentario */ }
<COMENTARIO_MULTI>.         { /* Ignorar caracteres internos del comentario */ }


"void"                      { return TOKEN_VOID; }
"Void"                      { return TOKEN_VOID; }
"VOID"                      { return TOKEN_VOID; }
"main"                      { return TOKEN_MAIN; }
"Main"                      { return TOKEN_MAIN; }
"MAIN"                      { return TOKEN_MAIN; }
"function"                  { return TOKEN_FUNCTION; }
"Function"                  { return TOKEN_FUNCTION; }
"FUNCTION"                  { return TOKEN_FUNCTION; }

"int"                       { return TOKEN_INT; }
"Int"                       { return TOKEN_INT; }
"char"                      { return TOKEN_CHAR; }
"Char"                      { return TOKEN_CHAR; }
"string"                    { return TOKEN_STRING_TYPE; }
"String"                    { return TOKEN_STRING_TYPE; }
"bool"                      { return TOKEN_BOOL; }
"Bool"                      { return TOKEN_BOOL; }
"array"                     { return TOKEN_ARRAY; }
"Array"                     { return TOKEN_ARRAY; }

"if"                        { return TOKEN_IF; }
"If"                        { return TOKEN_IF; }
"else"                      { return TOKEN_ELSE; }
"Else"                      { return TOKEN_ELSE; }
"for"                       { return TOKEN_FOR; }
"For"                       { return TOKEN_FOR; }
"while"                     { return TOKEN_WHILE; }
"While"                     { return TOKEN_WHILE; }
"return"                    { return TOKEN_RETURN; }
"Return"                    { return TOKEN_RETURN; }


"input"                     { return TOKEN_INPUT; }
"Input"                     { return TOKEN_INPUT; }
"print"                     { return TOKEN_PRINT; }
"Print"                     { return TOKEN_PRINT; }
"draw"                      { return TOKEN_DRAW; }
"Draw"                      { return TOKEN_DRAW; }
"DRAW"                      { return TOKEN_DRAW; }
"time"                      { return TOKEN_TIME; }
"Time"                      { return TOKEN_TIME; }
"TIME"                      { return TOKEN_TIME; }
"open_window"               { return TOKEN_OPEN_WINDOW; }
"Open_window"               { return TOKEN_OPEN_WINDOW; }
"Open_Window"               { return TOKEN_OPEN_WINDOW; }


"true"                      { yylval.valor_int = 1; return TOKEN_TRUE; }
"True"                      { yylval.valor_int = 1; return TOKEN_TRUE; }
"TRUE"                      { yylval.valor_int = 1; return TOKEN_TRUE; }
"false"                     { yylval.valor_int = 0; return TOKEN_FALSE; }
"False"                     { yylval.valor_int = 0; return TOKEN_FALSE; }
"FALSE"                     { yylval.valor_int = 0; return TOKEN_FALSE; }


"="                         { return TOKEN_OP_ASIG; }
"+"                         { return TOKEN_OP_SUM; }
"-"                         { return TOKEN_OP_RES; }
"*"                         { return TOKEN_OP_MUL; }
"/"                         { return TOKEN_OP_DIV; }
"^"                         { return TOKEN_OP_POT; }


">"                         { return TOKEN_OP_MAYOR; }
">="                        { return TOKEN_OP_MAYORIGUAL; }
"<"                         { return TOKEN_OP_MENOR; }
"<="                        { return TOKEN_OP_MENORIGUAL; }
"=="                        { return TOKEN_OP_IGUAL; }
"!="                        { return TOKEN_OP_DISTINTO; }


"&&"                        { return TOKEN_OP_AND; }
"and"                       { return TOKEN_OP_AND; }
"And"                       { return TOKEN_OP_AND; }
"AND"                       { return TOKEN_OP_AND; }
"||"                        { return TOKEN_OP_OR; }
"or"                        { return TOKEN_OP_OR; }
"Or"                        { return TOKEN_OP_OR; }
"OR"                        { return TOKEN_OP_OR; }
"!"                         { return TOKEN_OP_NOT; }
"not"                       { return TOKEN_OP_NOT; }
"Not"                       { return TOKEN_OP_NOT; }
"NOT"                       { return TOKEN_OP_NOT; }


"("                         { return TOKEN_PAR_A; }
")"                         { return TOKEN_PAR_C; }
"["                         { return TOKEN_CORCH_A; }
"]"                         { return TOKEN_CORCH_C; }
"{"                         { return TOKEN_LLAVE_A; }
"}"                         { return TOKEN_LLAVE_C; }
";"                         { return TOKEN_PUNTOYCOMA; }
","                         { return TOKEN_COMA; }


#[0-9a-fA-F]{3,8}           { yylval.texto = strdup(yytext); return TOKEN_HEX_COLOR; }

"IsKeyPressed"              { return TOKEN_IS_KEY_PRESSED; }


{IDENTIFICADOR}             { yylval.texto = strdup(yytext); return TOKEN_ID; }
{ENTERO}                    { yylval.valor_int = atoi(yytext); return TOKEN_LIT_INT; }
{CARACTER}                  { yylval.texto = strdup(yytext); return TOKEN_LIT_CHAR; }
{STRING}                    { yylval.texto = strdup(yytext); return TOKEN_LIT_STRING; }

.                           { 
                                printf("Error léxico en la línea %d: Carácter inesperado '%s'\n", yylineno, yytext); 
                            }

%%