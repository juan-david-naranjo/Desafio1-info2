#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;
void printtable(unsigned int col,unsigned int fil,char **table);    //imprimir la tabla !!!!NO LA ACTUALIZA OJO
void printinicio(); //mostrar el incio del juego
void limpiarConsola();
bool valido(unsigned int valor);    //valdiar dimemsiones tabla
char updtable();      //actuaizar la tabla


void move(unsigned int &Xc, unsigned int &Yc, char** table, char &state, unsigned int &rot,int typ,unsigned int col,unsigned int fil);
void generateform(unsigned int col,unsigned int fil,char**table,unsigned int &x,unsigned int &y);        // generar las fichas
void renderI(unsigned int &Xc, unsigned int &Yc, char**table,unsigned int &rot);        //actualiza el estado de la ficha I
void render2x2(unsigned int &Xc, unsigned int &Yc, char**table);        //actualiza el estado de la ficha
void renderL(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot); //dibuja a la pieza L
void renderL2(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot); //dibuja a la pieza L2
void renderT(unsigned int &Xc, unsigned int &Yc, char**table,unsigned int &rot);   //dibuja T
void renderZ(unsigned int &Xc, unsigned int &Yc, char** table, unsigned int &rot); //dibuja a z
void renderZ2(unsigned int &Xc, unsigned int &Yc, char** table, unsigned int &rot); //dinuja a Z2 o Z invertida
void renderGeneral(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot, int type);  //para esoger que tipo de ficha vamos a renderizar
int obtenerTipoAleatorio();



int main()
{
    srand(time(0));
    unsigned int col,fil;
    bool exit=true;
    char name[20];
    unsigned short int control=0;
    unsigned int rotacion=0;
    // codigo principal del juego
    while(exit){
        printinicio();
        limpiarConsola();


        cout<<"Ingresa tu nombre de jugador: "<<endl;
        cin>>name;
        cin.ignore(100, '\n');
        bool playing=true;
        cout<<"1.Nueva partida"<<endl;
        cout<<"2.Salir"<<endl;
        cin>>control;
        limpiarConsola();
        switch (control) {
        case 1:
            cout<<"Bienvenido: "<<name<<endl;
            cout<<"Ingresa las Dimensiones del tetrtis (multiplos de 8)"<<endl;




                    //validar dimensiones sean multiplos de 8
                    cout<<"Ingresa la cantidad de columnas: ";
                    cin>>col;
                    cout<<"Ingresa la cantidad de filas: ";
                    cin>>fil;


                    //reserva de las dimensiones del tablero
                    if(valido(col)&&valido(fil)){
                        col+=2;
                        char **table = new char*[col];
                        for(unsigned int i=0;i<col;i++){
                            table[i]=new char[fil];
                            for (unsigned int j=0;j<fil;j++){
                                table[i][j]=0;
                            }
                        }
                        int c=0;
                    while(playing){
                        unsigned int X=0;
                        unsigned int Y=0;

                        printtable(col,fil,table);
                        generateform(col,fil,table,X,Y);
                        c++;
                        if(c>0){playing=false;}

                    }








                        //eliminacion del tablero dinamico
                        for(unsigned int i=0;i<col;i++){
                            delete []table[i];
                        }
                        delete []table;
                        cout<<"tabla eliminada"<<endl;
                    }else{
                        cout<<"Recuerde que las dimensiones del tetris deben ser multiplos de 8"<<endl;
                        playing=false;
                    }







         break;

        case 2:
            cout<<"Saliendo del juego"<<endl;
            exit=false;
            break;
        default:
            cout<<"Ingrese una opcion valida"<<endl;
            break;
        }




    }

}


// funciones del tetris
void printtable(unsigned int col,unsigned int fil,char **table){
    limpiarConsola();
    for (int i = 2; i < col; i++) {
        for (int j = 0; j < fil; j++) {

            switch(table[i][j]) {
            case 0:
                cout << ". "; // Vacío (un punto es discreto)
                break;
            case 1:
                cout << "+ "; // Pieza (caracteres que parecen bloques)
                break;
            default:
                cout << "? ";
            }
        }
        cout << "|" << endl;
    }

    return;
}

void printinicio(){
    cout << "========================================" << endl;
    cout << "       ¡BIENVENIDO A TETRIS C++!        " << endl;
    cout << "========================================" << endl;
    cout << "  Instrucciones:                       " << endl;
    cout << "  - Usa las teclas para mover piezas.  " << endl;
    cout << "  - Completa filas para ganar puntos.  " << endl;
    cout << "----------------------------------------" << endl;
    cout << "Presiona ENTER para comenzar..." << endl;
    cin.get();    // Espera a que el usuario presione una tecla
}
void limpiarConsola() {
// Si estamos en Windows, usa "cls", si no, usa "clear"
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool valido(unsigned int valor){
    if(valor%8==0){return true;}
    else{return false;}
}


char updtable(){



    cout<<"Accion: [A]Izq [D]Der [S]Bajar [W]Rotar [Q]Exit"<<endl;
    char move;
    cin>>move;
    switch (move) {
    case 'a':
    case 'A':
        cout<<"Moviendose a la izq"<<endl;
        return 'a';
    case 'd':
    case 'D':
        cout<<"Moviendose a la Der"<<endl;
        return 'd';
    case 's':
    case 'S':
        cout<<"Bajando un bit"<<endl;
        return 's';
    case 'w':
    case'W':
        cout<<"Rotando 90 grados"<<endl;
        return 'w';
    case 'q':
    case 'Q':
        cout<<"saliendo...."<<endl;
        return 's';
    default:
        cout<<"Ingresaste una opcion invalida"<<endl;
        break;
    }


    return '0';
}

// generar las fichas
void generateform(unsigned int col,unsigned int fil,char**table,unsigned int &x,unsigned int &y){
    int form=obtenerTipoAleatorio();
    char state;
    unsigned int rot=0;
    unsigned int mid=(fil/2)-1;
    x=1;
    y=mid;

    int c=0;
    switch (form) {
    case 0:
        table[x][mid]=1;
        table[x-1][y]^=1;        //al inicio las coordenadas de la ficha es la mitad
        table[x+1][y]^=1;
        while(c<16){

            printtable(col,fil,table);
            state=updtable();
            move(x,y,table,state,rot,form,col,fil);
            cout<<"saliendo"<<endl;
            c++;
        }break;
    case 1:
        table[x][mid]=1;
        table[x-1][y]^=1;
        table[x-1][y+1]^=1;     //inicializamos el cuadro 2x2
        table[x][y+1]^=1;
        while(c<16){

            printtable(col,fil,table);
            state=updtable();
            move(x,y,table,state,rot,form,col,fil);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;

    case 2:
        table[x-1][y]^=1;
        table[x][y]^=1;
        table[x+1][y]^=1;        //inicializamos L
        table[x+1][y+1]^=1;
        while(c<16){

            printtable(col,fil,table);
            state=updtable();
            move(x,y,table,state,rot,form,col,fil);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;

    case 3:
        table[x-1][y]^=1;
        table[x+1][y]^=1;     //inicializamos L invertida
        table[x-1][y+1]^=1;
        table[x][y]^=1;
        while(c<16){
            printtable(col,fil,table);
            state=updtable();
             move(x,y,table,state,rot,form,col,fil);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;
    case 4:
        table[x][y]^=1;
        table[x+1][y]^=1;
        table[x][y-1]^=1;     //inicializamos T
        table[x][y+1]^=1;
        while(c<16){
            printtable(col,fil,table);
            state=updtable();
            move(x,y,table,state,rot,form,col,fil);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;
    case 5:
        table[x][y-1]^=1;
        table[x][y]^=1;
        table[x+1][y]^=1;     //inicializamos z
        table[x+1][y+1]^=1;
        while(c<16){
            printtable(col,fil,table);
            state=updtable();
            move(x,y,table,state,rot,form,col,fil);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;
    case 6:
        table[x][y+1]^=1;
        table[x][y]^=1;
        table[x+1][y]^=1;     //inicializamos z2
        while(c<16){
            printtable(col,fil,table);
            state=updtable();
            move(x,y,table,state,rot,form,col,fil);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;
    default:
        cout<<"Ingresaste una opcion invalida"<<endl;
        break;
    }

    return;
}


void renderI(unsigned int &Xc, unsigned int &Yc, char**table,unsigned int &rot){

    table[Xc][Yc] ^= 1;

    switch(rot % 2) {
    case 0: // 0° I normal parada
        table[Xc-1][Yc] ^= 1;
        table[Xc+1][Yc] ^= 1;
        break;
    case 1: // 90° - I invertida acostada
        table[Xc][Yc+1] ^= 1;
        table[Xc][Yc-1] ^= 1;
        break;
    }
}

void render2x2(unsigned int &Xc, unsigned int &Yc, char**table){
    table[Xc][Yc]^=1;
    table[Xc][Yc+1]^=1;
    table[Xc-1][Yc]^=1;
    table[Xc-1][Yc+1]^=1;
}


void renderL(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot) {
    // El centro siempre es parte de la ficha
    table[Xc][Yc] ^= 1;

    switch(rot % 4) {
    case 0: // 0° - L normal parada

        table[Xc-1][Yc] ^= 1;
        table[Xc+1][Yc] ^= 1;
        table[Xc+1][Yc+1] ^= 1;
        break;
    case 1: // 90° - L acostada
        table[Xc][Yc+1] ^= 1;
        table[Xc][Yc-1] ^= 1;
        table[Xc-1][Yc+1] ^= 1;

        break;
    case 2: // 180° - L cabeza abajo
        table[Xc+1][Yc] ^= 1;
        table[Xc-1][Yc] ^= 1;
        table[Xc-1][Yc-1] ^= 1;
        break;
    case 3: // 270° - L acostada invertida
        table[Xc][Yc-1] ^= 1;
        table[Xc][Yc+1] ^= 1;
        table[Xc+1][Yc+1] ^= 1;
        break;
    }
    return;
}

void renderL2(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot) {
    // El centro siempre es parte de la ficha
    table[Xc][Yc] ^= 1;

    switch(rot % 4) {
    case 0: // 0° - L invertida normal parada
        table[Xc-1][Yc] ^= 1;
        table[Xc-1][Yc+1] ^= 1;
        table[Xc+1][Yc] ^= 1;
        break;
    case 1: // 90° - L invertida acostada
        table[Xc][Yc+1] ^= 1;
        table[Xc][Yc-1] ^= 1;
        table[Xc-1][Yc-1] ^= 1;

        break;
    case 2: // 180° - L cabeza abajo
        table[Xc+1][Yc] ^= 1;
        table[Xc-1][Yc] ^= 1;
        table[Xc+1][Yc-1] ^= 1;
        break;
    case 3: // 270° - L acostada invertida
        table[Xc][Yc-1] ^= 1;
        table[Xc][Yc+1] ^= 1;
        table[Xc+1][Yc+1] ^= 1;
        break;
    }
    return;
}

void renderT(unsigned int &Xc, unsigned int &Yc, char**table,unsigned int &rot){

    // El centro siempre es parte de la ficha
    table[Xc][Yc] ^= 1;

    switch(rot % 4) {
    case 0: // 0° - T invertida normal parada
        table[Xc][Yc-1] ^= 1;
        table[Xc][Yc+1] ^= 1;
        table[Xc+1][Yc] ^= 1;
        break;
    case 1: // 90° - T invertida acostada
        table[Xc+1][Yc] ^= 1;
        table[Xc][Yc+1] ^= 1;
        table[Xc-1][Yc] ^= 1;

        break;
    case 2: // 180° - T cabeza abajo
        table[Xc][Yc-1] ^= 1;
        table[Xc][Yc+1] ^= 1;
        table[Xc-1][Yc] ^= 1;
        break;
    case 3: // 270° - T acostada invertida
        table[Xc][Yc-1] ^= 1;
        table[Xc-1][Yc] ^= 1;
        table[Xc+1][Yc] ^= 1;
        break;
    }
    return;


}

void renderZ(unsigned int &Xc, unsigned int &Yc, char** table, unsigned int &rot) {
    // El centro siempre es parte de la ficha
    table[Xc][Yc] ^= 1;

    switch(rot % 2) {
    case 0: // Z Horizontal
        table[Xc][Yc-1] ^= 1;   // Bloque a la izquierda
        table[Xc+1][Yc] ^= 1;   // Bloque abajo
        table[Xc+1][Yc+1] ^= 1; // Bloque abajo a la derecha
        break;
    case 1: // Z Vertical
        table[Xc-1][Yc] ^= 1;   // Bloque arriba
        table[Xc][Yc-1] ^= 1;   // Bloque a la izquierda
        table[Xc+1][Yc-1] ^= 1; // Bloque abajo a la izquierda
        break;
    }
}


void renderZ2(unsigned int &Xc, unsigned int &Yc, char** table, unsigned int &rot) {
    // El centro siempre es parte de la ficha
    table[Xc][Yc] ^= 1;

    switch(rot % 2) {
    case 0: // S Horizontal (Z invertida)
        table[Xc][Yc+1] ^= 1;   // Bloque a la derecha
        table[Xc+1][Yc] ^= 1;   // Bloque abajo
        table[Xc+1][Yc-1] ^= 1; // Bloque abajo a la izquierda
        break;
    case 1: // S Vertical
        table[Xc-1][Yc-1] ^= 1; // Bloque arriba a la izquierda
        table[Xc][Yc-1] ^= 1;   // Bloque a la izquierda
        table[Xc+1][Yc] ^= 1;   // Bloque abajo
        break;
    }
}

void move(unsigned int &Xc, unsigned int &Yc, char** table, char &state, unsigned int &rot,int type,unsigned int col,unsigned int fil){
    // Dibujo inicial
    // 1. BORRAR la pieza en su posición y rotación actual
    renderGeneral(Xc, Yc, table, rot, type);

    // 2. ACTUALIZAR
    switch(state) {
    case 'a': // Izquierda
        if(Yc>0){
            Yc -= 1;
        }
        break;
    case 'd': // Derecha
        if(Yc+1>fil-1)break;
         Yc += 1;
        break;
    case 's': // Abajo
        if(Xc+1>col-1)break;
         Xc += 1;
        break;
    case 'w': // Rotar
        rot = (rot + 1) % 4;
        break;
    }

    // 3. Dibuja la pieza en la nueva posición o rotación
    renderGeneral(Xc, Yc, table, rot, type);
}

void renderGeneral(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot, int type) {
    switch(type) {
    case 0: renderI(Xc, Yc, table, rot); break;     //dibujamos I
    case 1: render2x2(Xc, Yc, table); break;        //dibujamos 2x2
    case 2: renderL(Xc, Yc, table, rot); break;     //dibujamos L
    case 3: renderL2(Xc, Yc, table, rot); break;    //dibujamos l2 L(invertida)
    case 4: renderT(Xc, Yc, table, rot); break;     //dibujamos T
    case 5: renderZ(Xc, Yc, table, rot); break;     //dibujamos Z
    case 6: renderZ2(Xc, Yc, table, rot); break;    //dibujamos Z2
    }
}


int obtenerTipoAleatorio() {
    // Genera un número entre 0 y 6
    // La fórmula es: rand() % (MAX - MIN + 1) + MIN
    return rand() % 7;
}


