#include <iostream>

using namespace std;
void printtable(unsigned int col,unsigned int fil,char **table);
int main()
{
    unsigned int col,fil;
    char name[20];
    cout<<"Bienvenido al video juego"<<endl;
    cout<<"Ingresa tu nombre: "<<endl;
    cin>>name;
    cout<<"Ingresa las Dimensiones del tetrtis"<<endl;
    cout<<"Ingresa la cantidad de columnas: ";
    cin>>col;
    cout<<"Ingresa la cantidad de filas: ";
    cin>>fil;

    char **table = new char*[col];        //reserva de las dimensiones del tablero
    for(unsigned int i=0;i<col;i++){
        table[i]=new char[fil];
        for (unsigned int j=0;j<fil;j++){
            table[i][j]=0;
        }
    }

    printtable(col,fil,table);
    for(unsigned int i=0;i<col;i++){
        delete []table[i];
    }
    delete []table;


}


// funciones del videojuego
void printtable(unsigned int col,unsigned int fil,char **table){
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
