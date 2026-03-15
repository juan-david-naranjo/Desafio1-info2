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
bool actficha(unsigned int &Xc, unsigned int &Yc, char**table,char &state,unsigned int &rot);        //actualiza el estado de la ficha


int main()
{
    srand(time(0));
    unsigned int col,fil;
    bool exit=true;
    char name[20];

    unsigned int rotacion=0;
    // codigo principal del juego
    while(exit){
        printinicio();
        limpiarConsola();


        cout<<"Ingresa tu nombre de jugador: "<<endl;
        cin>>name;
        cout<<"Ingresa las Dimensiones del tetrtis (multiplos de 8)"<<endl;



        //validar dimensiones sean multiplos de 8
        cout<<"Ingresa la cantidad de columnas: ";
        cin>>col;
        cout<<"Ingresa la cantidad de filas: ";
        cin>>fil;


        //reserva de las dimensiones del tablero
        if(valido(col)&&valido(fil)){
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
            exit=false;
        }
        exit=false;








    }





}


// funciones del tetris
void printtable(unsigned int col,unsigned int fil,char **table){
    limpiarConsola();
    for (int i = 0; i < col; i++) {
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
        cout<<"Ingresaste W"<<endl;
       return 'd';
    case 's':
    case 'S':
        cout<<"Ingresaste W"<<endl;
        return 's';
    case 'w':
    case'W':
        cout<<"Ingresaste W"<<endl;
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
    unsigned short int form=0;
    char state;
    unsigned int rot=0;
    unsigned int mid=(fil/2)-1;
    x=0;
    y=mid;
    switch (form) {
    case 0:
        cout<<"pieza I"<<endl;  //al inicio las coordenadas de la ficha es la mitad

        for(unsigned short int i=0;i<3;i++){
            state=updtable();
            if(actficha(x,y,table,state,rot)){
                table[x][y]=1;
                printtable(col,fil,table);}
        }

        break;
    case 1:
        cout<<"pieza 2x2"<<endl;
        for(unsigned short int i=0;i<2;i++){
            table[i][mid]=1;
            table[i][fil+1]=1;
            printtable(col,fil,table);
        }
        break;

    case 2:
        cout<<"pieza L inversa"<<endl;
        for(unsigned short int i=0;i<3;i++){
            table[i][fil]=1;
            if(i==0){table[i][fil-1]=1;}
            printtable(col,fil,table);
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
            table[i][11];
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


bool actficha(unsigned int &Xc, unsigned int &Yc, char**table,char &state,unsigned int &rot){
    //Xcentro es la coordenada central en x de la ficha<--->Ycentro es la coordenada central en y de la ficha
    switch(state) {
    case 'a':
        Yc-=1;

        return true;
    case 's':
        Xc+=1;

        return true;
    case 'd':
        Xc+=1;

        return true;
    case 'w':
        rot+=1;

        return true;
    case 'q':

        return false;
    default:
        return false;
    }





}

