#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ─── Prototipos ───────────────────────────────────────────────────────────────
void printtable(unsigned int fil, unsigned int col, unsigned char **table);
void printinicio();
void limpiarConsola();
bool valido(unsigned int valor);
char updtable();
int  obtenerTipoAleatorio();
void gameover(unsigned int score);

// Operaciones bitwise a nivel de byte
// El tablero es unsigned char**[fil][col/8]:
//   cada fila tiene col/8 bytes
//   cada byte almacena 8 celdas consecutivas
//   bit j de la fila i = table[i][j/8] en la posición (7 - j%8)
void         toggleBit(unsigned int x, unsigned int y, unsigned char **table);
bool         checkBit(unsigned int x, unsigned int y, unsigned char **table);
bool         isRowFull(unsigned int row, unsigned int col, unsigned char **table);
unsigned int clearRows(unsigned int fil, unsigned int col, unsigned char **table);
bool         colision(int Xc, int Yc, unsigned int rot, int type,
              unsigned int fil, unsigned int col, unsigned char **table);
bool         isGameOver(int type, unsigned int fil, unsigned int col, unsigned char **table);
void         lockPiece(unsigned int Xc, unsigned int Yc, unsigned int rot,
               int type, unsigned char **table);

void getPieceMask(int type, unsigned int rot, int dx[4], int dy[4]);
void renderGeneral(unsigned int Xc, unsigned int Yc, unsigned char **table,
                   unsigned int rot, int type);
void render2x2(unsigned int Xc, unsigned int Yc, unsigned char **table);
void renderL(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot);
void renderL2(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot);
void renderT(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot);
void renderI(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot);
void renderZ(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot);
void renderZ2(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot);

void moveImproved(unsigned int &Xc, unsigned int &Yc, unsigned char **table,
                  char &state, unsigned int &rot, int type,
                  unsigned int fil, unsigned int col, bool &locked);
void generateformImproved(unsigned int fil, unsigned int col, unsigned char **table,
                          bool &gameOver, unsigned int &score);

// ─── Main ─────────────────────────────────────────────────────────────────────
int main()
{
    srand(time(0));
    unsigned int col, fil;
    bool exit = true;
    char name[20];
    unsigned short int control = 0;

    while (exit) {
        printinicio();
        limpiarConsola();

        cout << "Ingresa tu nombre de jugador: " << endl;
        cin >> name;
        cin.ignore(100, '\n');

        bool playing = true;
        cout << "1. Nueva partida" << endl;
        cout << "2. Salir" << endl;
        cin >> control;
        limpiarConsola();

        switch (control) {
        case 1:
            cout << "Bienvenido: " << name << endl;
            cout << "Ingresa las dimensiones del Tetris (multiplos de 8)" << endl;
            cout << "Columnas: ";
            cin >> col;
            cout << "Filas: ";
            cin >> fil;

            if (valido(col) && valido(fil)) {
                // Reserva: fil filas, cada fila tiene col/8 bytes
                // Cada byte almacena 8 columnas (1 bit por celda)
                unsigned int bytescol = col / 8;
                unsigned char **table = new unsigned char *[fil];
                for (unsigned int i = 0; i < fil; i++) {
                    table[i] = new unsigned char[bytescol];
                    for (unsigned int j = 0; j < bytescol; j++)
                        table[i][j] = 0;
                }

                bool gameOver = false;
                unsigned int score = 0;

                while (playing && !gameOver)
                    generateformImproved(fil, col, table, gameOver, score);

                if (gameOver) {
                    gameover(score);
                    cin.ignore(100, '\n');
                    cin.get();
                    limpiarConsola();
                }

                for (unsigned int i = 0; i < fil; i++)
                    delete[] table[i];
                delete[] table;

            } else {
                cout << "Las dimensiones deben ser multiplos de 8" << endl;
            }
            break;

        case 2:
            cout << "Saliendo..." << endl;
            exit = false;
            break;

        default:
            cout << "Opcion invalida" << endl;
            break;
        }
    }

    return 0;
}

// ─── toggleBit ────────────────────────────────────────────────────────────────
// Invierte el bit de la celda (x=fila, y=columna) usando XOR.
//
// El byte que contiene la columna y es: table[x][y/8]
// La posición del bit dentro del byte es: 7 - (y%8)
//   (bit 7 = columna 0, bit 6 = columna 1, ..., bit 0 = columna 7)
//
// XOR con la máscara (1 << posicion) invierte solo ese bit:
//   si era 0 → queda 1  (pintar celda)
//   si era 1 → queda 0  (borrar celda)
void toggleBit(unsigned int x, unsigned int y, unsigned char **table)
{
    unsigned int byteIdx = y / 8;
    unsigned int bitPos  = 7 - (y % 8);
    table[x][byteIdx] ^= (1 << bitPos);
}

// ─── checkBit ─────────────────────────────────────────────────────────────────
// Retorna true si el bit de la celda (x, y) es 1 (celda ocupada).
// AND con la máscara aísla el bit; si es != 0, la celda está ocupada.
bool checkBit(unsigned int x, unsigned int y, unsigned char **table)
{
    unsigned int byteIdx = y / 8;
    unsigned int bitPos  = 7 - (y % 8);
    return (table[x][byteIdx] & (1 << bitPos)) != 0;
}

// ─── isRowFull ────────────────────────────────────────────────────────────────
// Una fila está llena si todos sus bytes son 0xFF (11111111 en binario).
// Se compara cada byte con 0xFF usando AND acumulado:
//   acc = 0xFF & byte0 & byte1 & ... & byteN
//   si todos son 0xFF → acc = 0xFF → fila llena
bool isRowFull(unsigned int row, unsigned int col, unsigned char **table)
{
    unsigned int bytesPerRow = col / 8;
    unsigned char acc = 0xFF;
    for (unsigned int b = 0; b < bytesPerRow; b++)
        acc = acc & table[row][b];
    return acc == 0xFF;
}

// ─── clearRows ────────────────────────────────────────────────────────────────
// Recorre filas de abajo hacia arriba.
// Si isRowFull(): guarda el puntero de esa fila, desplaza punteros hacia
// abajo (las filas superiores bajan), reutiliza el puntero como nueva
// fila vacía en el tope, y la limpia poniendo cada byte en 0 con AND 0.
// No decrementa row para reverificar la nueva fila que bajó.
unsigned int clearRows(unsigned int fil, unsigned int col, unsigned char **table)
{
    unsigned int bytesPerRow = col / 8;
    unsigned int cleared = 0;
    unsigned int row = fil - 1;

    while (row >= 2) {
        if (isRowFull(row, col, table)) {
            unsigned char *fullRow = table[row];

            for (unsigned int r = row; r > 2; r--)
                table[r] = table[r - 1];

            table[2] = fullRow;

            // AND con 0 limpia todos los bits del byte
            for (unsigned int b = 0; b < bytesPerRow; b++)
                table[2][b] = table[2][b] & 0;

            cleared++;
        } else {
            if (row == 0) break;
            row--;
        }
    }

    return cleared;
}

// ─── getPieceMask ─────────────────────────────────────────────────────────────
// Llena dx[4] y dy[4] con los offsets de cada bloque relativo al centro.
// Usa rot & 3 (≡ rot%4) y rot & 1 (≡ rot%2): AND bitwise con potencias de 2.
void getPieceMask(int type, unsigned int rot, int dx[4], int dy[4])
{
    switch (type) {
    case 0: // I — 4 bloques en línea
        switch (rot & 1) {
        case 0:
            dx[0]=-1; dy[0]=0;  dx[1]=0; dy[1]=0;
            dx[2]=1;  dy[2]=0;  dx[3]=2; dy[3]=0;
            break;
        case 1:
            dx[0]=0; dy[0]=-1;  dx[1]=0; dy[1]=0;
            dx[2]=0; dy[2]=1;   dx[3]=0; dy[3]=2;
            break;
        }
        break;

    case 1: // O — cuadrado 2x2
        dx[0]=-1; dy[0]=0;  dx[1]=-1; dy[1]=1;
        dx[2]=0;  dy[2]=0;  dx[3]=0;  dy[3]=1;
        break;

    case 2: // L
        switch (rot & 3) {
        case 0: dx[0]=-1;dy[0]=0;  dx[1]=0;dy[1]=0;  dx[2]=1;dy[2]=0;   dx[3]=1; dy[3]=1;  break;
        case 1: dx[0]=0; dy[0]=-1; dx[1]=0;dy[1]=0;  dx[2]=0;dy[2]=1;   dx[3]=-1;dy[3]=1;  break;
        case 2: dx[0]=-1;dy[0]=0;  dx[1]=0;dy[1]=0;  dx[2]=1;dy[2]=0;   dx[3]=-1;dy[3]=-1; break;
        case 3: dx[0]=0; dy[0]=-1; dx[1]=0;dy[1]=0;  dx[2]=0;dy[2]=1;   dx[3]=1; dy[3]=1;  break;
        }
        break;

    case 3: // J — L invertida
        switch (rot & 3) {
        case 0: dx[0]=-1;dy[0]=0;  dx[1]=-1;dy[1]=1; dx[2]=0;dy[2]=0;  dx[3]=1; dy[3]=0;  break;
        case 1: dx[0]=0; dy[0]=-1; dx[1]=0; dy[1]=0;  dx[2]=0;dy[2]=1;  dx[3]=-1;dy[3]=-1; break;
        case 2: dx[0]=-1;dy[0]=0;  dx[1]=0; dy[1]=0;  dx[2]=1;dy[2]=0;  dx[3]=1; dy[3]=-1; break;
        case 3: dx[0]=0; dy[0]=-1; dx[1]=0; dy[1]=0;  dx[2]=0;dy[2]=1;  dx[3]=1; dy[3]=1;  break;
        }
        break;

    case 4: // T
        switch (rot & 3) {
        case 0: dx[0]=0;dy[0]=-1; dx[1]=0;dy[1]=0; dx[2]=0;dy[2]=1;  dx[3]=1; dy[3]=0;  break;
        case 1: dx[0]=-1;dy[0]=0; dx[1]=0;dy[1]=0; dx[2]=1;dy[2]=0;  dx[3]=0; dy[3]=1;  break;
        case 2: dx[0]=0;dy[0]=-1; dx[1]=0;dy[1]=0; dx[2]=0;dy[2]=1;  dx[3]=-1;dy[3]=0;  break;
        case 3: dx[0]=-1;dy[0]=0; dx[1]=0;dy[1]=0; dx[2]=1;dy[2]=0;  dx[3]=0; dy[3]=-1; break;
        }
        break;

    case 5: // Z
        switch (rot & 1) {
        case 0: dx[0]=0;dy[0]=-1; dx[1]=0;dy[1]=0; dx[2]=1;dy[2]=0;  dx[3]=1;dy[3]=1;  break;
        case 1: dx[0]=-1;dy[0]=0; dx[1]=0;dy[1]=0; dx[2]=0;dy[2]=-1; dx[3]=1;dy[3]=-1; break;
        }
        break;

    case 6: // S — Z invertida
        switch (rot & 1) {
        case 0: dx[0]=0;dy[0]=0;  dx[1]=0; dy[1]=1;  dx[2]=1;dy[2]=0;  dx[3]=1;dy[3]=-1; break;
        case 1: dx[0]=-1;dy[0]=-1;dx[1]=0; dy[1]=-1; dx[2]=0;dy[2]=0;  dx[3]=1;dy[3]=0;  break;
        }
        break;

    default:
        dx[0]=0;dy[0]=0; dx[1]=0;dy[1]=0;
        dx[2]=0;dy[2]=0; dx[3]=0;dy[3]=0;
        break;
    }
}

// ─── colision ─────────────────────────────────────────────────────────────────
// Calcula las 4 posiciones absolutas de la pieza con getPieceMask() y verifica:
//   1. Que ningún bloque salga del tablero (fronteras)
//   2. Que ningún bloque caiga en una celda ocupada (checkBit AND bitwise)
// Retorna true si hay colisión.
bool colision(int Xc, int Yc, unsigned int rot, int type,
              unsigned int fil, unsigned int col, unsigned char **table)
{
    int dx[4], dy[4];
    getPieceMask(type, rot, dx, dy);

    for (unsigned int k = 0; k < 4; k++) {
        int bx = Xc + dx[k];
        int by = Yc + dy[k];

        if (bx < 0 || bx >= (int)fil) return true;
        if (by < 0 || by >= (int)col) return true;
        if (checkBit(bx, by, table))  return true;
    }

    return false;
}

// ─── isGameOver ───────────────────────────────────────────────────────────────
// Verifica si la nueva pieza puede colocarse en su posición inicial.
// Posición inicial: fila=1 (buffer oculto), columna central=(col/2)-1.
bool isGameOver(int type, unsigned int fil, unsigned int col, unsigned char **table)
{
    return colision(1, (int)(col / 2) - 1, 0, type, fil, col, table);
}

void gameover(unsigned int score){
    limpiarConsola();

    cout << "+==============================+" << endl;
    cout << "|         GAME  OVER           |" << endl;
    cout << "|  Puntuacion: " << score         << endl;
    cout << "+==============================+" << endl;
    cout << "Presiona ENTER para continuar..." << endl;

}





// ─── lockPiece ────────────────────────────────────────────────────────────────
// Fija la pieza al tablero usando toggleBit (XOR).
// Como la pieza ya fue borrada antes de llamar esta función,
// cada celda está en 0 y el XOR la deja en 1 permanentemente.
void lockPiece(unsigned int Xc, unsigned int Yc, unsigned int rot,
               int type, unsigned char **table)
{
    int dx[4], dy[4];
    getPieceMask(type, rot, dx, dy);

    for (unsigned int k = 0; k < 4; k++)
        toggleBit(Xc + dx[k], Yc + dy[k], table);
}

// ─── Render ───────────────────────────────────────────────────────────────────
// Cada función usa toggleBit (XOR) sobre las 4 celdas de la pieza.
// Llamarla dos veces borra la pieza; una vez la dibuja o la borra.

void renderI(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot)
{
    switch (rot & 1) {
    case 0:
        toggleBit(Xc-1,Yc,table); toggleBit(Xc,  Yc,table);
        toggleBit(Xc+1,Yc,table); toggleBit(Xc+2,Yc,table);
        break;
    case 1:
        toggleBit(Xc,Yc-1,table); toggleBit(Xc,Yc,  table);
        toggleBit(Xc,Yc+1,table); toggleBit(Xc,Yc+2,table);
        break;
    }
}

void render2x2(unsigned int Xc, unsigned int Yc, unsigned char **table)
{
    toggleBit(Xc-1,Yc,  table); toggleBit(Xc-1,Yc+1,table);
    toggleBit(Xc,  Yc,  table); toggleBit(Xc,  Yc+1,table);
}

void renderL(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot)
{
    toggleBit(Xc,Yc,table);
    switch (rot & 3) {
    case 0: toggleBit(Xc-1,Yc,  table); toggleBit(Xc+1,Yc,  table); toggleBit(Xc+1,Yc+1,table); break;
    case 1: toggleBit(Xc,  Yc+1,table); toggleBit(Xc,  Yc-1,table); toggleBit(Xc-1,Yc+1,table); break;
    case 2: toggleBit(Xc+1,Yc,  table); toggleBit(Xc-1,Yc,  table); toggleBit(Xc-1,Yc-1,table); break;
    case 3: toggleBit(Xc,  Yc-1,table); toggleBit(Xc,  Yc+1,table); toggleBit(Xc+1,Yc+1,table); break;
    }
}

void renderL2(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot)
{
    toggleBit(Xc,Yc,table);
    switch (rot & 3) {
    case 0: toggleBit(Xc-1,Yc,  table); toggleBit(Xc-1,Yc+1,table); toggleBit(Xc+1,Yc,  table); break;
    case 1: toggleBit(Xc,  Yc+1,table); toggleBit(Xc,  Yc-1,table); toggleBit(Xc-1,Yc-1,table); break;
    case 2: toggleBit(Xc+1,Yc,  table); toggleBit(Xc-1,Yc,  table); toggleBit(Xc+1,Yc-1,table); break;
    case 3: toggleBit(Xc,  Yc-1,table); toggleBit(Xc,  Yc+1,table); toggleBit(Xc+1,Yc+1,table); break;
    }
}

void renderT(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot)
{
    toggleBit(Xc,Yc,table);
    switch (rot & 3) {
    case 0: toggleBit(Xc,  Yc-1,table); toggleBit(Xc,  Yc+1,table); toggleBit(Xc+1,Yc,  table); break;
    case 1: toggleBit(Xc+1,Yc,  table); toggleBit(Xc,  Yc+1,table); toggleBit(Xc-1,Yc,  table); break;
    case 2: toggleBit(Xc,  Yc-1,table); toggleBit(Xc,  Yc+1,table); toggleBit(Xc-1,Yc,  table); break;
    case 3: toggleBit(Xc,  Yc-1,table); toggleBit(Xc-1,Yc,  table); toggleBit(Xc+1,Yc,  table); break;
    }
}

void renderZ(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot)
{
    toggleBit(Xc,Yc,table);
    switch (rot & 1) {
    case 0: toggleBit(Xc,  Yc-1,table); toggleBit(Xc+1,Yc,  table); toggleBit(Xc+1,Yc+1,table); break;
    case 1: toggleBit(Xc-1,Yc,  table); toggleBit(Xc,  Yc-1,table); toggleBit(Xc+1,Yc-1,table); break;
    }
}

void renderZ2(unsigned int Xc, unsigned int Yc, unsigned char **table, unsigned int rot)
{
    toggleBit(Xc,Yc,table);
    switch (rot & 1) {
    case 0: toggleBit(Xc,  Yc+1,  table); toggleBit(Xc+1,Yc,  table); toggleBit(Xc+1,Yc-1,table); break;
    case 1: toggleBit(Xc-1,Yc-1,  table); toggleBit(Xc,  Yc-1,table); toggleBit(Xc+1,Yc,  table); break;
    }
}

void renderGeneral(unsigned int Xc, unsigned int Yc, unsigned char **table,
                   unsigned int rot, int type)
{
    switch (type) {
    case 0: renderI  (Xc, Yc, table, rot); break;
    case 1: render2x2(Xc, Yc, table     ); break;
    case 2: renderL  (Xc, Yc, table, rot); break;
    case 3: renderL2 (Xc, Yc, table, rot); break;
    case 4: renderT  (Xc, Yc, table, rot); break;
    case 5: renderZ  (Xc, Yc, table, rot); break;
    case 6: renderZ2 (Xc, Yc, table, rot); break;
    }
}

// ─── moveImproved ─────────────────────────────────────────────────────────────
// 1. Borra la pieza (toggleBit via renderGeneral — XOR la pone en 0).
// 2. Calcula posición/rotación tentativa.
// 3. colision() verifica fronteras y checkBit AND en cada bloque.
//    - Sin colisión  → actualiza posición y redibuja.
//    - Colisión + S  → lockPiece() y activa locked.
//    - Colisión + otro → ignora y redibuja en posición actual.
void moveImproved(unsigned int &Xc, unsigned int &Yc, unsigned char **table,
                  char &state, unsigned int &rot, int type,
                  unsigned int fil, unsigned int col, bool &locked)
{
    locked = false;
    renderGeneral(Xc, Yc, table, rot, type);

    unsigned int newX   = Xc;
    unsigned int newY   = Yc;
    unsigned int newRot = rot;

    switch (state) {
    case 'a': if (Yc > 0) newY = Yc - 1; break;
    case 'd':             newY = Yc + 1;  break;
    case 's':             newX = Xc + 1;  break;
    case 'w': newRot = (rot + 1) & 3;     break;
    default:  break;
    }

    if (!colision((int)newX, (int)newY, newRot, type, fil, col, table)) {
        Xc  = newX;
        Yc  = newY;
        rot = newRot;
    } else if (state == 's') {
        lockPiece(Xc, Yc, rot, type, table);
        locked = true;
        return;
    }

    renderGeneral(Xc, Yc, table, rot, type);
}

// ─── generateformImproved ─────────────────────────────────────────────────────
// Ciclo completo de una pieza:
//   1. isGameOver() → colision en posición inicial → game over.
//   2. Coloca pieza con renderGeneral (toggleBit XOR).
//   3. Bucle: printtable → updtable → moveImproved.
//   4. Al fijarse: clearRows() con isRowFull (AND 0xFF) y acumula puntaje.
void generateformImproved(unsigned int fil, unsigned int col, unsigned char **table,
                          bool &gameOver, unsigned int &score)
{
    int form = obtenerTipoAleatorio();

    if (isGameOver(form, fil, col, table)) {
        gameOver = true;
        return;
    }

    unsigned int Xc  = 1;
    unsigned int Yc  = (col / 2) - 1;
    unsigned int rot = 0;

    renderGeneral(Xc, Yc, table, rot, form);

    bool locked = false;

    while (!locked) {
        printtable(fil, col, table);
        char state = updtable();

        if (state == 'q') {
            gameOver = true;
            return;
        }

        moveImproved(Xc, Yc, table, state, rot, form, fil, col, locked);

        if (locked) {
            const unsigned int pts[5] = {0, 100, 300, 500, 800};
            unsigned int rowsCleared = clearRows(fil, col, table);

            if (rowsCleared > 0 && rowsCleared <= 4) {
                score += pts[rowsCleared];
                cout << "+" << pts[rowsCleared] << " puntos! Total: " << score << endl;
            }
        }
    }
}

// ─── Interfaz ─────────────────────────────────────────────────────────────────
// printtable usa checkBit para leer cada celda bit a bit.
// El borde superior usa fil-2 filas visibles (filas 0 y 1 son buffer oculto).
void printtable(unsigned int fil, unsigned int col, unsigned char **table)
{
    limpiarConsola();
    cout << "  TETRIS C++" << endl;
    cout << "+";
    for (unsigned int j = 0; j < col; j++) cout << "--";
    cout << "+" << endl;

    for (unsigned int i = 2; i < fil; i++) {
        cout << "|";
        for (unsigned int j = 0; j < col; j++)
            cout << (checkBit(i, j, table) ? "# " : ". ");
        cout << "|" << endl;
    }

    cout << "+";
    for (unsigned int j = 0; j < col; j++) cout << "--";
    cout << "+" << endl;
    cout << "  [A]Izq [D]Der [S]Bajar [W]Rotar [Q]Salir" << endl;
}

void printinicio()
{
    cout << "========================================" << endl;
    cout << "       BIENVENIDO A TETRIS C++          " << endl;
    cout << "========================================" << endl;
    cout << "  [A] Izquierda  [D] Derecha            " << endl;
    cout << "  [S] Bajar      [W] Rotar   [Q] Salir  " << endl;
    cout << "----------------------------------------" << endl;
    cout << "Presiona ENTER para comenzar..." << endl;
    cin.get();
}

void limpiarConsola()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// valido: multiplo de 8 si los 3 bits bajos son 0  (valor & 7 == 0)
bool valido(unsigned int valor)
{
    return (valor & 7) == 0;
}

char updtable()
{
    char m;
    cin >> m;
    switch (m) {
    case 'a': case 'A': return 'a';
    case 'd': case 'D': return 'd';
    case 's': case 'S': return 's';
    case 'w': case 'W': return 'w';
    case 'q': case 'Q': return 'q';
    default:
        cout << "Opcion invalida" << endl;
        return '0';
    }
}

int obtenerTipoAleatorio()
{
    return rand() % 7;
}
