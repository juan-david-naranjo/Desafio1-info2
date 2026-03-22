#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ─── Prototipos ───────────────────────────────────────────────────────────────
void printtable(unsigned int col, unsigned int fil, char **table);
void printinicio();
void limpiarConsola();
bool valido(unsigned int valor);
char updtable();
int  obtenerTipoAleatorio();

void         getPieceMask(int type, unsigned int rot, int dx[4], int dy[4]);
bool         canMove(unsigned int newX, unsigned int newY, unsigned int newRot,
             int type, char **table, unsigned int col, unsigned int fil);
void         lockPiece(unsigned int Xc, unsigned int Yc, unsigned int rot,
               int type, char **table);
bool         isRowFull(unsigned int row, unsigned int fil, char **table);
unsigned int clearRows(unsigned int col, unsigned int fil, char **table);
bool         isGameOver(int type, char **table, unsigned int col, unsigned int fil);
void         moveImproved(unsigned int &Xc, unsigned int &Yc, char **table,
                  char &state, unsigned int &rot, int type,
                  unsigned int col, unsigned int fil, bool &locked);
void         generateformImproved(unsigned int col, unsigned int fil, char **table,
                          bool &gameOver, unsigned int &score);

void renderI(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot);
void render2x2(unsigned int Xc, unsigned int Yc, char **table);
void renderL(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot);
void renderL2(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot);
void renderT(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot);
void renderZ(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot);
void renderZ2(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot);
void renderGeneral(unsigned int Xc, unsigned int Yc, char **table,
                   unsigned int rot, int type);

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
                col += 2;

                char **table = new char *[col];
                for (unsigned int i = 0; i < col; i++) {
                    table[i] = new char[fil];
                    for (unsigned int j = 0; j < fil; j++)
                        table[i][j] = 0;
                }

                bool gameOver = false;
                unsigned int score = 0;

                while (playing && !gameOver)
                    generateformImproved(col, fil, table, gameOver, score);

                if (gameOver) {
                    limpiarConsola();
                    printtable(col, fil, table);
                    cout << "================================" << endl;
                    cout << "          GAME  OVER            " << endl;
                    cout << "   Puntuacion: " << score         << endl;
                    cout << "================================" << endl;
                    cout << "Presiona ENTER para continuar..." << endl;
                    cin.ignore(100, '\n');
                    cin.get();
                }

                for (unsigned int i = 0; i < col; i++)
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

// ─── getPieceMask ─────────────────────────────────────────────────────────────
// Llena dx[4] y dy[4] con los desplazamientos de cada bloque relativo al
// centro (Xc, Yc). Usa arrays por referencia.
// Usa rot & 3 (≡ rot % 4) y rot & 1 (≡ rot % 2): AND bitwise con potencias de 2.
void getPieceMask(int type, unsigned int rot, int dx[4], int dy[4])
{
    switch (type) {
    case 0: // I — línea de 4 bloques
        switch (rot & 1) {
        case 0:
            dx[0] = -1; dy[0] = 0;
            dx[1] =  0; dy[1] = 0;
            dx[2] =  1; dy[2] = 0;
            dx[3] =  2; dy[3] = 0;
            break;
        case 1:
            dx[0] = 0; dy[0] = -1;
            dx[1] = 0; dy[1] =  0;
            dx[2] = 0; dy[2] =  1;
            dx[3] = 0; dy[3] =  2;
            break;
        }
        break;

    case 1: // O — cuadrado 2x2 (sin rotación)
        dx[0] = -1; dy[0] = 0;
        dx[1] = -1; dy[1] = 1;
        dx[2] =  0; dy[2] = 0;
        dx[3] =  0; dy[3] = 1;
        break;

    case 2: // L
        switch (rot & 3) {
        case 0:
            dx[0] = -1; dy[0] =  0;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  1; dy[2] =  0;
            dx[3] =  1; dy[3] =  1;
            break;
        case 1:
            dx[0] =  0; dy[0] = -1;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  0; dy[2] =  1;
            dx[3] = -1; dy[3] =  1;
            break;
        case 2:
            dx[0] = -1; dy[0] =  0;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  1; dy[2] =  0;
            dx[3] = -1; dy[3] = -1;
            break;
        case 3:
            dx[0] = 0; dy[0] = -1;
            dx[1] = 0; dy[1] =  0;
            dx[2] = 0; dy[2] =  1;
            dx[3] = 1; dy[3] =  1;
            break;
        }
        break;

    case 3: // J — L invertida
        switch (rot & 3) {
        case 0:
            dx[0] = -1; dy[0] =  0;
            dx[1] = -1; dy[1] =  1;
            dx[2] =  0; dy[2] =  0;
            dx[3] =  1; dy[3] =  0;
            break;
        case 1:
            dx[0] =  0; dy[0] = -1;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  0; dy[2] =  1;
            dx[3] = -1; dy[3] = -1;
            break;
        case 2:
            dx[0] = -1; dy[0] =  0;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  1; dy[2] =  0;
            dx[3] =  1; dy[3] = -1;
            break;
        case 3:
            dx[0] = 0; dy[0] = -1;
            dx[1] = 0; dy[1] =  0;
            dx[2] = 0; dy[2] =  1;
            dx[3] = 1; dy[3] =  1;
            break;
        }
        break;

    case 4: // T
        switch (rot & 3) {
        case 0:
            dx[0] =  0; dy[0] = -1;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  0; dy[2] =  1;
            dx[3] =  1; dy[3] =  0;
            break;
        case 1:
            dx[0] = -1; dy[0] =  0;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  1; dy[2] =  0;
            dx[3] =  0; dy[3] =  1;
            break;
        case 2:
            dx[0] =  0; dy[0] = -1;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  0; dy[2] =  1;
            dx[3] = -1; dy[3] =  0;
            break;
        case 3:
            dx[0] = -1; dy[0] =  0;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  1; dy[2] =  0;
            dx[3] =  0; dy[3] = -1;
            break;
        }
        break;

    case 5: // Z
        switch (rot & 1) {
        case 0:
            dx[0] =  0; dy[0] = -1;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  1; dy[2] =  0;
            dx[3] =  1; dy[3] =  1;
            break;
        case 1:
            dx[0] = -1; dy[0] =  0;
            dx[1] =  0; dy[1] =  0;
            dx[2] =  0; dy[2] = -1;
            dx[3] =  1; dy[3] = -1;
            break;
        }
        break;

    case 6: // S — Z invertida
        switch (rot & 1) {
        case 0:
            dx[0] =  0; dy[0] =  0;
            dx[1] =  0; dy[1] =  1;
            dx[2] =  1; dy[2] =  0;
            dx[3] =  1; dy[3] = -1;
            break;
        case 1:
            dx[0] = -1; dy[0] = -1;
            dx[1] =  0; dy[1] = -1;
            dx[2] =  0; dy[2] =  0;
            dx[3] =  1; dy[3] =  0;
            break;
        }
        break;

    default:
        dx[0] = 0; dy[0] = 0;
        dx[1] = 0; dy[1] = 0;
        dx[2] = 0; dy[2] = 0;
        dx[3] = 0; dy[3] = 0;
        break;
    }
}

// ─── canMove ──────────────────────────────────────────────────────────────────
// Por cada bloque calcula su posición absoluta y verifica:
//   - Fronteras: bx en [0, col) y by en [0, fil)
//   - Colisión:  table[bx][by] & 1  (AND bitwise; 1 = celda ocupada)
// Retorna false ante cualquier fallo.
bool canMove(unsigned int newX, unsigned int newY, unsigned int newRot,
             int type, char **table, unsigned int col, unsigned int fil)
{
    int dx[4], dy[4];
    getPieceMask(type, newRot, dx, dy);

    for (unsigned int k = 0; k < 4; k++) {
        int bx = (int)newX + dx[k];
        int by = (int)newY + dy[k];

        if (bx < 0 || bx >= (int)col) return false;
        if (by < 0 || by >= (int)fil) return false;
        if (table[bx][by] & 1)        return false;
    }

    return true;
}

// ─── lockPiece ────────────────────────────────────────────────────────────────
// Fija la pieza con OR bitwise: table[bx][by] |= 1
// El OR deja el bit en 1 permanentemente sin importar el valor previo.
void lockPiece(unsigned int Xc, unsigned int Yc, unsigned int rot,
               int type, char **table)
{
    int dx[4], dy[4];
    getPieceMask(type, rot, dx, dy);

    for (unsigned int k = 0; k < 4; k++) {
        int bx = (int)Xc + dx[k];
        int by = (int)Yc + dy[k];
        table[bx][by] |= 1;
    }
}

// ─── isRowFull ────────────────────────────────────────────────────────────────
// AND acumulado sobre la fila: si alguna celda es 0, acc queda en 0.
// Retorna true solo si todos los bits son 1.
bool isRowFull(unsigned int row, unsigned int fil, char **table)
{
    unsigned char acc = 1;
    for (unsigned int j = 0; j < fil; j++)
        acc = acc & (unsigned char)table[row][j];
    return (acc & 1) == 1;
}

// ─── clearRows ────────────────────────────────────────────────────────────────
// Recorre filas de abajo hacia arriba (ignorando el buffer oculto, filas 0-1).
// Al encontrar una fila llena:
//   1. Guarda su puntero.
//   2. Desplaza punteros de filas superiores hacia abajo (mueve punteros, no datos).
//   3. Reutiliza el puntero guardado como nueva fila vacía en el tope (tabla[2]).
//   4. Limpia esa fila con AND 0 en cada celda.
// Retorna la cantidad de filas eliminadas.
unsigned int clearRows(unsigned int col, unsigned int fil, char **table)
{
    unsigned int cleared = 0;
    unsigned int row = col - 1;

    while (row >= 2) {
        if (isRowFull(row, fil, table)) {
            char *fullRow = table[row];

            for (unsigned int r = row; r > 2; r--)
                table[r] = table[r - 1];

            table[2] = fullRow;

            for (unsigned int j = 0; j < fil; j++)
                table[2][j] = table[2][j] & 0;

            cleared++;
        } else {
            if (row == 0) break;
            row--;
        }
    }

    return cleared;
}

// ─── isGameOver ───────────────────────────────────────────────────────────────
// Intenta colocar la pieza en su posición inicial con canMove().
// Si no cabe, el tablero está lleno: game over.
bool isGameOver(int type, char **table, unsigned int col, unsigned int fil)
{
    return !canMove(1, (fil / 2) - 1, 0, type, table, col, fil);
}

// ─── moveImproved ─────────────────────────────────────────────────────────────
// 1. Borra la pieza del tablero (XOR).
// 2. Calcula posición/rotación tentativa según input.
// 3. canMove() valida:
//    - Válido              → aplica movimiento y redibuja con XOR.
//    - Inválido + bajada S → lockPiece() y activa locked.
//    - Inválido + A/D/W    → ignora el movimiento y redibuja sin cambios.
void moveImproved(unsigned int &Xc, unsigned int &Yc, char **table,
                  char &state, unsigned int &rot, int type,
                  unsigned int col, unsigned int fil, bool &locked)
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

    if (canMove(newX, newY, newRot, type, table, col, fil)) {
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
// Gestiona el ciclo completo de una pieza:
//   1. Verifica game over antes de colocar (canMove en posición inicial).
//   2. Coloca la pieza en el tablero con XOR.
//   3. Bucle de turnos: imprime tablero → input → moveImproved.
//   4. Al fijarse: clearRows() y acumula puntaje.
void generateformImproved(unsigned int col, unsigned int fil, char **table,
                          bool &gameOver, unsigned int &score)
{
    int form = obtenerTipoAleatorio();

    if (isGameOver(form, table, col, fil)) {
        gameOver = true;
        return;
    }

    unsigned int Xc  = 1;
    unsigned int Yc  = (fil / 2) - 1;
    unsigned int rot = 0;

    renderGeneral(Xc, Yc, table, rot, form);

    bool locked = false;

    while (!locked) {
        printtable(col, fil, table);
        char state = updtable();

        if (state == 'q') {
            gameOver = true;
            return;
        }

        moveImproved(Xc, Yc, table, state, rot, form, col, fil, locked);

        if (locked) {
            const unsigned int pts[5] = {0, 100, 300, 500, 800};
            unsigned int rowsCleared = clearRows(col, fil, table);

            if (rowsCleared > 0 && rowsCleared <= 4) {
                score += pts[rowsCleared];
                cout << "+" << pts[rowsCleared] << " puntos! Total: " << score << endl;
            }
        }
    }
}

// ─── Interfaz ─────────────────────────────────────────────────────────────────
void printtable(unsigned int col, unsigned int fil, char **table)
{
    limpiarConsola();
    for (unsigned int i = 2; i < col; i++) {
        for (unsigned int j = 0; j < fil; j++)
            cout << ((table[i][j] & 1) ? "# " : ". ");
        cout << "|" << endl;
    }
}

void printinicio()
{
    limpiarConsola();
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
    cout << "Accion: [A]Izq [D]Der [S]Bajar [W]Rotar [Q]Salir" << endl;
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

// ─── Render ───────────────────────────────────────────────────────────────────
// Cada función aplica XOR (^= 1) sobre las celdas de la pieza.
// Invocarla dos veces borra la pieza; una vez la dibuja o la borra.

void renderI(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot)
{
    switch (rot & 1) {
    case 0:
        table[Xc-1][Yc] ^= 1; table[Xc  ][Yc] ^= 1;
        table[Xc+1][Yc] ^= 1; table[Xc+2][Yc] ^= 1;
        break;
    case 1:
        table[Xc][Yc-1] ^= 1; table[Xc][Yc  ] ^= 1;
        table[Xc][Yc+1] ^= 1; table[Xc][Yc+2] ^= 1;
        break;
    }
}

void render2x2(unsigned int Xc, unsigned int Yc, char **table)
{
    table[Xc-1][Yc  ] ^= 1; table[Xc-1][Yc+1] ^= 1;
    table[Xc  ][Yc  ] ^= 1; table[Xc  ][Yc+1] ^= 1;
}

void renderL(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot)
{
    table[Xc][Yc] ^= 1;
    switch (rot & 3) {
    case 0: table[Xc-1][Yc  ] ^= 1; table[Xc+1][Yc  ] ^= 1; table[Xc+1][Yc+1] ^= 1; break;
    case 1: table[Xc  ][Yc+1] ^= 1; table[Xc  ][Yc-1] ^= 1; table[Xc-1][Yc+1] ^= 1; break;
    case 2: table[Xc+1][Yc  ] ^= 1; table[Xc-1][Yc  ] ^= 1; table[Xc-1][Yc-1] ^= 1; break;
    case 3: table[Xc  ][Yc-1] ^= 1; table[Xc  ][Yc+1] ^= 1; table[Xc+1][Yc+1] ^= 1; break;
    }
}

void renderL2(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot)
{
    table[Xc][Yc] ^= 1;
    switch (rot & 3) {
    case 0: table[Xc-1][Yc  ] ^= 1; table[Xc-1][Yc+1] ^= 1; table[Xc+1][Yc  ] ^= 1; break;
    case 1: table[Xc  ][Yc+1] ^= 1; table[Xc  ][Yc-1] ^= 1; table[Xc-1][Yc-1] ^= 1; break;
    case 2: table[Xc+1][Yc  ] ^= 1; table[Xc-1][Yc  ] ^= 1; table[Xc+1][Yc-1] ^= 1; break;
    case 3: table[Xc  ][Yc-1] ^= 1; table[Xc  ][Yc+1] ^= 1; table[Xc+1][Yc+1] ^= 1; break;
    }
}

void renderT(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot)
{
    table[Xc][Yc] ^= 1;
    switch (rot & 3) {
    case 0: table[Xc  ][Yc-1] ^= 1; table[Xc  ][Yc+1] ^= 1; table[Xc+1][Yc  ] ^= 1; break;
    case 1: table[Xc+1][Yc  ] ^= 1; table[Xc  ][Yc+1] ^= 1; table[Xc-1][Yc  ] ^= 1; break;
    case 2: table[Xc  ][Yc-1] ^= 1; table[Xc  ][Yc+1] ^= 1; table[Xc-1][Yc  ] ^= 1; break;
    case 3: table[Xc  ][Yc-1] ^= 1; table[Xc-1][Yc  ] ^= 1; table[Xc+1][Yc  ] ^= 1; break;
    }
}

void renderZ(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot)
{
    table[Xc][Yc] ^= 1;
    switch (rot & 1) {
    case 0: table[Xc  ][Yc-1] ^= 1; table[Xc+1][Yc  ] ^= 1; table[Xc+1][Yc+1] ^= 1; break;
    case 1: table[Xc-1][Yc  ] ^= 1; table[Xc  ][Yc-1] ^= 1; table[Xc+1][Yc-1] ^= 1; break;
    }
}

void renderZ2(unsigned int Xc, unsigned int Yc, char **table, unsigned int rot)
{
    table[Xc][Yc] ^= 1;
    switch (rot & 1) {
    case 0: table[Xc  ][Yc+1] ^= 1; table[Xc+1][Yc  ] ^= 1; table[Xc+1][Yc-1] ^= 1; break;
    case 1: table[Xc-1][Yc-1] ^= 1; table[Xc  ][Yc-1] ^= 1; table[Xc+1][Yc  ] ^= 1; break;
    }
}

void renderGeneral(unsigned int Xc, unsigned int Yc, char **table,
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
