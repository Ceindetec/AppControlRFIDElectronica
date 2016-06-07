#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>
#include <ctime>

using namespace std;

//ESTO SE USA PARA DODER DEFINIR EL POSICIONAMIENTO DEL CURSOR EN LA PANTALLA
COORD coord = {0,0};

void gotoxy(int x,int y){
coord.X=x;
coord.Y=y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

class tienda{
    public:
    char prod [20];
    int precio;
    protected:
    virtual void escribir()=0;
    virtual void mostrar()=0;
};

class producto:public tienda{
    public:
    int codigo;
    int cantidad;
    public:
 void escribir();
 void mostrar();
 void modificar ();
 void vender ();
 };

void producto::escribir(){                                       //pide los datos
 char a;
 char cadena[3128];
char linea[50][3000];

ifstream fs("INVENTARIO TIENDA UNILLANOS.txt");
ofstream p("prueba.txt");

 		do{
        printf("DATOS DEL PRODUCTO\n");
        printf("NOMBRE:");gets(prod);gets(prod);                                    //si en el nombre se detecta numero vuelve a pedir nombre
        printf("CODIGO:");cin>>codigo;
        printf("PRECIO:");cin>>precio;
        printf("CANTIDAD:");cin>>cantidad;

        while (!fs.eof()){
        fs.getline(cadena,3000);
        p << cadena << endl; // añadido: escribo.
        }

            p << "  "<<codigo <<"    "<< prod<<"    "<< precio<<"    "<<cantidad <<endl;
            fs.close();
            p.close();
            ofstream fs("INVENTARIO TIENDA UNILLANOS.txt");
            ifstream p ("prueba.txt");
        while (!p.eof()){
        p.getline(cadena,3000);
        fs << cadena << endl; // añadido: escribo.
        }
        fs.close();
        p.close();
        printf("\nDESEA INGRESAR OTRO REGISTRO? S/N");
        cin>>a;
        system ("cls");
        }while((a=='s')||(a=='S'));

}

void producto::mostrar(){
ifstream fe("INVENTARIO TIENDA UNILLANOS.txt");
char cadena[3128];
char linea[50][3000];
string texto;
string lineacadena;
int a=0;
int b=0;
cout <<"-------------------------------------\n";
cout <<"\nCOD.  NOMBRE   PRECIO  CANTIDAD\n";
cout <<"-------------------------------------\n";
a=-1;
while (!fe.eof()){
fe.getline(cadena,3000);
a++;
string lineacadena (cadena); // añadido: convierto array de char a cadena
texto +=lineacadena + '\n';  // añado a la cadena texto.
strcat(linea[a],cadena); //añadido trabajando con cadenas
cout << cadena << endl; // añadido: escribo.
}


   }

void producto::modificar(){
    fstream fe("INVENTARIO TIENDA UNILLANOS.txt");
    ofstream p("prueba.txt");
    char g;
    char cadena[3128],nom[20];
    char linea[50][3000],c[2];
    string texto;
    string lineacadena;
    int a=0,cod,cod1,pre,can;
    int b=0;
    do {
    cout <<"INGRESE EL CODIGO DEL PRODUCTO QUE DESEA MODIFICAR: ";cin>>cod;
if (cod<=9){c[0]='0';c[1]=cod+48;}
else {c[0]=(cod/10)+48;
      c[1]=(cod%10)+48;  }
    a=-1;
    while (!fe.eof()){
    fe.getline(cadena,3000);
    a++;
    string lineacadena (cadena); // añadido: convierto array de char a cadena
    texto +=lineacadena + '\n';  // añado a la cadena texto.
    strcat(linea[a],cadena); //añadido trabajando con cadenas
        if (cadena[2]==c[0]&&cadena [3]==c[1]){/////////////////////////////////////////////////////

        printf("DATOS DEL PRODUCTO\n");
        printf("NOMBRE:");gets(nom);gets(nom);                                    //si en el nombre se detecta numero vuelve a pedir nombre
        printf("CODIGO:");cin>>cod1;
        printf("PRECIO:");cin>>pre;
        printf("CANTIDAD:");cin>>can;
         p << "  "<<cod1 <<"    "<< nom<<"    "<< pre<<"    "<<can<<endl;

        }////////////////////////////////////////////////////////////////////////////////////////
    else {p<<cadena<<endl;}
    }p.close();
    fe.close();
     ofstream fs("INVENTARIO TIENDA UNILLANOS.txt");
     ifstream p ("prueba.txt");
        while (!p.eof()){
        p.getline(cadena,3000);
        fs << cadena << endl; // añadido: escribo.
        }
        fs.close();
        p.close();
        cout<<"desea hacer otro cambio si (s) o no (n)? :"; cin>>g;
        }while(g=='s'||g=='S');
}

void producto::vender(){
    char g,a1;
    char cadena[3128],c[2];
    int cod,pre,can,venta=0,venxdia=0;
    int dia,mes,ano;
    time_t tSac= time (NULL);
    struct tm* tmP=localtime(&tSac);
    dia=tmP->tm_mday;
    mes=tmP->tm_mon+1;
    ano=tmP->tm_year+1900;
do{
    do {
    fstream fe("INVENTARIO TIENDA UNILLANOS.txt");
    cout <<"INGRESE EL CODIGO DEL PRODUCTO: ";cin>>cod;
    if (cod<=9){c[0]='0';c[1]=cod+48;}
    else {c[0]=(cod/10)+48;c[1]=(cod%10)+48;}
    while (!fe.eof()){
    fe.getline(cadena,3000);

    if (cadena[2]==c[0]&&cadena [3]==c[1]){/////////////////////////////////////////////////////
        cout<<cadena<<endl;
        printf("INGRESE EL PRECIO Y LA CANTIDAD:\n");
        printf("PRECIO:");cin>>pre;
        printf("CANTIDAD:");cin>>can;
        venta=(pre*can)+venta;
        printf("EL TOTAL DE COMPRA FUE DE : %d\n\n",venta);
        }////////////////////////////////////////////////////////////////////////////////////////

    }
    fe.close();

        cout<<"desea hacer otra compra si (s) o no (n)? :"; cin>>g;
        }while(g=='s'||g=='S');
venxdia=venta+venxdia;
venta=0;
        ////////////////////////////////////////////////////
         ifstream fr ("ventas x dia.txt");
         ofstream p("prueba2.txt");
        while (!fr.eof()){
        fr.getline(cadena,3000);
        p << cadena << endl; // añadido: escribo.
        }

        p<<venxdia<<"   "<<dia<<"   "<<mes<<"   "<<ano<<endl;

        p.close();
        fr.close();

        ofstream fe("ventas x dia.txt");
        ifstream p1("prueba2.txt");
         while (!p1.eof()){
        p1.getline(cadena,3000);
        fe << cadena << endl; // añadido: escribo.
        }
         fe.close();
         p1.close ();
        system("cls");
        cout<<"otro cliente si (s) o no (n):";cin>>a1;
   } while  (a1=='s'||a1=='S');      ////////////////////////////////////////////////////


}

class menu{
    public:
 virtual void muestramenu()=0;
};

class menu_principal:public menu{
    public:
    void muestramenu();
};

void menu_principal::muestramenu(){
 system ("cls");
 gotoxy(18,3);printf("===========================================");
 gotoxy(18,4);printf("||            TIENDA UNILLANOS             ||");
 gotoxy(18,5);printf("===========================================");
 gotoxy(18,6);printf("||      1. INGRESO ADMINISTRADOR         ||");
 gotoxy(18,7);printf("||      2. INGRESO CAJERO                ||");
 gotoxy(18,8);printf("||      3.SALIR                          ||");
 gotoxy(18,9);printf("||                                       ||");
 gotoxy(18,10);printf("==========================================");
 gotoxy(18,11);printf("||_______________________________________||");
 gotoxy(18,12);printf("ELIJA SU OPCION:");
}

class menu_administrador:public menu{
 public:
 void muestramenu();
 };

void menu_administrador::muestramenu(){
 system ("cls");
 gotoxy(18,3);printf("===========================================");
 gotoxy(18,4);printf("||           MENU ADMINISTRADOR          ||");
 gotoxy(18,5);printf("===========================================");
 gotoxy(18,6);printf("||      1. INGRESAR PRODUCTOS            ||");
 gotoxy(18,7);printf("||      2. VER INVENTARIO                ||");
 gotoxy(18,8);printf("||      3. MODIFICAR DATOS               ||");
 gotoxy(18,9);printf("||      4. VER PROOVEDORES               ||");
 gotoxy(18,10);printf("||     5.SALIR                          ||");
 gotoxy(18,11);printf("===========================================");
 gotoxy(18,12);printf("||______________________________________ ||");
 gotoxy(18,13);printf("ELIJA SU OPCION:");
}


int main(){
 int op,op1;
char a,a1, a2, a3;
  producto pr;
  menu_principal mp;
  menu_administrador ma;

do{
        mp.muestramenu();
        cin>>op;
        switch(op){


           case 1:
           do{
            ma.muestramenu();
            cin>>op1;

                    switch (op1)
                {

                case 1:
                pr.escribir();
                break;

                case 2:
                pr.mostrar();
                cout<<"oprima una tecla y enter para continuar";
                cin>>a3;
                system ("cls");
                a2='s';
                break;

                case 3:
                pr.modificar();
a2='s';
                break;

                case 4:
a2='s';
                break;

                case 5:
                a2='n';
                break;

                default:
                cout<<"opcion invalida"<<endl;
                break;
               }

               }while(a2=='s'||a2=='S');
               a1='s';

           break;

            case 2:
            system("cls");
            pr.vender ();
            a1='s';
            break;

            case 3:
            a1='n';
            break;

            default :
            cout<<"opcion invalida"<<endl;
            break;
            }
}while(a1=='s'||a1=='S');
}
