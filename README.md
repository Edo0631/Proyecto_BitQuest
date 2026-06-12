# BitQuest
### Juego de Exploración en Laberinto — Consola C + NASM

> **Proyecto universitario** · Programación de Sistemas / Lenguajes de Bajo Nivel  
> Implementado en **C (GCC/MinGW)** con rutinas optimizadas en **ensamblador NASM (x86-64)**

---

## Descripción General

**BitQuest** es un videojuego de exploración tipo laberinto ejecutado completamente en la consola de Windows. El jugador navega por tres niveles de complejidad creciente, recolectando monedas, obteniendo llaves, abriendo puertas y alcanzando la salida para avanzar. La mecánica central combina lógica de juego escrita en C con rutinas críticas de rendimiento implementadas directamente en lenguaje ensamblador NASM x86-64, demostrando la integración entre un lenguaje de alto nivel y código de bajo nivel.

---

## Objetivos del Proyecto

- Aplicar conceptos de programación en C: estructuras, punteros, arreglos bidimensionales y modularización.
- Implementar rutinas de bajo nivel en NASM x86-64 y enlazarlas con código C usando la convención de llamada Windows x64 (Microsoft ABI).
- Demostrar la ventaja de optimizar operaciones críticas (búsqueda lineal, validación) en ensamblador.
- Diseñar un sistema de juego funcional con múltiples niveles, HUD interactivo y puntaje acumulado.

---

## Características Principales

| Característica | Descripción |
|---|---|
| Movimiento | Teclas `W A S D` para desplazamiento en las 4 direcciones |
| Cámara parcial | Ventana de visión de 20×20 celdas centrada en el jugador |
| Coleccionables | Monedas (`M`) distribuidas en el mapa |
| Llaves y puertas | Obtener la llave (`K`) para abrir la puerta (`P`) |
| Multiples niveles | 3 mapas de 60×60 con dificultad progresiva |
| HUD en tiempo real | Nivel, monedas, estado de llave y pasos recorridos |
| Puntaje final | Calculado en NASM: `monedas×100 − pasos×2 + niveles×500` |
| Colores ANSI | Interfaz visual con códigos de escape para color en consola |
| Rutinas NASM | 5 funciones en ensamblador integradas mediante enlace externo |

---

## Tecnologías Utilizadas

- **Lenguaje C** (estándar C99) — lógica principal, estructuras y renderizado
- **NASM** (Netwide Assembler) — rutinas de bajo nivel x86-64
- **GCC / MinGW-w64** — compilación y enlace del proyecto en Windows
- **Consola de Windows** — interfaz de usuario con `conio.h` y códigos ANSI
- **Convención de llamada Windows x64** (Microsoft ABI) — integración C ↔ NASM

---

## Estructura de Archivos

```
BitQuest/
├── main.c          # Punto de entrada, flujo entre niveles y puntaje final
├── juego.c         # Lógica del jugador, renderizado y manejo de objetos
├── juego.h         # Constantes, estructuras (Jugador, EstadoNivel) y prototipos
├── mapas.h         # Definición estática de los tres mapas (60×60 chars)
├── rutinas.asm     # Rutinas optimizadas en ensamblador NASM x86-64
└── README.md       # Documentación del proyecto
```

---

## Funcionamiento del Juego

### Flujo general

El juego inicia en `main()`, donde se llama secuencialmente a `jugar_nivel()` para cada uno de los tres mapas. Si el jugador presiona `Q`, el programa termina anticipadamente. Al completar los tres niveles, se calcula y muestra el puntaje final.

### Mecánicas por turno

En cada iteración del bucle principal:

1. Se captura una tecla con `_getch()` (sin eco, sin bloqueo de buffer).
2. Se calcula la nueva posición según la dirección indicada.
3. Se valida que la celda destino no sea una pared — usando la rutina NASM `validar_movimiento()`.
4. Se detecta el objeto en la celda destino y se aplica el efecto correspondiente (recoger moneda, obtener llave, abrir puerta, alcanzar salida).
5. Se actualiza el mapa y se re-renderiza la ventana de visión.

### Objetos del mapa

| Símbolo | Color | Objeto |
|---|---|---|
| `J` | Verde | Jugador |
| `#` | Morado | Pared |
| `.` | Blanco | Camino libre |
| `M` | Amarillo | Moneda (coleccionable) |
| `K` | Cian | Llave |
| `P` | Azul | Puerta (requiere llave) |
| `X` | Rojo | Salida del nivel |

### Ventana de visión

El mapa completo es de 60×60 celdas, pero solo se renderizan 20×20 alrededor del jugador (`VIS_FILAS`, `VIS_COLAS`). El desplazamiento de cámara se calcula centrando la ventana en la posición actual del jugador, con clamp en los bordes del mapa.

---

## Integración entre C y NASM

### Convención de llamada Windows x64 (Microsoft ABI)

Las funciones NASM reciben sus primeros cuatro argumentos en los registros `RCX`, `RDX`, `R8` y `R9`. El quinto argumento en adelante se pasa por pila (offset `[RSP + 40]` dentro del cuerpo del callee, considerando el shadow space de 32 bytes).

```
Parámetro   Registro / Pila
    1º          RCX
    2º          RDX
    3º          R8
    4º          R9
    5º+         [RSP + 40] (con shadow space)
```

El valor de retorno se coloca en `RAX` (entero/puntero de 64 bits).

### Declaración en C (`juego.h`)

```c
// Prototipos — el enlazador resuelve la implementación desde rutinas.asm
long contar_caracter  (char *mapa, long total_celdas, char caracter);
long validar_movimiento(char *mapa, long cols, long nueva_fila, long nueva_col);
long detectar_objeto  (char *mapa, long cols, long fila, long col, char objeto);
long calcular_puntaje (long monedas, long pasos, long niveles);
long contar_libres    (char *mapa, long total_celdas);
```

### Rutinas implementadas

| Rutina | Función | Descripción |
|---|---|---|
| `contar_caracter` | Conteo lineal | Recorre el mapa byte a byte y acumula ocurrencias de un carácter dado. Usado para contar monedas al iniciar cada nivel. |
| `validar_movimiento` | Acceso directo | Calcula el offset `fila × cols + col` y verifica que la celda no sea `'#'`. Retorna 1 (válido) o 0 (bloqueado). |
| `detectar_objeto` | Comparación | Igual que `validar_movimiento` pero compara contra un carácter arbitrario pasado como quinto parámetro. Detecta monedas al moverse. |
| `contar_libres` | Conteo lineal | Recorre el mapa y cuenta celdas con valor `'.'`. Permite conocer el espacio transitable al inicio. |
| `calcular_puntaje` | Aritmética | Aplica la fórmula `monedas×100 − pasos×2 + niveles×500` usando `IMUL` y retorna el resultado en `RAX`. |

### Ejemplo de implementación NASM (`validar_movimiento`)

```nasm
validar_movimiento:         ; rcx=mapa, rdx=cols, r8=fila, r9=col
    mov  rax, r8
    imul rax, rdx           ; offset = fila * cols
    add  rax, r9            ; offset += col
    mov  bl,  [rcx + rax]  ; bl = mapa[fila][col]
    cmp  bl,  '#'
    je   .bloqueado
    mov  rax, 1
    ret
.bloqueado:
    mov  rax, 0
    ret
```

---

## Compilación y Ejecución

### Requisitos

- **NASM** instalado y en el PATH del sistema.
- **MinGW-w64** (GCC para Windows, 64 bits) instalado y en el PATH.

### Pasos

```bash
# 1. Ensamblar las rutinas NASM en objeto de 64 bits para Windows
nasm -f win64 rutinas.asm -o rutinas.obj

# 2. Compilar y enlazar todo el proyecto
gcc main.c juego.c rutinas.obj -o BitQuest.exe

# 3. Ejecutar
BitQuest.exe
```

> **Nota:** el flag `-f win64` es obligatorio para que NASM genere código compatible con la ABI de Windows x64 que usa GCC/MinGW.

---

## Flujo del Programa

```
main()
│
├─ Pantalla de inicio
│
├─ jugar_nivel(mapa1, 1, ...)
│   ├─ contar_caracter()  ← NASM: cuenta monedas del nivel
│   ├─ contar_libres()    ← NASM: cuenta celdas transitables
│   ├─ iniciar_jugador()  ← Localiza 'J' y establece estado inicial
│   └─ Bucle del juego
│       ├─ _getch()
│       ├─ mover_jugador()
│       │   ├─ validar_movimiento() ← NASM
│       │   └─ detectar_objeto()    ← NASM
│       └─ imprimir_mapa()
│           └─ mostrar_hud()
│
├─ mostrar_resumen_nivel()
│
├─ jugar_nivel(mapa2, 2, ...)  ← igual que nivel 1
├─ jugar_nivel(mapa3, 3, ...)  ← igual que nivel 1
│
└─ calcular_puntaje()  ← NASM: fórmula final
   └─ Pantalla de resultados
```

---

## Aprendizajes Obtenidos

- **Manejo de memoria en bajo nivel:** acceder a arreglos bidimensionales como punteros planos (`mapa[0][0]`) y calcular offsets manualmente en ensamblador.
- **Convención de llamada x64:** comprender el shadow space, el orden de registros y el paso de argumentos adicionales por pila.
- **Modularización en C:** separar lógica de juego, estructuras de datos, mapas y main en archivos independientes con cabeceras.
- **Interfaz de usuario en consola:** uso de códigos de escape ANSI para colores, `conio.h` para entrada sin eco y técnicas de re-renderizado con limpieza de pantalla.
- **Depuración entre C y ASM:** identificar desajustes en ABI (registros incorrectos, shadow space omitido) como fuente de bugs difíciles de rastrear.

---

## Posibles Mejoras Futuras

- **Modo de depuración:** mostrar el mapa completo y coordenadas del jugador.
- **Enemigos:** entidades con movimiento autónomo que el jugador debe evadir.
- **Temporizador por nivel:** presión de tiempo como factor de dificultad adicional.
- **Generación procedural de mapas:** algoritmos como _recursive backtracking_ para generar laberintos aleatorios.
- **Compatibilidad con Linux/macOS:** portar `conio.h` y `cls` a `termios.h` y `clear` para sistemas POSIX.
- **Más rutinas NASM:** optimizar el renderizado del mapa completo usando instrucciones SIMD (SSE2/AVX2).
- **Sistema de guardado:** serializar el estado del juego a un archivo binario.

---

## Conclusión

BitQuest logra integrar de forma funcional un sistema de juego escrito en C con rutinas de procesamiento de bajo nivel implementadas en NASM x86-64. El proyecto demuestra cómo la convención de llamada de Windows permite que código de alto nivel y ensamblador coexistan dentro de un mismo ejecutable, asignando a cada nivel de abstracción las tareas más adecuadas: C para la lógica de control y la interfaz, NASM para las operaciones de acceso y conteo sobre arreglos de caracteres. El resultado es un juego completamente funcional que sirve como evidencia práctica de los principios estudiados en la materia de Programación de Sistemas y Lenguajes de Bajo Nivel.

---

*Proyecto académico — Ingeniería en Sistemas de Computación*