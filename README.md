# MiniLogic — Proyecto de Lenguaje 

Este repositorio contiene MiniLogic, un lenguaje de programación interpretado y su intérprete desarrollado como proyecto universitario en la Universidad de la Cuenca del Plata. El objetivo del trabajo fue implementar las etapas clásicas de un compilador (análisis léxico, análisis sintáctico, construcción del AST y ejecución/interprete) y diseñar un lenguaje educativo con capacidades de E/S.

## Documentación principal

- Reporte técnico: [REPORTE_TECNICO_Lenguaje_MiniLogic.md](REPORTE_TECNICO_Lenguaje_MiniLogic.md)
- Guía de sintaxis: [SINTAXIS_MiniLogic.md](SINTAXIS_MiniLogic.md)

## Estructura del proyecto

- Código fuente del intérprete y gramática: `Codigo del lenguaje/` (contiene `lexer.l`, `parser.y`, `interprete.c`, `interprete.h`, etc.).
- Casos de prueba: [Tests de codigo del lenguaje](Tests de codigo del lenguaje)
  - Algunos archivos de ejemplo: [Tests de codigo del lenguaje/test.ml](Tests de codigo del lenguaje/test.ml), [Tests de codigo del lenguaje/prueba_interprete.ml](Tests de codigo del lenguaje/prueba_interprete.ml)

## Cómo compilar (Windows)

Ejecutar los siguientes comandos en la carpeta `Codigo del lenguaje` (herramientas para Windows, tal como se usó en el desarrollo):

```bash
win_flex lexer.l
win_bison -d parser.y
gcc parser.tab.c lex.yy.c interprete.c -o minilogic.exe -lgdi32 -mconsole
```

Alternativa (si se dispone de los binarios finales ya generados en la raíz):

```bash
.\minilogic.exe ruta\del\archivo.min
```

También se distribuye/usa en algunos entregables con el nombre alternativo:

```bash
.\Compilador_Minilogic.exe ruta\del\archivo.min
```

## Cómo compilar/ejecutar en entornos tipo Unix

El código contiene dependencias Win32 para la parte gráfica. Para compilar en Linux/Unix sin la GUI hay que adaptar o aislar las secciones que incluyen `windows.h` y las APIs GDI.

Comandos generales (flex/bison/gcc):

```bash
flex -o lex.yy.c lexer.l
bison -d -o parser.tab.c parser.y
gcc -c parser.tab.c -o parser.tab.o
gcc -c lex.yy.c -o lex.yy.o
gcc -c interprete.c -o interprete.o   # requiere adaptar el código gráfico para Linux
gcc parser.tab.o lex.yy.o interprete.o -o minilogic -lfl
```

## Ejecutar los scripts de prueba

Los scripts de prueba se encuentran en la carpeta `Tests de codigo del lenguaje`. Para ejecutar un test con el ejecutable generado:

```bash
.\minilogic.exe "Tests de codigo del lenguaje\prueba_interprete.ml"
```

ó

```bash
.\Compilador_Minilogic.exe "Tests de codigo del lenguaje\prueba_interprete.ml"
```

Sustituir `prueba_interprete.ml` por cualquiera de los archivos de prueba.

## Notas finales

- El proyecto fue desarrollado como trabajo práctico universitario para la asignatura de compiladores.
- Para detalles técnicos ver el [REPORTE_TECNICO_Lenguaje_MiniLogic.md](REPORTE_TECNICO_Lenguaje_MiniLogic.md).
- Para aprender a programar en MiniLogic consulte la guía de sintaxis: [SINTAXIS_MiniLogic.md](SINTAXIS_MiniLogic.md).

## Información académica y autores

Este trabajo práctico universitario fue realizado para la asignatura **Teoría de la Computación** de la Universidad de la Cuenca del Plata. Fue desarrollado por los alumnos **Blanco Facundo** y **Fagundez Gabriel**.
