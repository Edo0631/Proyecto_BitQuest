#ifndef JUEGO_H
#define JUEGO_H

//Constantes de los tamaños para los mapas
#define FILAS     60
#define COLAS     60
#define VIS_FILAS 20    //lo que se vera al jugar
#define VIS_COLAS 20

//simbolo 2del mapa como constantes
#define PARED   '#'
#define CAMINO  '.'
#define JUGADOR 'J'
#define MONEDA  'M'
#define LLAVE   'K'
#define PUERTA  'P'
#define SALIDA  'X'

//Estructura que guarda el estado del jugador
//Donde esta, si tiene llaves, monedas, etc.
typedef struct {
    int fila, col;
    int tiene_llave;
    int monedas;
    int pasos;
} Jugador;

//info del nivel actual
typedef struct {
    int numero;
    int total_monedas;
    int celdas_libres;
} EstadoNivel;

//Prototipos de Funciones
void imprimir_mapa(char mapa[FILAS][COLAS], Jugador *j, EstadoNivel *nivel);
void mover_jugador(char mapa[FILAS][COLAS], Jugador *j, char tecla, int *nivel_terminado);
void mostrar_hud(Jugador *j, EstadoNivel *nivel);
void mostrar_resumen_nivel(Jugador *j, EstadoNivel *nivel);
void iniciar_jugador(char mapa[FILAS][COLAS], Jugador *j);

//Prototipos de funciones en NASM
long contar_caracter(char *mapa, long total_celdas, char caracter);
long validar_movimiento(char *mapa, long cols, long nueva_fila, long nueva_col);
long detectar_objeto(char *mapa, long cols, long fila, long col, char objeto);
long calcular_puntaje(long monedas, long pasos, long niveles);
long contar_libres(char *mapa, long total_celdas);

#endif


