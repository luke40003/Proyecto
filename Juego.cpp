#include "Juego.h" // Importa la librer�a del juego con las estructuras y funciones

// Funci�n para liberar la memoria din�mica asignada a las estructuras
void liberarMemoriaEstado(EstadoJuego& estado) {
    if (estado.cofres != NULL) { // Si el puntero no est� vac�o, significa que hay cofres en memoria
        delete[] estado.cofres; // Libera el arreglo din�mico de cofres
        estado.cofres = NULL; // Deja el puntero vac�o despu�s de liberar la memoria
    }
    if (estado.enemigos != NULL) { // Si hay enemigos en memoria
        delete[] estado.enemigos; // Libera el arreglo de enemigos
        estado.enemigos = NULL; // Indica que ya no apunta a ning�n enemigo
    }
}

// Verificar si una casilla est� ocupada (true: ocupada/false: libre)
bool casillaOcupada(int x, int y, const EstadoJuego& estado) { //Recibe una pos y el estado del juego por referencia
    if (x == estado.jugador.posX && y == estado.jugador.posY) return true; //Revisa si la casilla coincide con la posici�n del jugador.
    if (x == estado.salidaX && y == estado.salidaY) return true;//Revisa si la casilla es la salida del nivel

    // Recorrido de cofres
    Cofre* cPtr = estado.cofres;//Puntero que apunta al primer cofre del arreglo din�mico
    for (int i = 0; i < estado.numCofres; i++, cPtr++) { //Recorre todos los cofres
        if (cPtr->posX == x && cPtr->posY == y) return true;//Revisa si el cofre actual est� en la casilla
    }

    // Recorrido de enemigos
    int totalE = estado.numGoblins + estado.numArqueros + estado.numJefes;//Calcula cu�ntos enemigos hay en total.
    Enemigo* ePtr = estado.enemigos;//Apunta al primer enemigo del arreglo
    for (int i = 0; i < totalE; i++, ePtr++) {//Recorre todos los enemigos.
        if (ePtr->posX == x && ePtr->posY == y) return true;//Revisa si el enemigo actual est� en la casilla
    }

    return false;//Si nadie ocupaba la casilla, entonces est� libre. Devuelve false.
}

// Guardar estado en binario
void guardarEstado(const EstadoJuego& estado) {//Recibe el estado por referencia constante (no lo modifica, solo lo lee).
    fstream archivo("estado.bin", ios::out | ios::binary);//Abre un archivo llamado estado.bin.
    if (archivo) {//Verifica si el archivo se abri�

        // Escribir variables del estado

        archivo.write((char*)&estado.n, sizeof(int));//Guarda el tama�o del tablero
        archivo.write((char*)&estado.turnoCount, sizeof(int));//Guarda el n�mero de turno actual.
        archivo.write((char*)&estado.jugador, sizeof(Jugador));//Guarda toda la estructura Jugador completa
        //Guarda la posici�n de la salida.
        archivo.write((char*)&estado.salidaX, sizeof(int));
        archivo.write((char*)&estado.salidaY, sizeof(int));
        //Guarda cu�ntos enemigos hay de cada tipo.
        archivo.write((char*)&estado.numGoblins, sizeof(int));
        archivo.write((char*)&estado.numArqueros, sizeof(int));
        archivo.write((char*)&estado.numJefes, sizeof(int));
        //Guarda cu�ntos cofres hay.
        archivo.write((char*)&estado.numCofres, sizeof(int));
        //Guarda si el juego est� activo o no.
        archivo.write((char*)&estado.juegoActivo, sizeof(bool));

        // Guardar arreglos din�micos en el archivo binario
        int totalE = estado.numGoblins + estado.numArqueros + estado.numJefes;//Calcula cu�ntos enemigos hay en total para saber cu�nto escribir.
        archivo.write((char*)estado.cofres, sizeof(Cofre) * estado.numCofres);//Escribe todos los cofres del arreglo din�mico en binario.
        archivo.write((char*)estado.enemigos, sizeof(Enemigo) * totalE);//Escribe todos los enemigos del arreglo din�mico en binario.

        archivo.close();
    }
    else {//Muestra un mensaje de error en consola.
        cout << "Error al guardar el estado del juego." << endl;
    }
}

// Cargar estado reservando Memoria Din�mica
bool cargarEstado(EstadoJuego& estado) {//La funci�n devuelve true si carg� bien, false si fall�.
    fstream archivo("estado.bin", ios::in | ios::binary);
    if (archivo) {//Verifica si el archivo se abri� correctamente.
        //Lee datos del archivo y los coloca dentro del estado
        archivo.read((char*)&estado.n, sizeof(int));//Lee el tama�o del tablero.
        archivo.read((char*)&estado.turnoCount, sizeof(int));//Lee el n�mero de turno.
        archivo.read((char*)&estado.jugador, sizeof(Jugador));//Lee toda la estructura del jugador.
        //Lee la posici�n de la salida.
        archivo.read((char*)&estado.salidaX, sizeof(int));
        archivo.read((char*)&estado.salidaY, sizeof(int));
        //Lee cu�ntos enemigos hay de cada tipo.
        archivo.read((char*)&estado.numGoblins, sizeof(int));
        archivo.read((char*)&estado.numArqueros, sizeof(int));
        archivo.read((char*)&estado.numJefes, sizeof(int));
        archivo.read((char*)&estado.numCofres, sizeof(int));//Lee cu�ntos cofres hay.
        archivo.read((char*)&estado.juegoActivo, sizeof(bool));//Lee si el juego est� activo.

        //Calcula cu�ntos enemigos hay en total para saber cu�nta memoria reservar.
        int totalE = estado.numGoblins + estado.numArqueros + estado.numJefes;

        // Reserva de memoria din�mica
        estado.cofres = new Cofre[estado.numCofres];//Reserva un arreglo din�mico de cofres.
        estado.enemigos = new Enemigo[totalE];//Reserva un arreglo din�mico para todos los enemigos.

        archivo.read((char*)estado.cofres, sizeof(Cofre) * estado.numCofres);//Lee todos los cofres del archivo y los coloca en el arreglo
        archivo.read((char*)estado.enemigos, sizeof(Enemigo) * totalE);//Lee todos los enemigos y los coloca en su arreglo

        archivo.close();
        return true;//Indica que la carga fue exitosa.
    }
    return false;//Devuelve false indicando que no se pudo cargar nada.
}

// Inicializaci�n de la partida (nueva partida)
void iniciarJuego(int n, int numGoblins, int numArqueros, int numCofres) {//Recibe tam del tablero, cantidad de enemigos y cantidad de cofres

    srand((unsigned int)time(NULL));//Inicializa el generador de nums aleatorios para que cada partida sea diferente

    EstadoJuego estado;//Crea una estructura en el stack
    estado.n = n;
    estado.turnoCount = 0;
    estado.juegoActivo = true;

    strcpy(estado.jugador.nombre, "Heroe");//Hace una copia (nombre del jugador)
    //Asigna puntos de vida, puntos de ataque y oro inicial.
    estado.jugador.pv = 100;
    estado.jugador.ph = 20;
    estado.jugador.oro = 0;

    //Coloca al jugador en el centro del tablero.
    estado.jugador.posX = n / 2;
    estado.jugador.posY = n / 2;

    int lado = rand() % 4;//Elige aleatoriamente uno de los 4 lados del tablero.
    //Seg�n el lado elegido, coloca la salida en un borde aleatorio del tablero.
    if (lado == 0) { estado.salidaX = 0; estado.salidaY = rand() % n; }
    else if (lado == 1) { estado.salidaX = n - 1; estado.salidaY = rand() % n; }
    else if (lado == 2) { estado.salidaX = rand() % n; estado.salidaY = 0; }
    else { estado.salidaX = rand() % n; estado.salidaY = n - 1; }

    //Guarda las cantidades de cada tipo de enemigo y cofres.
    estado.numGoblins = numGoblins;
    estado.numArqueros = numArqueros;
    estado.numJefes = 1;
    estado.numCofres = numCofres;

    int totalE = numGoblins + numArqueros + 1;//Calcula el total de enemigos

    // Crea arreglos din�micos para cofres y enemigos.
    estado.cofres = new Cofre[numCofres];
    estado.enemigos = new Enemigo[totalE];

    // Generar cofres
    Cofre* cPtr = estado.cofres;//Crea un puntero al primer cofre.
    for (int i = 0; i < numCofres; i++, cPtr++) {//Recorre todos los cofres.
        int cx, cy;
        //Genera coordenadas aleatorias, repite hasta encontrar una casilla libre
        do {
            cx = rand() % n;
            cy = rand() % n;
        } while (casillaOcupada(cx, cy, estado));
        //Asigna la posici�n del cofre y marca que no est� abierto.
        *cPtr = {cx, cy, false};
    }

    // Generar enemigos (goblins)
    Enemigo* ePtr = estado.enemigos;//Puntero al primer enemigo.
    for (int i = 0; i < numGoblins; i++, ePtr++) {//Recorre los goblins.
        //Busca una casilla libre.
        int ex, ey;
        do {
            ex = rand() % n;
            ey = rand() % n;
        } while (casillaOcupada(ex, ey, estado));
        //Asigna atributos del goblin.
        *ePtr = {'G', 30, 10, ex, ey, true, false};
    }

    for (int i = 0; i < numArqueros; i++, ePtr++) {//Recorre los arqueros.

        int ex, ey;
        //Busca casilla libre.
        do {
            ex = rand() % n;
            ey = rand() % n;
        } while (casillaOcupada(ex, ey, estado));
        //Asigna atributos del arquero.
        *ePtr = {'A', 20, 10, ex, ey, true, false};
    }

    // Jefe
    int ex, ey;
    //Busca una casilla libre para el jefe.
    do {
        ex = rand() % n;
        ey = rand() % n;
    } while (casillaOcupada(ex, ey, estado));
    //Asigna atributos del jefe.
    *ePtr = {'J', 60, 40, ex, ey, true, false};

    guardarEstado(estado);//Guarda todo el estado en el archivo binario.
    cout << "Juego iniciado con exito" << endl;

    liberarMemoriaEstado(estado); // Liberar memoria del stack local
    mostrarTablero();
}

// Mostrar el mapa
void mostrarTablero() {

    EstadoJuego estado;//Crea un estado vac�o en el stack
    if (!cargarEstado(estado)) {//Intenta cargar el estado desde el archivo binario.
        cout << "No hay una partida activa." << endl;//Si no existe, muestra un mensaje y sale.
        return;
    }

    int N = estado.n;//Guarda el tama�o del tablero

    // Construcci�n de la matriz din�mica
    char** mapa = new char*[N];//Crea un arreglo de punteros a filas (N filas).
    for (int i = 0; i < N; i++) {//Para cada fila, reserva un arreglo din�mico de N columnas.
        *(mapa + i) = new char[N];
        for (int j = 0; j < N; j++) {//Inicializa cada casilla con '-'.
            *(*(mapa + i) + j) = '-';
        }
    }

    // Dibujar Salida (#)
    *(*(mapa + estado.salidaX) + estado.salidaY) = '#';

    // Dibujar Cofres
    Cofre* cPtr = estado.cofres;//Puntero al primer cofre.
    for (int i = 0; i < estado.numCofres; i++, cPtr++) {//Recorre todos los cofres
                //Si el cofre est� exactamente en la casilla de la salida, no lo dibuja
        if (cPtr->posX == estado.salidaX && cPtr->posY == estado.salidaY) continue;
        //Si el cofre no est� en la salida, lo dibuja
        *(*(mapa + cPtr->posX) + cPtr->posY) = cPtr->abierto ? 'O' : '?';
    }

    // Dibujar Enemigos
    int totalE = estado.numGoblins + estado.numArqueros + estado.numJefes;//Calcula cu�ntos enemigos hay en total.
    Enemigo* ePtr = estado.enemigos;//Puntero al primer enemigo.
    for (int i = 0; i < totalE; i++, ePtr++) {//Recorre todos los enemigos.
        //Si un enemigo est� en la salida, no lo dibuja.
        if (ePtr->posX == estado.salidaX && ePtr->posY == estado.salidaY) continue;

            //Si el enemigo est� vivo: Si est� descubierto dibuja su tipo (G, A, J)/Si no, dibuja 'E'
            //condici�n ? valor_si_verdadero : valor_si_falso (Si la condici�n es verdadera, usa el primer valor; si es falsa, usa el segundo)
            if (ePtr->vivo) {
                *(*(mapa + ePtr->posX) + ePtr->posY) = ePtr->descubierto ? ePtr->tipo : 'E';
            }
            // Si est� muerto dibuja X
            else {
                *(*(mapa + ePtr->posX) + ePtr->posY) = 'X';
            }
    }

    // Dibujar Jugador (@)
    *(*(mapa + estado.jugador.posX) + estado.jugador.posY) = '@';//Coloca el jugador en el mapa con '@'.

    // Imprimir el tablero
    for (int i = 0; i < N; i++) {
        cout << (i + 1) << " | ";//Imprime el n�mero de fila.
        for (int j = 0; j < N; j++) {
            cout << *(*(mapa + i) + j) << "  ";//Imprime cada casilla de la fila.
        }
        cout << "|" << endl;
    }

    // LIBERACI�N DE MEMORIA DIN�MICA DEL MAPA
    for (int i = 0; i < N; i++) {
        delete[] *(mapa + i);
    }
    delete[] mapa;//Libera el arreglo de punteros.

    // LIBERACI�N DE MEMORIA DEL ESTADO
    liberarMemoriaEstado(estado);
}

// Mostrar Stats
void mostrarStats() {
    EstadoJuego estado;//Crea una variable estado en el stack.
    if (!cargarEstado(estado)) {//Intenta cargar el estado desde estado.bin.
        cout << "No hay partida cargada." << endl;
        return;
    }

    //Mostrar estad�sticas del jugador
    cout << "Stats:" << endl;
    cout << "PV: " << estado.jugador.pv << endl;//Muestra los puntos de vida del jugador.
    cout << "PH: " << estado.jugador.ph << endl;//Muestra los puntos de ataque del jugador.
    cout << "Botin: " << estado.jugador.oro << endl;//Muestra cu�nto oro tiene el jugador.

    liberarMemoriaEstado(estado);//Libera la memoria din�mica de los arreglos
}

// Mover Jugador
void moverJugador(const char* direccion) {//Recibe texto como up, down, left, right
    EstadoJuego estado;//Crea un estado vac�o en el stack.
    if (!cargarEstado(estado) || !estado.juegoActivo) {//Carga el estado desde el archivo binario./Si no existe o el juego est� terminado, no se puede mover al jugador.
        cout << "No hay una partida activa." << endl;
        return;
    }
    //Copia la posici�n actual del jugador para calcular la nueva.
    int nuevaX = estado.jugador.posX;
    int nuevaY = estado.jugador.posY;

    //Compara cadenas y dependiendo de eso se mueve
    if (strcmp(direccion, "up") == 0) nuevaX--;
    else if (strcmp(direccion, "down") == 0) nuevaX++;
    else if (strcmp(direccion, "left") == 0) nuevaY--;
    else if (strcmp(direccion, "right") == 0) nuevaY++;
    //Si la direcci�n no es ninguna de las cuatro v�lidas, se cancela el movimiento.
    else {
        cout << "Direccion no valida." << endl;
        liberarMemoriaEstado(estado);
        return;
    }

    //Si la nueva posici�n est� fuera del tablero, el movimiento es inv�lido.
    if (nuevaX < 0 || nuevaX >= estado.n || nuevaY < 0 || nuevaY >= estado.n) {
        cout << "Movimiento invalido." << endl;
        liberarMemoriaEstado(estado);
        return;
    }

    //Actualizar posici�n del jugador
    estado.jugador.posX = nuevaX;
    estado.jugador.posY = nuevaY;
    //Aumenta el contador de turnos
    estado.turnoCount++;

    // Descubrir enemigo al pisar la casilla
    //Calcula cu�ntos enemigos hay y crea un puntero al primero.
    int totalE = estado.numGoblins + estado.numArqueros + estado.numJefes;
    Enemigo* ePtr = estado.enemigos;

    for (int i = 0; i < totalE; i++, ePtr++) {//Recorre todos los enemigos
        //Si el jugador pisa la casilla de un enemigo vivo, ese enemigo se descubre.
        if (ePtr->vivo && ePtr->posX == nuevaX && ePtr->posY == nuevaY) {
            ePtr->descubierto = true;
        }
    }

    //Se llama a finalizar juego (victoria) y se libera memoria, pero si el jefe esta en la casilla de salida, este bloquea la salida, hasta derrotarlo
    if (nuevaX == estado.salidaX && nuevaY == estado.salidaY) {
        
        bool jefeBloqueando = false;
        Enemigo* jPtr = estado.enemigos;
        for (int i = 0; i < totalE; i++, jPtr++) {
            if (jPtr->vivo && jPtr->tipo == 'J' && jPtr->posX == estado.salidaX && jPtr->posY == estado.salidaY) {
                jefeBloqueando = true;
            }
        }

        if (!jefeBloqueando) {
            finalizarJuego(estado, true);
            liberarMemoriaEstado(estado);
            return;
        } else {
            cout << "El Jefe bloquea la salida! Debes derrotarlo para escapar." << endl;
        }
    }

    //Se mueven los enemigos seg�n el mov del jugador
    actualizarEventosEnemigos(estado);

    //Si el juego sigue activo
    if (estado.juegoActivo) {
        //Guarda el estado actualizado, libera memoria din�mica y muestra el tablero actualizado
        guardarEstado(estado);
        liberarMemoriaEstado(estado);
        mostrarTablero();
    //Si el juego termin�, solo se libera memoria
    } else {
        liberarMemoriaEstado(estado);
    }
}

// Explorar Cofres (abrir un cofre)
void explorarCofre() {
    EstadoJuego estado;//Crea un estado vac�o en el stack.
    if (!cargarEstado(estado) || !estado.juegoActivo) return;//Carga el estado desde el archivo binario.

    bool encontrado = false;//Variable para saber si el jugador encontr� un cofre cerrado en su casilla.
    Cofre* cPtr = estado.cofres;//Puntero al primer cofre del arreglo din�mico.

    //Recorre todos los cofres
    for (int i = 0; i < estado.numCofres; i++, cPtr++) {
        //Verifica si el cofre est� en la misma casilla que el jugador y si el cofre est� cerrado
        if (cPtr->posX == estado.jugador.posX &&
            cPtr->posY == estado.jugador.posY &&
            !cPtr->abierto) {

            encontrado = true;//Marca que s� se encontr� un cofre v�lido.
            cPtr->abierto = true;//El cofre se abre.
            estado.turnoCount++;//Abrir un cofre consume un turno.
            srand(time(NULL));
            int prob = rand() % 100;//Genera un n�mero aleatorio entre 0 y 99 para decidir el contenido del cofre.
            
            //Asigna Oro a valores menores a 50
            if (prob < 50) {
                estado.jugador.oro += 10;
                cout << "Encontraste 10 piezas de oro." << endl;
            }
            //Asigna una pocion de vida a valores menores a 80
            else if (prob < 80) {
                estado.jugador.pv += 50;
                cout << "Pocion encontrada (+50 PV)." << endl;
            }
            //Asigna un cofre trampa que quita vida
            else {
                estado.jugador.pv -= 30;
                cout << "�Cofre trampa! (-30 PV)." << endl;
            }
            break;//Se detiene el ciclo porque ya se abri� un cofre.
        }
    }

    if (!encontrado) {
        cout << "No hay cofres cerrados en esta casilla." << endl;
    }
    //Si el jugador muri� por un cofre trampa
    else {
        if (estado.jugador.pv <= 0) {
            //Se finaliza el juego y se libera la memoria
            finalizarJuego(estado, false);
            liberarMemoriaEstado(estado);
            return;
        }
        actualizarEventosEnemigos(estado);
        //Se guarda el estado actualizado en el archivo binario si el juego sigue activo
        if (estado.juegoActivo) {
            guardarEstado(estado);
        }
    }

    //Se liberan los arreglos din�micos de cofres y enemigos que se cargaron al inicio.
    liberarMemoriaEstado(estado);
}

// Atacar Enemigos
void atacarEnemigo() {
    EstadoJuego estado;//Crea un estado vac�o en el stack.
    if (!cargarEstado(estado) || !estado.juegoActivo) return;//Carga el estado desde el archivo binario.

    int totalE = estado.numGoblins + estado.numArqueros + estado.numJefes;//Calcula cu�ntos enemigos hay en total.
    bool encontrado = false;//Variable para saber si el jugador encontr� un enemigo en su casilla.
    Enemigo* ePtr = estado.enemigos;//Puntero al primer enemigo del arreglo din�mico.

    //Buscar enemigo en la casilla del jugador
    for (int i = 0; i < totalE; i++, ePtr++) {
        //Verifica si el enemigo est� vivo y est� en la misma pos del jugador
        if (ePtr->vivo &&
            ePtr->posX == estado.jugador.posX &&
            ePtr->posY == estado.jugador.posY) {

            encontrado = true;//Marca que s� se encontr� un enemigo v�lido.
            estado.turnoCount++;//Atacar consume un turno.
            ePtr->descubierto = true;//El enemigo se revela
            ePtr->pv -= estado.jugador.ph;//El enemigo pierde vida seg�n el poder del jugador.

            cout << "Atacaste al enemigo " << ePtr->tipo
                 << " infligiendo " << estado.jugador.ph << " de dano." << endl;

            //Si el enemigo muere
            if (ePtr->pv <= 0) {
                ePtr->vivo = false;//Lo marca como muerto
                cout << "�Enemigo derrotado!" << endl;

                /// El jugador gana fuerza dependiendo del tipo de enemigo
                if (ePtr->tipo == 'G') estado.jugador.ph += 2;
                else if (ePtr->tipo == 'A') estado.jugador.ph += 3;
                else if (ePtr->tipo == 'J') estado.jugador.ph += 5;

                // NUEVA REGLA: Si derrotaste al Jefe Estando parado en la salida, ganas el juego automáticamente.
                if (ePtr->tipo == 'J' && estado.jugador.posX == estado.salidaX && estado.jugador.posY == estado.salidaY) {
                    finalizarJuego(estado, true);
                    liberarMemoriaEstado(estado);
                    return;
                }
            }
            //Si el enemigo sobrevive
            else {
                //Contraataca y el jugador pierde 10PV
                estado.jugador.pv -= ePtr->ph;
                cout << "El enemigo contraataca (-" << ePtr->ph << " PV)." << endl;
            }
            break;//Se detiene el ciclo porque ya se atac� al enemigo.
        }
    }

    //Si no se encontr� ning�n enemigo
    if (!encontrado) {
        cout << "No hay enemigos en tu casilla para atacar." << endl;
    }
    //Si el jugador muri� por el contraataque
    else {
        if (estado.jugador.pv <= 0) {
            //Se finaliza el juego (derrota) y se libera memoria
            finalizarJuego(estado, false);
            liberarMemoriaEstado(estado);
            return;
        }
        actualizarEventosEnemigos(estado);
        //Si el jugador muri� por el contraataque
        if (estado.juegoActivo) {
            guardarEstado(estado);
        }
    }

    //Liberaci�n de memoria din�mica
    liberarMemoriaEstado(estado);
}

// Esta funci�n actualiza ataques y movimientos de enemigos.
void actualizarEventosEnemigos(EstadoJuego& estado) {
    int totalE = estado.numGoblins + estado.numArqueros + estado.numJefes;//Calcula cu�ntos enemigos hay en total.
    Enemigo* ePtr = estado.enemigos;//Puntero al primer enemigo del arreglo din�mico.

    //Recorrer todos los enemigos
    for (int i = 0; i < totalE; i++, ePtr++) {
        //Si el enemigo est� muerto, se salta esta iteraci�n.
        if (!ePtr->vivo) continue;

        // Ataque Goblin
        if (ePtr->tipo == 'G' && ePtr->posX == estado.jugador.posX && ePtr->posY == estado.jugador.posY) {//Solo ataca si est� en la misma casilla que el jugador
            estado.jugador.pv -= 10;//Resta 10 PV al jugador.
            cout << "�Un Goblin te ataca en su casilla! (-10 PV)" << endl;
        }

        // Ataque Arquero
        if (ePtr->tipo == 'A') {
            //Calcula la distancia Manhattan entre arquero y jugador.
            int dist = abs(ePtr->posX - estado.jugador.posX) + abs(ePtr->posY - estado.jugador.posY);
            if (dist == 1) {//Un arquero ataca si est� justo al lado del jugador (distancia 1).
                estado.jugador.pv -= 10;//Resta 10 PV.
                cout << "�Un Arquero te ataca desde una casilla adyacente! (-10 PV)" << endl;
            }
        }
    }

    // Movimiento Jefe
    if (estado.turnoCount > 0 && estado.turnoCount % 2 == 0) {//El jefe solo se mueve cada 2 turnos
        ePtr = estado.enemigos;
        for (int i = 0; i < totalE; i++, ePtr++) {
            //Busca al jefe vivo.
            if (ePtr->vivo && ePtr->tipo == 'J') {
                //Calcula hacia d�nde debe moverse el jefe para acercarse a la salida.
                int dx = estado.salidaX - ePtr->posX;
                int dy = estado.salidaY - ePtr->posY;

                //Si la distancia horizontal es mayor, se mueve en X
                if (abs(dx) > abs(dy)) {
                    if (dx > 0) ePtr->posX++;
                    else if (dx < 0) ePtr->posX--;
                }
                //Si la distancia vertical es mayor o igual, se mueve en Y
                else if (dy != 0) {
                    if (dy > 0) ePtr->posY++;
                    else if (dy < 0) ePtr->posY--;
                }
                cout << "�El Jefe se ha desplazado una casilla hacia la salida!" << endl;
                break;//Termina el ciclo (solo hay un jefe)
            }
        }
    }

    //Si los PV del jugador bajan a 0 o menos (derrota)
    if (estado.jugador.pv <= 0) {
        finalizarJuego(estado, false);
    }
}

// Finalizar Juego y guardar reporte
void finalizarJuego(EstadoJuego& estado, bool victoria) {
    //Marca el juego como terminado.
    estado.juegoActivo = false;
    guardarEstado(estado);//Guarda en el archivo binario que el juego termin�

    //Bono por victoria
    int B = victoria ? 100 : 0;
    //Se calcula el puntaje final
    int P = B + (estado.jugador.oro * 2) + (estado.jugador.pv * 2) + (estado.jugador.ph * 5);

    //Dependiendo de victoria, muestra un mensaje distinto.
    if (victoria) cout << "�FELICIDADES! Escapaste con exito de la mazmorra." << endl;
    else cout << "�HAS SIDO DERROTADO EN LA MAZMORRA!" << endl;

    cout << "Puntaje Final: " << P << endl;//Imprime el puntaje final

    //Guardar reporte en archivo de texto (modo escritura)
    fstream reporte("reporte.txt", ios::out | ios::app);
    if (reporte) { //Verifica que el archivo abri�
        reporte << "Nombre: " << estado.jugador.nombre
                << " | Resultado: " << (victoria ? "Victoria" : "Derrota")
                << " | Puntaje: " << P << endl;
        reporte.close();
    }
}
