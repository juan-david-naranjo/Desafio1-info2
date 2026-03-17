#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;
void printtable(unsigned int col,unsigned int fil,char **table);    //imprimir la tabla !!!!NO LA ACTUALIZA OJO
void printinicio(); //mostrar el incio del juego
void limpiarConsola();
bool valido(unsigned int valor);    //valdiar dimemsiones tabla
char updtable();      //actuaizar la tabla
void generateform(unsigned int col,unsigned int fil,char**table,unsigned int &x,unsigned int &y);        // generar las fichas
void moveI(unsigned int &Xc, unsigned int &Yc, char**table,char &state,unsigned int &rot);        //actualiza el estado de la ficha
void move2x2(unsigned int &Xc, unsigned int &Yc, char**table,char &state,unsigned int &rot);    //actualiza el estado de la ficha 2x2
void moveL1(unsigned int &Xc, unsigned int &Yc, char**table,char &state,unsigned int &rot);     //actualiza el estado de la ficha L inversa
void renderL(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot); //dibuja a la pieza L


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
        cout<<"1.Nueva partida"<<endl;
        cout<<"2.Salir"<<endl;
        cin>>control;
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
                unsigned int X=0;
                unsigned int Y=0;

                printtable(col,fil,table);
                generateform(col,fil,table,X,Y);










                //eliminacion del tablero dinamico
                for(unsigned int i=0;i<col;i++){
                    delete []table[i];
                }
                delete []table;
                cout<<"tabla eliminada"<<endl;
            }else{
                cout<<"Recuerde que las dimensiones del tetris deben ser multiplos de 8"<<endl;
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
    unsigned short int form=2;
    char state;
    unsigned int rot=0;
    unsigned int mid=(fil/2)-1;
    x=1;
    y=mid;

    int c=0;
    switch (form) {
    case 0:
        table[x][mid]=1;
        cout<<"pieza I"<<endl;  //al inicio las coordenadas de la ficha es la mitad
        table[x+1][y]^=1;
        while(c<16){

            printtable(col,fil,table);
            state=updtable();
            moveI(x,y,table,state,rot);
             cout<<"saliendo"<<endl;
             c++;
        }





        break;
    case 1:
        table[x][mid]=1;
        table[x-1][y]^=1;
        table[x-1][y+1]^=1;     //inicializamos el cuadro 2x2
        table[x][y+1]^=1;
        while(c<16){

            printtable(col,fil,table);
            state=updtable();
            move2x2(x,y,table,state,rot);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;

    case 2:
        // table[x-1][y]^=1;
        // table[x-1][y+1]^=1;     //inicializamos L
        // table[x+1][y]^=1;
        while(c<16){

            printtable(col,fil,table);
            state=updtable();
            moveL1(x,y,table,state,rot);
            cout<<"saliendo"<<endl;
            c++;
        }
        break;

    case 3:
        cout<<"pieza L "<<endl;
        for(unsigned short int i=0;i<3;i++){
            table[i][fil]=1;
            if(i==0){table[i][1]=1;}
            printtable(col,fil,table);
        }
        break;
    case 4:
        cout<<"pieza T "<<endl;
        for(unsigned short int i=0;i<2;i++){
            table[i][fil]=1;
            if(i==0){table[i][fil+1]=1;
            table[i][fil-1]=1;}
            printtable(col,fil,table);
        }
        break;
    case 5:
        cout<<"pieza z inversa "<<endl;
        for(unsigned short int i=0;i<2;i++){
            table[i][fil]=1;
            if(i==0){table[i][1]=1;}else{table[i][+1]=1;}

            printtable(col,fil,table);
        }
        break;
    case 6:
        cout<<"pieza z  "<<endl;
        for(unsigned short int i=0;i<2;i++){
            table[i][1];
            if(i==0){table[i][+1]=1;}else{table[i][-1]=1;}

            printtable(col,fil,table);
        }
        break;
    default:
        cout<<"Ingresaste una opcion invalida"<<endl;
        break;
    }

    return;
}


void moveI(unsigned int &Xc, unsigned int &Yc, char**table,char &state,unsigned int &rot){
    //Xcentro es la coordenada central en x de la ficha<--->Ycentro es la coordenada central en y de la ficha
    switch(state) {
    case 'a':
        Yc-=1;
        table[Xc-1][Yc]^=1;
        table[Xc][Yc]^=1;
        table[Xc+1][Yc]^=1;
        // limpiar movimiento
        table[Xc-1][Yc+1]&=0;
        table[Xc][Yc+1]&=0;
        table[Xc+1][Yc+1]&=0;
        return;
    case 's':
        Xc+=1;
        table[Xc-1][Yc]&=1;
        table[Xc][Yc]&=1;
        table[Xc+1][Yc]^=1;
        // limpiar movimiento
        table[Xc-2][Yc]&=0;


        return;
    case 'd':
        Yc+=1;
        table[Xc-1][Yc]^=1;
        table[Xc][Yc]^=1;
        table[Xc+1][Yc]^=1;
        // limpiar movimiento
        table[Xc-1][Yc-1]&=0;
        table[Xc][Yc-1]&=0;
        table[Xc+1][Yc-1]&=0;

        return;
    case 'w':
        // 1. Borrar la forma actual ANTES de cambiar la rotación
        if (rot % 2 == 0) { // Estaba vertical
            table[Xc-1][Yc] ^= 1;
            table[Xc][Yc] &= 1;
            table[Xc+1][Yc] ^= 1;
        } else { // horizontal
            table[Xc][Yc-1] &= 0;
            table[Xc][Yc] &= 1;
            table[Xc][Yc+1] &= 0;
        }

        // 2. Cambiar el estado de rotación
        rot = (rot + 1) % 2; // Alterna entre 0 y 1

        // 3. Dibujar la nueva forma
        if (rot != 0) { // Ahora es Horizontal
            table[Xc][Yc-1] ^= 1;
            table[Xc][Yc] &= 1;
            table[Xc][Yc+1] ^= 1;
        } else { // Ahora es Vertical
            table[Xc-1][Yc] ^= 1;
            table[Xc][Yc] &= 1;
            table[Xc+1][Yc] ^= 1;
        }


        return;
    case 'q':
        cout<<"caso q"<<endl;
        return;
    default:
        return;
    }

}


void move2x2(unsigned int &Xc, unsigned int &Yc, char**table,char &state,unsigned int &rot){
    //Xcentro es la coordenada central en x de la ficha<--->Ycentro es la coordenada central en y de la ficha
    //inicializar la ficha

    switch(state) {
    case 'a':
        Yc-=1;
        table[Xc-1][Yc]^=1;
        table[Xc][Yc]^=1;
        // limpiar movimiento
        table[Xc-1][Yc+2]&=0;
        table[Xc][Yc+2]&=0;
        return;
    case 's':
        Xc+=1;
        table[Xc-1][Yc]&=1;
        table[Xc-1][Yc+1]&=1;
        table[Xc][Yc]^=1;
        table[Xc][Yc+1]^=1;
        // limpiar movimiento
        table[Xc-2][Yc]&=0;
        table[Xc-2][Yc+1]&=0;


        return;
    case 'd':
        Yc+=1;
        table[Xc-1][Yc+1]^=1;
        table[Xc][Yc+1]^=1;
        // limpiar movimiento
        table[Xc-1][Yc-1]&=0;
        table[Xc][Yc-1]&=0;

        return;
    case 'w':
        //2x2 no rota
        return;
    case 'q':
        cout<<"caso q"<<endl;
        return;
    default:
        return;
    }

}

void moveL1(unsigned int &Xc, unsigned int &Yc, char** table, char &state, unsigned int &rot) {
    // 1. Borrar la posición actual (XOR invierte el bit de 1 a 0)
    renderL(Xc, Yc, table, rot);

    // 2. Actualizar según la tecla
    switch(state) {
    case 'a': // Izquierda
        Yc -= 1;
        break;
    case 'd': // Derecha
        Yc += 1;
        break;
    case 's': // Abajo
        Xc += 1;
        break;
    case 'w': // Rotar 90 grados
        rot = (rot + 1) % 4;
        break;
    case 'q':
        return;
    }

    // 3. Pintar en la nueva posición (XOR invierte el bit de 0 a 1)
    renderL(Xc, Yc, table, rot);
}


void renderL(unsigned int Xc, unsigned int Yc, char** table, unsigned int rot) {
    // El centro siempre es parte de la ficha
    table[Xc][Yc] ^= 1;

    switch(rot % 4) {
    case 0: // 0° - L normal parada
        table[Xc-2][Yc] ^= 1;
        table[Xc-1][Yc] ^= 1;
        table[Xc+1][Yc] ^= 1;
        table[Xc+1][Yc+1] ^= 1;
        break;
    case 1: // 90° - L acostada
        table[Xc][Yc+1] ^= 1;
        table[Xc][Yc-1] ^= 1;
        table[Xc+1][Yc-1] ^= 1;

        break;
    case 2: // 180° - L cabeza abajo
        table[Xc+1][Yc] ^= 1;
        table[Xc-1][Yc] ^= 1;
        table[Xc-1][Yc-1] ^= 1;
        break;
    case 3: // 270° - L acostada invertida
        table[Xc][Yc-1] ^= 1;
        table[Xc][Yc+1] ^= 1;
        table[Xc-1][Yc+1] ^= 1;
        break;
    }
}



