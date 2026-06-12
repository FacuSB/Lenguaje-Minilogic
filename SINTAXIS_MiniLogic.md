# Guía de Sintaxis — MiniLogic

Esta guía resume la sintaxis práctica del lenguaje MiniLogic (declaraciones, asignaciones, arreglos, control de flujo, E/S y gráficos).

## 1. Literales y tipos

- Enteros: números con signo opcional, ejemplo: `123`, `-42` (token `TOKEN_LIT_INT`).
- Booleanos: `true`, `false` (tokens `TOKEN_TRUE`, `TOKEN_FALSE`).
- Caracteres: comilla simple, ejemplo: `'a'` (token `TOKEN_LIT_CHAR`).
- Cadenas: comillas dobles, ejemplo: `"hola"` (token `TOKEN_LIT_STRING`).
- Colores HEX: `#RRGGBB` o versiones abreviadas (`#RGB`, hasta 8 hexadecimales) (token `TOKEN_HEX_COLOR`).

## 2. Programa principal

La entrada debe contener la función principal con la siguiente forma:

```
void main() {
    /* sentencias */
}
```

## 3. Declaración de variables

- Entero: `int nombre;`
- Booleano: `bool nombre;`
- Char: `char nombre;`
- String: `string nombre;`
- Array (tamaño fijo): `int nombre[10];` o `array nombre[10];` (según uso en el proyecto)

Ejemplo:

```
int x;
bool flag;
char c;
string s;
int arr[5];
```

Nota: las declaraciones se reconocen en el parser y llaman a `registrar_variable(...)` durante el parsing.

## 4. Asignaciones

- Asignación simple: `identificador = expresion;`
- Asignación a celda de arreglo: `identificador[indice] = expresion;`

Ejemplos:

```
x = 10;
arr[2] = x + 3;
```

En un `for` la inicialización puede ser una asignación o una declaración con asignación: `for (i = 0; i < 10; i = i + 1) { ... }` o `for (int j = 0; j < 5; j = j + 1) { ... }`.

## 5. Expresiones y operadores

- Aritméticos: `+ - * / ^` (`^` es potencia, implementada por el intérprete como multiplicaciones repetidas).
- Comparación: `== != < > <= >=`.
- Lógicos: `&&` / `and`, `||` / `or`, `!` / `not`.
- Paréntesis para agrupar: `( expresion )`.
- Acceso a arreglo: `id[expresion]`.

Ejemplo:

```
int a;
a = (3 + 2) * 4 ^ 2;   // ^ es potencia
if ((a > 10) && (a < 100)) { print(a); }
```

## 6. Estructuras de control

- If simple:

```
if (condicion) {
    /* sentencias */
}
```

- If-else:

```
if (condicion) {
    /* entonces */
} else {
    /* sino */
}
```

- While:

```
while (condicion) {
    /* sentencias */
}
```

- For:

```
for (inicializacion; condicion; actualizacion) {
    /* sentencias */
}
```

Donde `inicializacion` puede ser `IDENT = expresion` o `int IDENT = expresion`.

## 7. Entrada y salida de datos

- Entrada por consola (lectura de enteros):

```
input("Ingrese un valor:", mi_var);
```

El `input` recibe una cadena (prompt) y el nombre de la variable donde guardar el entero.

- Salida por consola:

```
print(expresion);
```

`print` muestra valores formateados: booleans se muestran como `true`/`false`, strings/chars se imprimen sin las comillas.

## 8. Funciones de tiempo y teclado

- `time()` devuelve un entero (timestamp) — uso: `time()`.
- `IsKeyPressed(codigo)` devuelve booleano indicando si la tecla (código virtual) está presionada. Implementa debounce para evitar repeticiones rápidas.

Ejemplo:

```
if (IsKeyPressed(32)) { print("Espacio presionado"); }
```

## 9. Gráficos: abrir ventana y dibujar

El soporte gráfico usa Win32 GDI. Estas sentencias están en el parser e intérprete:

- Abrir ventana (tamaño):

```
open_window(ancho, alto);
```

Esto crea una ventana Win32 con un bitmap en memoria (double buffering) cuyo lienzo tiene dimensiones `ancho`x`alto`.

- Dibujar bloque (20x20) de color en (x,y):

```
draw(x, y, #RRGGBB);
```

Ejemplo:

```
open_window(800, 600);
int i;
for (i = 0; i < 10; i = i + 1) {
    draw(i*22, 50, #FF0000); // dibuja bloques rojos separados
}
```

Notas técnicas:

- `draw` toma la expresión `x`, `y` y un literal `#hex` (token `TOKEN_HEX_COLOR`). En el intérprete se parsea el `#RRGGBB` para extraer `r,g,b` y se llena un rect de 20x20 píxeles en el bitmap en memoria.
- La ventana usa `BitBlt` para volcar el buffer en pantalla y `procesar_mensajes()` para atender eventos. El código gráfico depende de `windows.h` y se compila solo en Windows a menos que se adapte.

## 10. Ejemplos completos

1) Programa que abre ventana y dibuja:

```
void main() {
    open_window(400, 300);
    draw(10, 10, #00FF00);
    draw(40, 10, #0000FF);
}
```

2) Programa con input y print:

```
void main() {
    int n;
    input("Ingrese n:", n);
    print(n);
}
```

3) Uso de arreglos:

```
void main() {
    int a[3];
    a[0] = 5;
    a[1] = a[0] + 2;
    print(a[1]);
}
```
