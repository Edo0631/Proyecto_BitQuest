#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "juego.h"  
#include "mapas.h"

int jugar_nivel(char mapa[FILAS][COLAS], int numero_nivel, int *monedas_totales, int *pasos_totales){
    
    Jugador jugador;
    EstadoNivel nivel;

    nivel.numero = numero_nivel;

    //Cuenta las monedas con NASM
    nivel.total_monedas = contar_caracter(&mapa[0][0], FILAS *COLAS, MONEDA);

    //Cuenta celdas libres con NASM
    nivel.celdas_libres = contar_libres(&mapa[0][0], FILAS * COLAS);

    iniciar_jugador(mapa, &jugador);

    int nivel_terminado = 0;
    int salir = 0;

    imprimir_mapa(mapa, &jugador, &nivel);

    while(!nivel_terminado && !salir) {
        char tecla = _getch();

        if(tecla == 'q' || tecla == 'Q') {
            return 0;  //Sale del juego
        }

        mover_jugador(mapa, &jugador, tecla, &nivel_terminado);
        imprimir_mapa(mapa, &jugador, &nivel);
    }

    //Acumular los datos globales
    *monedas_totales += jugador.monedas;
    *pasos_totales += jugador.pasos;   // BUG FIX: era *= (multiplicaba por pasos, siempre 0)

    mostrar_resumen_nivel(&jugador, &nivel);

    return 1;   //Nivel Completado
}

int main(){
    int monedas_totales = 46;
    int pasos_totales = 0;
    int niveles_completados = 0;

    printf("============= BitQuest ============\n");
    printf("Presiona cualquier tecla para iniciar...\n");
    _getch();

    //NIVEL 1
    if(!jugar_nivel(mapa1, 1, &monedas_totales, &pasos_totales )) {
        return 0;
    }
    niveles_completados++;   

    //NIVEL 2
    if(!jugar_nivel(mapa2, 2, &monedas_totales, &pasos_totales )) {
        return 0;
    }
    niveles_completados++;   

    //NIVEL 3
    if(!jugar_nivel(mapa3, 3, &monedas_totales, &pasos_totales )) {
        return 0;
    }
    niveles_completados++;   

    //PUNTAJE FINAL
    long puntaje = calcular_puntaje(monedas_totales, pasos_totales, niveles_completados);
    
    printf("\n===================================\n");
    printf("Juego completado\n");
    printf("Monedas totales: %d\n", monedas_totales);
    printf("Pasos totales: %d\n", pasos_totales);
    printf("Niveles completados: %d\n", niveles_completados);
    printf("Puntaje final: %ld\n", puntaje);
    printf("===================================\n");

    printf("\nGracias por Jugar BitQuest! \n");
    
    return 0;
}