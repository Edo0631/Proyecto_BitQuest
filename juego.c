#include <stdio.h>
#include <stdlib.h>
#include "juego.h"

//COLORES PARA LA IMPRESION
#define RESET      "\x1b[0m"
#define ROJO       "\x1b[31m"
#define VERDE      "\x1b[32m"
#define AMARILLO   "\x1b[33m"
#define AZUL       "\x1b[34m"
#define CIAN       "\x1b[36m"
#define BLANCO     "\x1b[37m"
#define MORADO     "\x1b[35m"

static void limpiar_pantalla() {
    system("cls");
}

void iniciar_jugador(char mapa[FILAS][COLAS], Jugador *j) {
    j->tiene_llave = 0;
    j->monedas = 0;
    j->pasos = 0;

    for(int f = 0; f < FILAS; f++){
        for(int c = 0; c < COLAS; c++){
            if(mapa[f][c] == JUGADOR) {
                j->fila = f;
                j->col = c;
                return;
            }
        }
    }
}

void mostrar_hud(Jugador *j, EstadoNivel *nivel) {
    printf("Nivel: " CIAN "%d " RESET "| ", nivel->numero);
    printf("Monedas: " AMARILLO "%d/%d"  RESET "| ", j->monedas, nivel->total_monedas);
    printf("Llave: ");
    if(j->tiene_llave)
        printf(VERDE "SI " RESET "| ");
    else
        printf(ROJO "NO " RESET "| ");
    
    printf("Pasos: " MORADO "%d\n" RESET, j->pasos);
    printf("Controles: " VERDE "W A S D" RESET " | " ROJO "Q salir\n " RESET);
    printf("-----------------------------------\n");
}

void imprimir_mapa(char mapa[FILAS][COLAS], Jugador *j, EstadoNivel *nivel) {
    limpiar_pantalla();
    mostrar_hud(j, nivel);

    int inicio_fila = j->fila - VIS_FILAS / 2;
    int inicio_col  = j->col - VIS_COLAS / 2;

    if(inicio_fila < 0) inicio_fila = 0;
    if (inicio_col < 0) inicio_col = 0;

    if(inicio_fila > FILAS - VIS_FILAS)
        inicio_fila = FILAS - VIS_FILAS;

    if(inicio_col > COLAS - VIS_COLAS)
        inicio_col = COLAS - VIS_COLAS;

    for(int f = inicio_fila; f < inicio_fila + VIS_FILAS; f++) {
        for(int c = inicio_col; c < inicio_col + VIS_COLAS; c++) {
            char celda = mapa[f][c];

            if(f == j->fila && c == j->col) {
                printf(VERDE "%c" RESET, JUGADOR);
            }
            else if (celda == PARED){
                printf(MORADO "%c" RESET, '#');
            }
            else if (celda == CAMINO) {
                printf(BLANCO "%c" RESET, '.');
            }
            else if (celda == MONEDA) {
                printf(AMARILLO "%c" RESET, 'M');
            }
            else if (celda == LLAVE) {
                printf(CIAN "%c" RESET, 'K');
            }
            else if (celda == PUERTA) {
                printf(AZUL "%c" RESET, 'P');
            }
            else if (celda == SALIDA) {
                printf(ROJO "%c" RESET, 'X');
            }
            else {
                printf("%c", celda);
            }     
        }
        putchar('\n');
    }     
}

void mover_jugador(char mapa[FILAS][COLAS], Jugador *j, char tecla, int *nivel_terminado) {
    int nueva_fila = j->fila;
    int nueva_col = j->col;

    switch (tecla) {
        case 'w': case 'W': nueva_fila--; break;
        case 's': case 'S': nueva_fila++; break;
        case 'a': case 'A': nueva_col--; break;
        case 'd': case 'D': nueva_col++; break;
        default: return;
    }

    //validando limites
    if (nueva_fila < 0 || nueva_fila >= FILAS || nueva_col < 0 || nueva_col >= COLAS) {
        return;
    }

    // validar movimiento con NASM
    if(!validar_movimiento(&mapa[0][0], COLAS, nueva_fila, nueva_col)) {
        return;
    }

    char celda = mapa[nueva_fila][nueva_col];

    // Iteracion
    //MONEDA (con NASM)
    if (detectar_objeto(&mapa[0][0], COLAS, nueva_fila, nueva_col, MONEDA)) {
        j->monedas++;
        mapa[nueva_fila][nueva_col] = CAMINO;
    }

    //Llave
    if(celda == LLAVE) {
        j->tiene_llave = 1;
        mapa[nueva_fila][nueva_col] = CAMINO;
    }
    
    //Puerta
    if(celda == PUERTA) {
        if(!j->tiene_llave) {
            printf("\n Necesita la llave! \n");
            return;
        }
        mapa[nueva_fila][nueva_col] = CAMINO;
    }

    //Salida
    if(celda == SALIDA) {
        *nivel_terminado = 1;
    }

    //Mover Jugador
    mapa[j->fila][j->col] = CAMINO;
    j->fila = nueva_fila;
    j->col  = nueva_col;
    j->pasos++;
}

void mostrar_resumen_nivel(Jugador *j, EstadoNivel *nivel) {
    printf("\n===================================\n");
    printf("Nivel %d completado\n", nivel->numero);
    printf("Monedas: %d/%d\n", j->monedas, nivel->total_monedas);
    printf("Pasos: %d\n", j->pasos);
    printf("===================================\n");

    printf("ENTER para continuar: ");
    getchar();
    getchar();
}