#include "stdio"
#include "conio"
#include "string"
#include "iostream"
#include "stdlib"
#include <fstream>
#include <iomanip>
#include <ctype>

void archivos();
void registrar();
void mostrar();
void modificar();
void metodo();

void main ()
{
clrscr();
int op;
gotoxy(5,14);
cprintf("MENU");
do
{
gotoxy(6,20);
cprintf("1.Manejo de Archivos");
gotoxy(6,22);
cprintf("3.Salir");
cscanf("%d",&op);
switch(op)
{

case 1:{gotoxy(6,23);cprintf("Manejo de Archivos"); archivos ();break;}

case 2:{gotoxy(6,24);cprintf("Metodo Merger Sort"); metodo (); break;}

default:{cprintf("Opcion No Valida");getch ();getch();clrscr();clrscr();}

}

}
while(op!=3);
}

void archivos()
{
clrscr ();

char op;

do {
gotoxy (30,10);printf ("A.RIGISTRAR\n");
gotoxy (30,11);printf ("B.MOSTRAR\n");
gotoxy (30,12);printf ("C.MODIFICAR EL CORREO DEL ESTUDIANTE\n");
gotoxy (30,13);printf ("D.SALIR\n");

do{do{op=toupper(getch());}while(!isalpha(op));}while (op<'A'||op>'E');

switch (op)
{
case 'A':{registrar ();break;}
case 'B':{mostrar ();break;}
case 'C':{modificar();break;}

}

}while (op!='D');
}


void registrar ()
{
char op, nombre [50], correo [50];
int codigo, telefono, edad;

ofstream entrada;
entrada.open ("estudiantes.txt",ios::out|ios::app);
if (entrada.fail())
{printf ("error al abrir el archivo\a");getch ();}
else {
do {
clrscr();
gotoxy (30,4);printf ("INTRODUZCA LOS DATOS");
gotoxy (30,10);printf ("NOMBRE DEL ESTUDIANTE: ");
gets(nombre);
gotoxy (30,11);printf("CODIGO: ");
scanf("%d",&codigo);
gotoxy (30,12);printf("TEL. FIJO: ");
scanf("%d",&telefono);
gotoxy (30,13);printf("EDAD: ");
scanf("%d",&edad);
gotoxy (30,14);printf ("CORREO ELECTRONICO: ");
gets (correo);gets (correo);


entrada<<nombre<<" "<<codigo<<" "<<telefono<<" "<<edad<<" "<<correo<<endl;


gotoxy (30,18);printf ("¿DESEA INGRESAR DATOS DE OTRO ESTUDIANTE?\a");
do{op=toupper(getch());}while(!isalpha(op));
}
while(op!='N');

entrada.close();
}
getch();clrscr();
}

void mostrar ()
{
char nombre [50], correo [50];
int codigo, telefono, edad;

ifstream salida;

salida.open ("estudiantes.txt",ios::in);
if (salida.fail()){clrscr(); printf("la Apertura Del Archivo Fallo"); getch();}
else {clrscr();
salida>>nombre;
while(!salida.eof()){ salida>>codigo>>telefono>>edad>>correo;
printf ("\nNOMBRE DEL ESTUDIANTE: %s",nombre);
printf("\nCODIGO: %d",codigo);
printf("\nTELEFONO: %d",telefono);
printf("\nEDAD: %d",edad);
printf ("\nCORREO ELECTRONICO: %s \n",correo);
salida>>nombre;}
salida.close();
}
getch();clrscr();}

void modificar ()
{ clrscr();
char nombre [50], correo [50];
int auxcod, codigo, telefono, edad;
ofstream temp;
temp.open("temp.txt",ios::out);
ifstream salida;
salida.open ("estudiantes.txt",ios::in);
if (temp.fail()||salida.fail()){clrscr();printf("El Archivo No Se Pudo Abrir correctamente");}

else{clrscr();
gotoxy (20,10); printf("INGRESE EL CODIGO DEL ESTUDIANTE: ");
scanf("%d",& auxcod);
salida>>nombre;
while (!salida.eof()){salida>>codigo>>telefono>>edad>>correo;

if (codigo==auxcod){clrscr();gotoxy (15,15);printf("INGRESE EL NUEVO CODIGO: ");gets (correo);gets (correo); }
temp<<nombre<<" "<<codigo<<" "<<telefono<<" "<<edad<<" "<<correo<<endl;
salida>>nombre;}

}
temp.close();
salida.close();
remove("estudiantes.txt");
rename("temp.txt","estudiantes.txt") ;
clrscr();
}
