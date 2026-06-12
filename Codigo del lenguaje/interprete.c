#include "interprete.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

/* --- VARIABLES GLOBALES REALES --- */
Scope* scope_actual = NULL;

/* Contexto para la ventana gráfica de Windows (GDI con Double Buffering) */
HWND global_hwnd = NULL;
HDC global_hdc = NULL;      
HDC mem_dc = NULL;          
HBITMAP mem_bm = NULL;      
int global_ancho = 800;
int global_alto = 600;

extern FILE* yyin;

/* --- AUXILIARES DEL SISTEMA EN WINDOWS --- */

void procesar_mensajes() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    static DWORD ultimo_tick = 0;
    DWORD tick_actual = GetTickCount();
    
    if (tick_actual - ultimo_tick < 16) {
        return; 
    }
    
    if (global_hdc && mem_dc) {
        BitBlt(global_hdc, 0, 0, global_ancho, global_alto, mem_dc, 0, 0, SRCCOPY);
    }
    
    Sleep(1); 
    ultimo_tick = tick_actual;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_ERASEBKGND:
            return 1; 
        case WM_DESTROY:
            global_hwnd = NULL;
            global_hdc = NULL;
            if (mem_dc) DeleteDC(mem_dc);
            if (mem_bm) DeleteObject(mem_bm);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


/* --- GESTIÓN DE LA TABLA DE SÍMBOLOS --- */

Simbolo* buscar_simbolo(const char* nombre) {
    Scope* s = scope_actual;
    while (s != NULL) {
        Simbolo* sym = s->lista_simbolos;
        while (sym != NULL) {
            if (strcmp(sym->nombre, nombre) == 0) return sym;
            sym = sym->siguiente;
        }
        s = s->padre;
    }
    return NULL;
}


/* --- EVALUADOR DE EXPRESIONES --- */

/* --- IMPRIMIR VALORES FORMATEADOS --- */
void imprimir_val(Val val) {
    if (val.tipo == T_BOOL) {
        printf("%s\n", val.v_int ? "true" : "false");
    } else if (val.tipo == T_CHAR || val.tipo == T_STRING) {
        if (val.v_str) {
            int len = strlen(val.v_str);
            if (len >= 2 && ((val.v_str[0] == '"' && val.v_str[len-1] == '"') || 
                             (val.v_str[0] == '\'' && val.v_str[len-1] == '\''))) {
                for (int i = 1; i < len - 1; i++) {
                    putchar(val.v_str[i]);
                }
                putchar('\n');
            } else {
                printf("%s\n", val.v_str);
            }
        } else {
            printf("\n");
        }
    } else {
        printf("%d\n", val.v_int);
    }
}

/* --- EVALUADOR DE EXPRESIONES --- */

Val evaluar_expresion(NodoAST* nodo) {
    Val res;
    res.tipo = T_INT;
    res.v_int = 0;
    res.v_str = NULL;

    if (!nodo) return res;

    switch (nodo->tipo) {
        case N_LITERAL:
            res.tipo = T_INT;
            res.v_int = nodo->valor_int;
            return res;

        case N_LITERAL_BOOL:
            res.tipo = T_BOOL;
            res.v_int = nodo->valor_int;
            return res;

        case N_LITERAL_CHAR:
            res.tipo = T_CHAR;
            res.v_str = nodo->valor_texto ? strdup(nodo->valor_texto) : NULL;
            return res;

        case N_LITERAL_STRING:
            res.tipo = T_STRING;
            res.v_str = nodo->valor_texto ? strdup(nodo->valor_texto) : NULL;
            return res;

        case N_IDENTIFICADOR: {
            Simbolo* sym = buscar_simbolo(nodo->valor_texto);
            if (!sym) {
                fprintf(stderr, "[ERROR RUNTIME]: Variable '%s' no declarada.\n", nodo->valor_texto);
                exit(1);
            }
            if (sym->es_array) {
                fprintf(stderr, "[ERROR RUNTIME]: El identificador '%s' es un arreglo, requiere indice.\n", nodo->valor_texto);
                exit(1);
            }
            return sym->valor;
        }

        case N_EXPRESION_BINARIA: {
            if (strcmp(nodo->operador, "[]") == 0) {
                Simbolo* sym = buscar_simbolo(nodo->hijos[0]->valor_texto);
                if (!sym || !sym->es_array) {
                    fprintf(stderr, "[ERROR RUNTIME]: Arreglo '%s' no valido.\n", nodo->hijos[0]->valor_texto);
                    exit(1);
                }
                int idx = evaluar_expresion(nodo->hijos[1]).v_int;
                if (idx < 0 || idx >= sym->tamano_array) {
                    fprintf(stderr, "[ERROR RUNTIME]: Indice %d fuera de rango en '%s'.\n", idx, sym->nombre);
                    exit(1);
                }
                return sym->celdas[idx];
            }

            Val izq_val = evaluar_expresion(nodo->hijos[0]);
            Val der_val = evaluar_expresion(nodo->hijos[1]);
            int izq = izq_val.v_int;
            int der = der_val.v_int;

            res.tipo = T_INT;

            if (strcmp(nodo->operador, "+") == 0) { res.v_int = izq + der; return res; }
            if (strcmp(nodo->operador, "-") == 0) { res.v_int = izq - der; return res; }
            if (strcmp(nodo->operador, "*") == 0) { res.v_int = izq * der; return res; }
            if (strcmp(nodo->operador, "/") == 0) {
                if (der == 0) { fprintf(stderr, "[ERROR]: Division por cero.\n"); res.v_int = 0; return res; }
                res.v_int = izq / der;
                return res;
            }
            if (strcmp(nodo->operador, "==") == 0) { res.tipo = T_BOOL; res.v_int = (izq == der); return res; }
            if (strcmp(nodo->operador, "!=") == 0) { res.tipo = T_BOOL; res.v_int = (izq != der); return res; }
            if (strcmp(nodo->operador, "<") == 0)  { res.tipo = T_BOOL; res.v_int = (izq < der); return res; }
            if (strcmp(nodo->operador, ">") == 0)  { res.tipo = T_BOOL; res.v_int = (izq > der); return res; }
            if (strcmp(nodo->operador, "<=") == 0) { res.tipo = T_BOOL; res.v_int = (izq <= der); return res; }
            if (strcmp(nodo->operador, ">=") == 0) { res.tipo = T_BOOL; res.v_int = (izq >= der); return res; }
            if (strcmp(nodo->operador, "&&") == 0) { res.tipo = T_BOOL; res.v_int = (izq && der); return res; }
            if (strcmp(nodo->operador, "||") == 0) { res.tipo = T_BOOL; res.v_int = (izq || der); return res; }
            
            if (strcmp(nodo->operador, "^") == 0) {
                int r = 1;
                for (int i = 0; i < der; i++) r *= izq;
                res.v_int = r;
                return res;
            }
            return res;
        }

        case N_EXPRESION_UNARIA: {
            Val valor = evaluar_expresion(nodo->hijos[0]);
            if (strcmp(nodo->operador, "!") == 0) {
                res.tipo = T_BOOL;
                res.v_int = !valor.v_int;
                return res;
            }
            return res;
        }

        case N_TIME:
            res.tipo = T_INT;
            res.v_int = (int)time(NULL);
            return res;

        case N_IS_KEY_PRESSED: {
            int vk_code = evaluar_expresion(nodo->hijos[0]).v_int;
            res.tipo = T_BOOL;
            if (vk_code < 0 || vk_code >= 256) {
                res.v_int = 0;
                return res;
            }

            static int tecla_presionada_antes[256] = {0};
            static DWORD tiempo_primer_toque[256] = {0};
            static DWORD tiempo_ultima_repeticion[256] = {0};

            int estado_actual = (GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0;
            DWORD ahora = GetTickCount();

            if (estado_actual) {
                if (!tecla_presionada_antes[vk_code]) {
                    tecla_presionada_antes[vk_code] = 1;
                    tiempo_primer_toque[vk_code] = ahora;
                    tiempo_ultima_repeticion[vk_code] = ahora;
                    res.v_int = 1;
                    return res;
                } else {
                    DWORD tiempo_retenido = ahora - tiempo_primer_toque[vk_code];
                    if (tiempo_retenido >= 220) { 
                        if (ahora - tiempo_ultima_repeticion[vk_code] >= 60) {
                            tiempo_ultima_repeticion[vk_code] = ahora;
                            res.v_int = 1;
                            return res; 
                        }
                    }
                }
            } else {
                tecla_presionada_antes[vk_code] = 0;
            }
            res.v_int = 0;
            return res;
        }

        default:
            return res;
    }
}


/* --- EVALUADOR DE SENTENCIAS --- */

void evaluar_AST(NodoAST* nodo) {
    while (nodo != NULL) {
        switch (nodo->tipo) {
            case N_PROGRAMA:
                evaluar_AST(nodo->hijos[0]);
                break;

            case N_ASIGNACION: {
                Simbolo* sym = buscar_simbolo(nodo->valor_texto);
                if (!sym) {
                    fprintf(stderr, "[ERROR RUNTIME]: Variable '%s' no declarada.\n", nodo->valor_texto);
                    exit(1);
                }
                sym->valor = evaluar_expresion(nodo->hijos[0]);
                break;
            }

            case N_ASIGNACION_ARRAY: {
                Simbolo* sym = buscar_simbolo(nodo->valor_texto);
                if (!sym || !sym->es_array) {
                    fprintf(stderr, "[ERROR RUNTIME]: Variable '%s' no es un arreglo.\n", nodo->valor_texto);
                    exit(1);
                }
                int idx = evaluar_expresion(nodo->hijos[0]).v_int;
                if (idx < 0 || idx >= sym->tamano_array) {
                    fprintf(stderr, "[ERROR RUNTIME]: Indice %d fuera de rango en '%s'.\n", idx, sym->nombre);
                    exit(1);
                }
                sym->celdas[idx] = evaluar_expresion(nodo->hijos[1]);
                break;
            }

            case N_PRINT:
                imprimir_val(evaluar_expresion(nodo->hijos[0]));
                break;

            case N_IF:
                if (evaluar_expresion(nodo->hijos[0]).v_int) {
                    evaluar_AST(nodo->hijos[1]);
                } else if (nodo->hijos[2] != NULL) {
                    evaluar_AST(nodo->hijos[2]);
                }
                break;

            case N_WHILE:
                while (evaluar_expresion(nodo->hijos[0]).v_int) {
                    procesar_mensajes(); 
                    evaluar_AST(nodo->hijos[1]);
                }
                break;

            case N_FOR:
                evaluar_AST(nodo->hijos[0]); 
                while (evaluar_expresion(nodo->hijos[1]).v_int) { 
                    procesar_mensajes(); 
                    evaluar_AST(nodo->hijos[3]); 
                    evaluar_AST(nodo->hijos[2]); 
                }
                break;

            case N_OPEN_WINDOW: {
                global_ancho = evaluar_expresion(nodo->hijos[0]).v_int;
                global_alto = evaluar_expresion(nodo->hijos[1]).v_int;

                static int clase_registrada = 0;
                if (!clase_registrada) {
                    WNDCLASS wc = {0};
                    wc.lpfnWndProc = WndProc;
                    wc.hInstance = GetModuleHandle(NULL);
                    wc.lpszClassName = "MinilogicWinClass";
                    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
                    RegisterClass(&wc);
                    clase_registrada = 1;
                }

                // CORRECCIÓN: Estilo rígido sin WS_THICKFRAME (redimensionar) ni WS_MAXIMIZEBOX (maximizar)
                DWORD estilo_fijo = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;

                // Forzamos a que el lienzo interno mida EXACTAMENTE lo que pidió el desarrollador
                RECT rect_ventana = {0, 0, global_ancho, global_alto};
                AdjustWindowRect(&rect_ventana, estilo_fijo, FALSE);
                int ancho_final_ventana = rect_ventana.right - rect_ventana.left;
                int alto_final_ventana = rect_ventana.bottom - rect_ventana.top;

                global_hwnd = CreateWindowEx(0, "MinilogicWinClass", "MiniLogic", 
                                             estilo_fijo,
                                             CW_USEDEFAULT, CW_USEDEFAULT, ancho_final_ventana, alto_final_ventana, 
                                             NULL, NULL, GetModuleHandle(NULL), NULL);
                if (global_hwnd) {
                    global_hdc = GetDC(global_hwnd);
                    
                    mem_dc = CreateCompatibleDC(global_hdc);
                    mem_bm = CreateCompatibleBitmap(global_hdc, global_ancho, global_alto);
                    SelectObject(mem_dc, mem_bm);
                    
                    RECT rect = {0, 0, global_ancho, global_alto};
                    FillRect(mem_dc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

                    ShowWindow(global_hwnd, SW_SHOW);
                    UpdateWindow(global_hwnd);
                    SetForegroundWindow(global_hwnd);
                    SetFocus(global_hwnd);
                }
                break;
            }

            case N_DRAW: {
                if (!mem_dc) break; 
                int x = evaluar_expresion(nodo->hijos[0]).v_int;
                int y = evaluar_expresion(nodo->hijos[1]).v_int;

                int r = 255, g = 255, b = 255;
                if (nodo->valor_texto && strlen(nodo->valor_texto) >= 7) {
                    sscanf(nodo->valor_texto + 1, "%02x%02x%02x", &r, &g, &b);
                }
                
                COLORREF color = RGB(r, g, b);
                HBRUSH brush = CreateSolidBrush(color);
                
                RECT rect = { x, y, x + 20, y + 20 }; 
                FillRect(mem_dc, &rect, brush);
                DeleteObject(brush);
                break;
            }

            case N_INPUT: {
                char* prompt = nodo->valor_texto;
                if (prompt && prompt[0] == '"') {
                    int len = strlen(prompt);
                    for (int i = 1; i < len - 1; i++) {
                        putchar(prompt[i]);
                    }
                } else if (prompt) {
                    printf("%s", prompt);
                }
                
                int valor;
                if (scanf("%d", &valor) != 1) {
                    valor = 0;
                    // flush stdin
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                }
                
                Simbolo* sym = buscar_simbolo(nodo->operador);
                if (!sym) {
                    fprintf(stderr, "[ERROR RUNTIME]: Variable '%s' no declarada.\n", nodo->operador);
                    exit(1);
                }
                sym->valor.tipo = T_INT;
                sym->valor.v_int = valor;
                sym->valor.v_str = NULL;
                break;
            }

            default:
                break;
        }
        nodo = nodo->siguiente;
    }
}


/* --- PUNTO DE ENTRADA PRINCIPAL --- */

int main(int argc, char* argv[]) {
    printf("===========================================\n");
    printf("   MINILOGIC ENGINE - Compiladores 2026    \n");
    printf("===========================================\n");

    if (argc > 1) {
        printf("Abriendo archivo de entrada: %s...\n", argv[1]);
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "[CRITICAL]: No se pudo abrir el archivo '%s'.\n", argv[1]);
            return 1;
        }
    } else {
        printf("Abriendo flujo de entrada (Consola)... \n");
        printf("[NOTA]: Escriba el codigo y presione Enter + Ctrl+Z para finalizar.\n\n");
    }

    if (yyparse() == 0) {
        printf("\n[SUCCESS]: Codigo parseado correctamente.\n");
        printf("Ejecutando Arbol de Sintaxis Abstracta (AST)...\n\n");
        
        evaluar_AST(raiz_ast);
        
        printf("\n[FIN]: Ejecucion finalizada con exito.\n");
        
        if (global_hwnd != NULL) {
            printf("Manteniendo ventana abierta. Cierre los graficos para terminar...\n");
            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    } else {
        fprintf(stderr, "\n[CRITICAL]: Error fatal durante la compilacion/parseo.\n");
    }

    if (argc > 1 && yyin != NULL) {
        fclose(yyin);
    }

    return 0;
}