#ifndef JUEGO_H // Evita cargar la librería Juego más de una vez
#define JUEGO_H // Marca la librería como ya incluida en la compilación

//Librerias
#include <iostream>
#include <fstream> //Manejar archivos
#include <cstring> //Cadenas de caracteres
#include <cstdlib> //Números aleatorios
#include <ctime> // ctime se usa para obtener la hora actual y así hacer que los números aleatorios cambien cada vez (pos. enemigos).
#include <cmath> //Calcular distancia entre un jugador y un arquero

using namespace std;

// Estructura para almacenar los datos del Jugador

struct Jugador {
    char nombre[50]; //Arreglo de 50 caracteres para guardar el texto del nombre del héroe
    int pv;        // Puntos de Vida
    int ph;        // Puntos de Habilidad
    int oro;       // Piezas de oro
    int posX;      // Fila actual
    int posY;      // Columna actual
};

// Estructura para representar a un Enemigo
struct Enemigo {
    char tipo;     // 'G' (Goblin), 'A' (Arquero), 'J' (Jefe)
    int pv; //Vida del enemigo
    int ph; //Daño del enemigo
    int posX;
    int posY;
    bool vivo;
    bool descubierto; //Hasta que el jugador entre a su casilla o lo ataque
};

// Estructura para representar un Cofre
struct Cofre {
    int posX;
    int posY;
    bool abierto;
};

// Estructura que reune el estado completo del juego
struct EstadoJuego {
    int n;                 // Tamaño del tablero (N x N)
    int turnoCount;        // Contador de turnos
    Jugador jugador;
    int salidaX;           // Posición Fila de la Salida
    int salidaY;           // Posición Columna de la Salida

    int numGoblins;
    int numArqueros;
    int numJefes;
    int numCofres;

    Enemigo* enemigos;     // Puntero para arreglo dinámico
    Cofre* cofres;         // Puntero para arreglo dinámico
    bool juegoActivo;      // Estado de la partida
    char* acciones;
};

// Declaración de funciones

//Inicializa la partida, asigna memoria dinámica para los arreglos, genera coordenadas aleatorias y guarda el primer registro en estado.bin
void iniciarJuego(int n, int numGoblins, int numArqueros, int numCofres);
//Imprime la matriz
void mostrarTablero();
//Muestra los datos del jugador (PV, PH y botín)
void mostrarStats();
//Mover al jugador y revisa lo si lo enemigos tienen que hacer algo
void moverJugador(const char* direccion);
//Con seek, revisa si hay cofre en la casilla y ejecuta aleatoriamente (oro, poción o trampa)
void explorarCofre();
//Con attack, hace daño al enemigo que está en la misma casilla
void atacarEnemigo();

// Carga la partida guardada; retorna true si existe, false si no.
bool cargarEstado(EstadoJuego& estado);
//Guardar la partida en el archivo estado.bin
void guardarEstado(const EstadoJuego& estado);
// Actualiza el comportamiento de todos los enemigos
// Goblins atacan si el jugador pisa su casilla
// Arqueros atacan si está adyacente
//Jefe se mueve cada 2 turnos hacia la salida.
void actualizarEventosEnemigos(EstadoJuego& estado);
// Desactiva la partida, calcula el puntaje final y registra el resultado en reporte.txt
void finalizarJuego(EstadoJuego& estado, bool victoria);
// Libera memoria con delete[] para evitar fugas de memoria
void liberarMemoriaEstado(EstadoJuego& estado);
void crearReporte(const EstadoJuego& estado);
void registrarAccion(EstadoJuego& estado, char accion);

#endif
