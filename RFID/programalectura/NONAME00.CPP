#include "stdio.h"
#include "conio.h"
#include "string.h"
#include "iostream.h"
#include "stdlib.h"
#include <fstream>
#include <iomanip>
#include <ctype>

void archivos();
void registrar();
void mostrar();
void modificar();
void metodo();
void mergesort(int *v, int i, int f);
void merge(int *v, int i, int m, int f);

void main ()
{
clrscr();
int op;
gotoxy(5,8);
cprintf("UNIVERSIDAD DE LOS LLANOS");
gotoxy(5,9);
cprintf("INGENIERIA ELECTRONICA");
gotoxy(5,10);
cprintf("Miguel Angel Mejia Sanchez");
gotoxy(5,11);
cprintf("CODIGO:161002921");
gotoxy(5,12);
cprintf("Ivan Santiago Rendon Gutierrez");
gotoxy(5,13);
cprintf("CODIGO:161002929");
gotoxy(5,14);
cprintf("MENU");
do
{
gotoxy(6,20);
cprintf("1.Manejo de Archivos");
gotoxy(6,21);
cprintf("2.Metodo Merge sort");
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


void metodo()
{
clrscr ();
int *v, n;
printf("Ingrese N: ");
scanf("%d", &n);

v = new int[n];
for(int i=0; i<n; i++)
{
printf("Elemento %d: ", i+1);
scanf("%d", &v[i]);
}

mergesort(v, 0, n-1);

for(int i=0; i<n; i++)
printf("%d ", v[i]);

}

void mergesort(int *v, int i, int f) {
if(i!=f) {
int m = (i+f)/2;
mergesort(v, i, m);
mergesort(v, m+1, f);
merge(v, i, m, f);
}
}

void merge(int *v, int i, int m, int f) {
int *aux = new int[m-i+1];
for(int j=i; j<=m; j++)
aux[j-i] = v[j];

int c1=0, c2=m+1;
for(int j=i; j<=f; j++) {
if(aux[c1] < v[c2]) {
v[j] = aux[c1++];
if(c1==m-i+1)
for(int k=c2; k<=f; k++)
v[++j] = v[k];
}
else {
v[j] = v[c2++];
if(c2==f+1)
for(int k=c1; k<=m-i; k++)
v[++j] = aux[k];
}
}
}
