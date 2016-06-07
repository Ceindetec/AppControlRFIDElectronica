#include <MFRC522.h>
#include <SPI.h>
#include <string.h>
#include <RFID.h>
void setup()
{
  Serial.begin(115200); //Inicializa la velocidad de Serial
  SPI.begin();  //Función que inicializa SPI
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

int tar = 8;
int res = 7;
int Autorizado1[5] = {0x50, 0x8E, 0x80, 0xE5, 0xBB};  //"Lorena"
int Autorizado2[5] = {0xD4, 0x79, 0x6F, 0x16, 0xD4};  //"Cesar"
int Autorizado3[5] = {0xF3, 0xE2, 0xCC, 0xF7, 0x2A};  //"Hugo"
int Autorizado4[5] = {0x82, 0xA0, 0x85, 0xE5, 0x42};  //"German"
int Autorizado5[5] = {0xCC, 0x8F, 0x85, 0xE5, 0x23};  //"Miguel" 
int Autorizado6[5] = {0x76, 0x3F, 0x85, 0xE5, 0x29};  //"Mike"
int Autorizado7[5] = {0x61, 0x7d, 0x80, 0xE5, 0x79};  //"Daniel"
int tarjeta[5];
int Lorena;
int Cesar;
int Hugo;
int German;
int Miguel;
int Mike;
int Daniel;

void loop()
{
  RFID rfid(tar,res);
  rfid.init();      //Función  que inicializa RFID
  if (rfid.isCard())
  {
    if (rfid.readCardSerial())
    {
      Lorena = 0;
      Cesar = 0;
      Hugo = 0;
      German = 0;
      Miguel = 0;
      Mike = 0;
      Daniel = 0;
      for(int i=4; i>=0; i--)
      {
        tarjeta[i] = rfid.serNum[i];
//        delay(100);
        if(tarjeta[i] == Autorizado1[i])
        {
          Lorena++;
//          Serial.println(Lorena);
        }
        if(tarjeta[i] == Autorizado2[i])
        {
          Cesar++;
//          Serial.println(Lorena);
        }
        if(tarjeta[i] == Autorizado3[i])
        {
          Hugo++;
//          Serial.println(Lorena);
        }
        if(tarjeta[i] == Autorizado4[i])
        {
          German++;
//          Serial.println(Lorena);
        }
        if(tarjeta[i] == Autorizado5[i])
        {
          Miguel++;
//          Serial.println(Lorena);
        }
        if(tarjeta[i] == Autorizado6[i])
        {
          Mike++;
//          Serial.println(Lorena);
        }
        if(tarjeta[i] == Autorizado7[i])
        {
          Daniel++;
//          Serial.println(Lorena);
        }
      }
      if (tar == 10 && res == 5)      //Puerta entrada
      {
        if (Lorena == 5)
        {
          Serial.println("CEINDETEC Llanos ");
          Serial.println("Bienvenida de nuevo Lorena");
          Serial.println("");
        }
        else if (Cesar == 5)
        {
          Serial.println("CEINDETEC Llanos");
          Serial.println("Bienvenido de nuevo Cesar");
          Serial.println("");
        }
        else if (Hugo == 5)
        {
          Serial.println("CEINDETEC Llanos");
          Serial.println("Bienvenido de nuevo Hugo");
          Serial.println("");
        }
        else if (German == 5)
        {
          Serial.println("CEINDETEC Llanos");
          Serial.println("Bienvenido de nuevo German");
          Serial.println("");
        }
        else if (Miguel == 5)
        {
          Serial.println("CEINDETEC Llanos");
          Serial.println("Bienvenido de nuevo Miguel");
          Serial.println("");
        }
        else if (Mike == 5)
        {
          Serial.println("CEINDETEC Llanos");
          Serial.println("Bienvenido de nuevo Mike");
          Serial.println("");
        }
        else if (Daniel == 5)
        {
          Serial.println("CEINDETEC Llanos");
          Serial.println("Bienvenido de nuevo Daniel");
          Serial.println("");
        }
        else
        {
          Serial.println("CEINDETEC Llanos");
          Serial.println("Acceso restringido para este usuario");
          Serial.println("");
        }
      }
      else if (tar == 9 && res == 6)  //Puerta laboratorio
      {
        if (Lorena == 5)
        {
          Serial.println("Laboratorio de Electronica");
          Serial.println("Bienvenida de nuevo Lorena");
          Serial.println("");
        }
        else if (Cesar == 5)
        {
          Serial.println("Laboratorio de Electronica");
          Serial.println("Bienvenido de nuevo Cesar");
          Serial.println("");
        }
        else if (Hugo == 5)
        {
          Serial.println("Laboratorio de Electronica");
          Serial.println("Bienvenido de nuevo Hugo");
          Serial.println("");
        }
        else if (German == 5)
        {
          Serial.println("Laboratorio de Electronica");
          Serial.println("Bienvenido de nuevo German");
          Serial.println("");
        }
        else if (Miguel == 5)
        {
          Serial.println("Laboratorio de Electronica");
          Serial.println("Bienvenido de nuevo Miguel");
          Serial.println("");
        }
        else
        {
          Serial.println("Laboratorio de Electronica");
          Serial.println("Acceso restringido para este usuario");
          Serial.println("");
        }
      }
      else if (tar == 8 && res == 7)  //Puerta gerencia
      {
        if (Lorena == 5)
        {
          Serial.println("Oficina de Gerencia");
          Serial.println("Bienvenida de nuevo Lorena");
          Serial.println("");
        }
        else if (Cesar == 5)
        {
          Serial.println("Oficina de Gerencia");
          Serial.println("Bienvenido de nuevo Cesar");
          Serial.println("");
        }
        else if (Hugo == 5)
        {
          Serial.println("Oficina de Gerencia");
          Serial.println("Bienvenido de nuevo Hugo");
          Serial.println("");
        }
        else if (German == 5)
        {
          Serial.println("Oficina de Gerencia");
          Serial.println("Bienvenido de nuevo German");
          Serial.println("");
        }
        else
        {
          Serial.println("Oficina de Gerencia");
          Serial.println("Acceso restringido para este usuario");
          Serial.println("");
        }
      }
    }
  }
  if (tar == 10 && res == 5)      //Puerta entrada
  {
    tar = 9;
    res = 6;
  }
  else if (tar == 9 && res == 6)  //Puerta laboratorio
  {
    tar = 8;
    res = 7;
  }
  else if (tar == 8 && res == 7)  //Puerta gerencia
  {
    tar = 10;
    res = 5;
  }
  delay(100);
}
