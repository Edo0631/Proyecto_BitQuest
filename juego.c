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