# Reporte Técnico: Lenguaje MiniLogic

## 1. Objetivos del Trabajo

El desarrollo del motor e intérprete de MiniLogic busca cumplir los siguientes objetivos principales:

- **Ejecución de Lógica Imperativa:** Interpretar programas estructurados resolviendo de forma jerárquica operaciones matemáticas avanzadas (incluyendo potencia `^`), lógica booleana completa, y estructuras de control dinámicas (`if-else`, bucles `while` y `for`).
- **Soporte de Tipos y Arreglos Heterogéneos:** Permitir el almacenamiento dinámico y tipado en memoria de enteros, booleanos, caracteres individuales y cadenas de texto, incluyendo celdas de arreglos capaces de contener tipos mixtos en tiempo de ejecución.
- **Control de Entradas e Interacción con el Usuario:** Capturar la entrada del usuario por consola de forma segura mediante la sentencia `input`. Proveer salida formateada con `print`, limpiando delimitadores y traduciendo booleanos a su representación textual.
- **Entorno Gráfico e Interacción a Bajo Nivel (Windows API):** Facilitar la creación de lienzos gráficos utilizando llamadas nativas a Win32 GDI sin parpadeo visual (Double Buffering). Habilitar el dibujo de bloques de color (`draw`) e implementar la detección de teclado físico en tiempo real (`IsKeyPressed`) con control de rebote (debounce).
- **Demostración Didáctica de Compiladores:** Servir como una herramienta práctica e ilustrativa del flujo clásico de traducción: Análisis Léxico (Flex) → Análisis Sintáctico y construcción del AST (Bison) → Ejecución en memoria por recorrido del AST (Intérprete).

## 2. Descripción Sintáctica

La sintaxis de MiniLogic fue diseñada bajo una combinación de dos filosofías de diseño:

- **Estilo Python Amigable:** Se buscó una sintaxis extremadamente legible, limpia y natural. Para ello, se adoptaron palabras clave sencillas escritas de forma natural (como los operadores lógicos `and`, `or`, `not`, y los literales booleanos `true` y `false`), reduciendo el ruido visual característico de lenguajes como C++ o Java.
- **Llaves {} Explícitas para Bloques:** En lugar de depender de la indentación significativa (como hace Python puro), se optó por delimitar los bloques de código mediante llaves `{}` explícitas.

**Ventaja del Diseño:** Esta decisión simplifica drásticamente el analizador léxico (evitando la generación compleja de tokens `INDENT` y `DEDENT`) y permite un análisis sintáctico por parte de Bison que es significativamente más rápido, robusto y eficiente, facilitando además la recuperación frente a errores y la sincronización de sentencias en el compilador.

## 3. Gramática Formal (EBNF)

A continuación se presenta la especificación formal y completa de la gramática corregida de MiniLogic en notación EBNF (Extended Backus-Naur Form):

### Reglas de Producción SINTÁCTICAS (No Terminales)

```ebnf
(* Punto de entrada del programa *)
programa ::= TOKEN_VOID TOKEN_MAIN TOKEN_PAR_A TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C
lista_sentencias ::= sentencias | (* vacío *)
sentencias ::= sentencias sentencia | sentencia
sentencia ::= TOKEN_INT TOKEN_ID TOKEN_PUNTOYCOMA
            | TOKEN_INT TOKEN_ID TOKEN_CORCH_A TOKEN_LIT_INT TOKEN_CORCH_C TOKEN_PUNTOYCOMA
            | TOKEN_BOOL TOKEN_ID TOKEN_PUNTOYCOMA
            | TOKEN_CHAR TOKEN_ID TOKEN_PUNTOYCOMA
            | TOKEN_STRING_TYPE TOKEN_ID TOKEN_PUNTOYCOMA
            | TOKEN_ARRAY TOKEN_ID TOKEN_CORCH_A TOKEN_LIT_INT TOKEN_CORCH_C TOKEN_PUNTOYCOMA
            | TOKEN_INPUT TOKEN_PAR_A TOKEN_LIT_STRING TOKEN_COMA TOKEN_ID TOKEN_PAR_C TOKEN_PUNTOYCOMA
            | TOKEN_ID TOKEN_OP_ASIG expresion TOKEN_PUNTOYCOMA
            | TOKEN_ID TOKEN_CORCH_A expresion TOKEN_CORCH_C TOKEN_OP_ASIG expresion TOKEN_PUNTOYCOMA
            | TOKEN_PRINT TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_PUNTOYCOMA
            | TOKEN_IF TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C
            | TOKEN_IF TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C TOKEN_ELSE TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C
            | TOKEN_WHILE TOKEN_PAR_A expresion TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C
            | TOKEN_FOR TOKEN_PAR_A asignacion_for TOKEN_PUNTOYCOMA expresion TOKEN_PUNTOYCOMA asignacion_for TOKEN_PAR_C TOKEN_LLAVE_A lista_sentencias TOKEN_LLAVE_C
            | TOKEN_OPEN_WINDOW TOKEN_PAR_A expresion TOKEN_COMA expresion TOKEN_PAR_C TOKEN_PUNTOYCOMA
            | TOKEN_DRAW TOKEN_PAR_A expresion TOKEN_COMA expresion TOKEN_COMA TOKEN_HEX_COLOR TOKEN_PAR_C TOKEN_PUNTOYCOMA
asignacion_for ::= TOKEN_ID TOKEN_OP_ASIG expresion
                 | TOKEN_INT TOKEN_ID TOKEN_OP_ASIG expresion
expresion ::= TOKEN_LIT_INT
            | TOKEN_TRUE
            | TOKEN_FALSE
            | TOKEN_LIT_CHAR
            | TOKEN_LIT_STRING
            | TOKEN_ID
            | TOKEN_ID TOKEN_CORCH_A expresion TOKEN_CORCH_C
            | TOKEN_TIME TOKEN_PAR_A TOKEN_PAR_C
            | TOKEN_IS_KEY_PRESSED TOKEN_PAR_A expresion TOKEN_PAR_C
            | expresion TOKEN_OP_SUM expresion
            | expresion TOKEN_OP_RES expresion
            | expresion TOKEN_OP_MUL expresion
            | expresion TOKEN_OP_DIV expresion
            | expresion TOKEN_OP_IGUAL expresion
            | expresion TOKEN_OP_DISTINTO expresion
            | expresion TOKEN_OP_MENOR expresion
            | expresion TOKEN_OP_MAYOR expresion
            | expresion TOKEN_OP_MENORIGUAL expresion
            | expresion TOKEN_OP_MAYORIGUAL expresion
            | expresion TOKEN_OP_AND expresion
            | expresion TOKEN_OP_OR expresion
            | TOKEN_OP_NOT expresion
            | TOKEN_PAR_A expresion TOKEN_PAR_C
```

### Definiciones LÉXICAS (Terminales)

```ebnf
(* Palabras Clave (insensibles a mayúsculas/minúsculas) *)
TOKEN_VOID           ::= "void" | "Void" | "VOID"
TOKEN_MAIN           ::= "main" | "Main" | "MAIN"
TOKEN_INT            ::= "int" | "Int"
TOKEN_BOOL           ::= "bool" | "Bool"
TOKEN_CHAR           ::= "char" | "Char"
TOKEN_STRING_TYPE    ::= "string" | "String"
TOKEN_ARRAY          ::= "array" | "Array"
TOKEN_IF             ::= "if" | "If"
TOKEN_ELSE           ::= "else" | "Else"
TOKEN_FOR            ::= "for" | "For"
TOKEN_WHILE          ::= "while" | "While"
TOKEN_INPUT          ::= "input" | "Input"
TOKEN_PRINT          ::= "print" | "Print"
TOKEN_DRAW           ::= "draw" | "Draw" | "DRAW"
TOKEN_TIME           ::= "time" | "Time" | "TIME"
TOKEN_OPEN_WINDOW    ::= "open_window" | "Open_window" | "Open_Window"
TOKEN_IS_KEY_PRESSED ::= "IsKeyPressed"
TOKEN_TRUE           ::= "true" | "True" | "TRUE"
TOKEN_FALSE          ::= "false" | "False" | "FALSE"
(* Operadores *)
TOKEN_OP_ASIG        ::= "="
TOKEN_OP_SUM         ::= "+"
TOKEN_OP_RES         ::= "-"
TOKEN_OP_MUL         ::= "*"
TOKEN_OP_DIV         ::= "/"
TOKEN_OP_POT         ::= "^"
TOKEN_OP_IGUAL       ::= "=="
TOKEN_OP_DISTINTO    ::= "!="
TOKEN_OP_MENOR       ::= "<"
TOKEN_OP_MAYOR       ::= ">"
TOKEN_OP_MENORIGUAL  ::= "<="
TOKEN_OP_MAYORIGUAL  ::= ">="
TOKEN_OP_AND         ::= "&&" | "and" | "And" | "AND"
TOKEN_OP_OR          ::= "||" | "or" | "Or" | "OR"
TOKEN_OP_NOT         ::= "!" | "not" | "Not" | "NOT"
(* Delimitadores *)
TOKEN_PAR_A          ::= "("
TOKEN_PAR_C          ::= ")"
TOKEN_CORCH_A        ::= "["
TOKEN_CORCH_C        ::= "]"
TOKEN_LLAVE_A        ::= "{"
TOKEN_LLAVE_C        ::= "}"
TOKEN_PUNTOYCOMA     ::= ";"
TOKEN_COMA           ::= ","
(* Expresiones Regulares para Literales e Identificadores *)
TOKEN_HEX_COLOR      ::= "#" [0-9a-fA-F]{3,8}
TOKEN_LIT_INT        ::= "-"?[0-9]+
TOKEN_LIT_CHAR       ::= "'" [^'\n] "'"
TOKEN_LIT_STRING     ::= '"' [^"\n]* '"'
TOKEN_ID             ::= [a-zA-Z_][a-zA-Z0-9_]*
```

---
 
## 4. Diseño e Implementación Técnica

En esta sección se describe qué hace cada componente según el código fuente del proyecto.

### 4.1 Fase Léxica (`lexer.l`)

Flex se encarga de reconocer los componentes del lenguaje mediante patrones (expresiones regulares) y devolver los tokens a Bison. El archivo `lexer.l` define macros para componentes básicos (`LETRA`, `DIGITO`, `IDENTIFICADOR`, `ENTERO`, `CARACTER`, `STRING`) y reglas para comentarios, palabras clave, operadores, delimitadores y literales. Cuando una regla coincide, el scanner puede asignar valores a `yylval` (por ejemplo `valor_int` o `texto`) y `return` el token correspondiente.

Tabla técnica de tokens (extracción literal de `lexer.l`):

- Palabras reservadas (literales exactas): `void`, `main`, `function`, `int`, `char`, `string`, `bool`, `array`, `if`, `else`, `for`, `while`, `return`, `input`, `print`, `draw`, `time`, `open_window`, `IsKeyPressed`, `true`, `false`.
- Operadores (literales exactas): `=` `+` `-` `*` `/` `^` `>` `>=` `<` `<=` `==` `!=` `&&` `||` `!` (además de variantes textuales `and`, `or`, `not`).
- Delimitadores (literales): `(` `)` `[` `]` `{` `}` `;` `,`.
- Literales y patrones (expresiones tal como aparecen en `lexer.l`):

    - `IDENTIFICADOR` := {LETRA}({LETRA}|{DIGITO})*  (implementado en el archivo como `{IDENTIFICADOR}`)
    - `ENTERO` := -?{DIGITO}+                        (implementado como `{ENTERO}`)
    - `CARACTER` := '[^'\n]'                         (implementado como `{CARACTER}`)
    - `STRING` := "[^"\n]*"                        (implementado como `{STRING}`)
    - `TOKEN_HEX_COLOR` := #[0-9a-fA-F]{3,8}          (implementado literal: `#[0-9a-fA-F]{3,8}`)

Reglas importantes en `lexer.l`:

- Espacios y tabs se ignoran: `[ \t\r]+`
- Nuevas líneas manejadas para `yylineno` con `\n`.
- Comentarios de línea: `//`.*
- Comentarios multilínea con un estado exclusivo `%x COMENTARIO_MULTI` y reglas `"/*"` ... `"*/"`.
- Para literales con valor se asigna a `yylval` antes de `return`, por ejemplo `yylval.valor_int = atoi(yytext); return TOKEN_LIT_INT;`.

(El detalle completo de las reglas está en el archivo `Codigo del lenguaje/lexer.l`.)

### 4.2 Fase Sintáctica (`parser.y`)

Bison recibe los tokens leídos por Flex y valida la estructura del programa aplicando las reglas de la gramática. En `parser.y` se definen:

- El `%union` con los campos usados por `yylval` (`valor_int`, `texto`, `nodo`).
- Los `%token` que mapean los nombres simbólicos a los tokens que devuelve el scanner.
- Las precedencias y asociatividades (%left, %right) para operadores, incluyendo `TOKEN_OP_POT` y `TOKEN_OP_NOT`.
- Las reglas del parser que construyen nodos del AST mediante funciones auxiliares `crear_nodo_*`. Por ejemplo:
    - La regla `programa` construye `N_PROGRAMA` con la lista de sentencias.
    - Las reglas recursivas `sentencias -> sentencias sentencia` permiten enlazar listas arbitrarias de sentencias (lista enlazada mediante `siguiente`).
    - Las sentencias de control (`if`, `if-else`, `while`, `for`) construyen nodos `N_IF`, `N_WHILE`, `N_FOR` y arman subárboles con las expresiones condicionales y listas de sentencias internas.

Cuando una regla crea un identificador o literal se invocan constructores como `crear_nodo_identificador($1)`, `crear_nodo_literal($1)`, etc., que inicializan la estructura `NodoAST` (definida en `interprete.h`). En el caso de declaraciones (`TOKEN_INT TOKEN_ID ;`) se llama a `registrar_variable(...)` directamente desde la acción del parser para poblar la tabla de símbolos en tiempo de análisis sintáctico.

El manejo de reglas recursivas para estructuras de control está implementado de forma explícita: la producción del `for` usa una triple porción (inicialización, condición, actualización) y la acción construye un `N_FOR` con los nodos correspondientes.

Errores sintácticos: `yyerror` está implementado para imprimir errores en el formato `"[ERROR SINTÁCTICO - Línea %d]: %s"` usando `yylineno` (mantenido por Flex con `%option yylineno`).

### 4.3 Fase Semántica y Ejecución

En este repositorio la Tabla de Símbolos NO está implementada con `std::map` (C++). En cambio, se usa una implementación en C basada en estructuras enlazadas definida en `interprete.h`:

- `Simbolo` es una estructura que contiene `nombre`, `es_array`, `tamano_array`, `valor` (tipo `Val`), puntero `celdas` para arreglos y `siguiente` para formar una lista enlazada de símbolos en un `Scope`.
- `Scope` contiene `lista_simbolos` (puntero al primer `Simbolo`) y `padre` (para encadenar ámbitos anidados).

Operaciones observadas en el código:

- `registrar_variable(nombre, es_array, tamano, tipo)` — crea un `Simbolo` y lo inserta al frente de la lista del `scope_actual`. Para arreglos reserva `celdas` con `calloc` e inicializa cada `Val`.
- `buscar_simbolo(nombre)` — recorre la pila de scopes (siguiendo `padre`) y, en cada scope, recorre la lista enlazada `lista_simbolos` comparando `nombre` con `strcmp`. Devuelve el `Simbolo*` si lo encuentra o `NULL` si no.

Control de tipos y detección de errores:

- Las funciones `evaluar_expresion` y `evaluar_AST` usan `buscar_simbolo` para verificar existencia y para validar uso (por ejemplo evitar uso de arreglo sin índice, índices fuera de rango, asignaciones a identificadores no declarados).
- Cuando se detectan errores críticos (variable no declarada, índice fuera de rango, división por cero) el motor emite mensajes tipo `[ERROR RUNTIME]: ...` y termina con `exit(1)`.

Ejecución / Generación

El motor no genera código intermedio ni binario: ejecuta directamente el AST en memoria. Una vez `yyparse()` completa con éxito, `main` llama a `evaluar_AST(raiz_ast)` que recorre la lista de sentencias y evalúa/ejecuta cada nodo mediante `evaluar_expresion` y ramas de `evaluar_AST` según el tipo de nodo.

## 5. Guía de Uso y Casos de Prueba (QA)

### 5.1 Instrucciones de compilación

Comandos exactos (desde la carpeta `Codigo del lenguaje`):

Windows (MinGW/MSYS2):

```bash
win_flex lexer.l
win_bison -d parser.y
gcc parser.tab.c lex.yy.c interprete.c -o minilogic.exe -lgdi32 -mconsole
```
SOLO FUNCIONA EN WINDOWS
Notas Windows: se enlazan `user32` y `gdi32` para la GUI (`-lfl` no es estrictamente necesario con el `%option noyywrap` y la generación directa de `lex.yy.c`).

```bash
flex -o lex.yy.c lexer.l
bison -d -o parser.tab.c parser.y # solo si se adapta para no usar windows.h
gcc -c parser.tab.c -o parser.tab.o
gcc -c lex.yy.c -o lex.yy.o
gcc -c interprete.c -o interprete.o -o minilogic -lfl
gcc parser.tab.o lex.yy.o interprete.o -o minilogic -lfl
```

Si desea compilar en Linux y evitar dependencias Win32, es necesario envolver el código gráfico con `#ifdef _WIN32` o comentar la sección de `open_window`/`draw`.

### 5.2 Estructura de Casos de Prueba (QA)

1) Prueba Exitosa (archivo: `prueba_exitosa.min`)

```
void main() {
        int a;
        int b;
        a = 3;
        b = a * 2 + 5;
        print(b);
}
```

Salida esperada en consola (líneas relevantes, en orden):
```

===========================================
     MINILOGIC ENGINE - Compiladores 2026    
===========================================
Abriendo archivo de entrada: prueba_exitosa.min...

[SUCCESS]: Codigo parseado correctamente.
Ejecutando Arbol de Sintaxis Abstracta (AST)...

13

[FIN]: Ejecucion finalizada con exito.

```

2) Prueba con Error Sintáctico (archivo: `prueba_error_sintactico.min`)

Falta la llave de cierre:

```
void main() {
        int a;
        a = 1;
        print(a);
        // falta '}' al final

```

Comportamiento esperado:

- El parser invoca `yyerror` y produce una línea similar a:
```

[ERROR SINTÁCTICO - Línea X]: syntax error
```

- `main` detecta fallo en `yyparse()` y emite:
```

[CRITICAL]: Error fatal durante la compilacion/parseo.
```

3) Prueba con Error Semántico (archivo: `prueba_error_semantico.min`)

Uso de variable no declarada:

```
void main() {
        a = 5;
}
```

Comportamiento esperado en tiempo de ejecución:
```

[ERROR RUNTIME]: Variable 'a' no declarada.
```
Esto ocurre porque `evaluar_expresion` encuentra un nodo `N_IDENTIFICADOR`, llama a `buscar_simbolo` y, al recibir `NULL`, aborta con el mensaje anterior.
