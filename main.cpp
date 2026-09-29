#include "Juego.h" //Archivo con todas las funciones

//argc cantidad de argumentos escritos en la consola.
//argv arreglo de cadenas con esos argumentos.
int main(int argc, char* argv[]) {

    //Si no hay argumentos, se muestra cómo usarlo
    if (argc < 2) {
        cout << "Uso correcto: .\\juego.exe [comando] [opciones]" << endl;
        cout << "Comandos: start, board, stats, move [up|down|left|right], seek, attack" << endl;
        return 1;
    }

    // Comando 'start'
    if (strcmp(argv[1], "start") == 0) {
        //Valores por defecto
        int n = 10, goblins = 3, arqueros = 2, cofres = 3;
        //El usuario ingresa los valores
        cout << "N = "; cin >> n;
        cout << "Goblins = "; cin >> goblins;
        cout << "Arqueros = "; cin >> arqueros;
        cout << "Cofres = "; cin >> cofres;

        iniciarJuego(n, goblins, arqueros, cofres);
    }
    // Comando 'board' - Mostrar tablero
    else if (strcmp(argv[1], "board") == 0) {
        mostrarTablero();
    }
    // Comando 'stats' - Puntos de vida, ataque y oro del jugador
    else if (strcmp(argv[1], "stats") == 0) {
        mostrarStats();
    }
    // Comando 'move' - Si falta la dirección, muestra error.
    else if (strcmp(argv[1], "move") == 0) {
        if (argc < 3) {
            cout << "Debes especificar la direccion: move [up|down|left|right]" << endl;
            return 1;
        }
        //Si está bien, llama a la función que mueve al jugador.
        moverJugador(argv[2]);
    }
    // Comando 'seek' - Intenta abrir un cofre en la casilla actual
    else if (strcmp(argv[1], "seek") == 0) {
        explorarCofre();
    }
    // Comando 'attack' - Ataca al enemigo que esté en la misma casilla
    else if (strcmp(argv[1], "attack") == 0) {
        atacarEnemigo();
    }

    //Comando desconocido
    else {
        cout << "Comando no reconocido." << endl;
    }

    return 0;
}
