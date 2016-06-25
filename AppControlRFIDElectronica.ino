///////////////////////////////////////////////////////////////////////////
// LIBRERIAS DEL PROYECTO /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <SPI.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <TimeLib.h>
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// DEFINICION DE PINES Y VARIABLES ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#define RST_PIN  16    //Pin 9 para el reset del RC522
#define SS_PIN  15   //Pin 10 para el SS (SDA) del RC522
#define Programacion 5
#define Puerta 16
#define USE_SERIAL Serial

MFRC522 INS_MFRC522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
WebSocketsClient INS_WebSocketsClient;

//agregue comentario
boolean Address;
long Buscar_espacio_vacio();
long Espacio;
byte ActualUID[4]; //almacenará el código del Tag leído
byte Master[4] = {0x8B, 0x23, 0x80, 0xE5} ; //código de la tarjeta Master
byte identificacion[4];
byte usuario[4];
uint32_t Id;
unsigned long xy;
String UID;

StaticJsonBuffer<500> jsonBuffer;
StaticJsonBuffer<1000> jsonBufferSocket;

const char* ssid     = "CEINDETEC2";
const char* password = "ceindetec123*";
const char* ip_websocket = "192.168.0.245";
const int port_websocket = 8082;
char Data[512];
char aux[4];

String mensajedatancritado;
double claveMochila[] = {335916, 428891, 866985, 2139945, 4385521, 8713045, 17162809, 34033702};
time_t prevDisplay = 0;
int reset = 0;

//#define DEBUG 1

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// DEFINICION DE METODOS //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void Modo_Programacion();
void Guardar_usuario(byte ActualUID[4], long Direccion);
boolean Buscar_usuario(byte ActualUID[4]);
void Borrar_usuario(long Direccion);
boolean compareArray(byte array1[], byte array2[]);
unsigned long ArrayToDecimal(byte array1[]);
void CharToByte(const char* tarjeta);
void borar_memoria_EEPROM ();
String encriptar(String msn_orgin);
String desencriptar (char* mensaje);
void  getNtpTime();
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// METODO WEBSOCKECTEVENT QUE MANEJA LOS EVENTOS DEL WEBSOCKET ////////////
///////////////////////////////////////////////////////////////////////////

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {

#ifdef DEBUG
  USE_SERIAL.printf("[SOCKE] **********************************\n");
  USE_SERIAL.printf("[SOCKE] TIPO: %d\n", type);
  USE_SERIAL.printf("[SOCKE] PAYLOAD: %s\n", payload);
  USE_SERIAL.printf("[SOCKE] LONGITUD: %d\n", lenght);
#endif

  switch (type) {

    case WStype_DISCONNECTED: {
#ifdef DEBUG
        USE_SERIAL.printf("[SOCKE] EVENTO DESCONEXION\n");
#endif

        break;
      }
    case WStype_CONNECTED: {
        JsonObject& root = jsonBuffer.createObject();
        root["dispositivo"] = "aaaaaaaa";
        root["accion"] = "presentacion";
        root["data"] = "";
        root.printTo(Data, sizeof(Data));

        mensajedatancritado = encriptar((String)Data);


        mensajedatancritado.replace(".00", "");
        char charBufData[mensajedatancritado.length() + 1];
        mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);

        INS_WebSocketsClient.sendTXT(charBufData);

        for ( int i = 0; i < strlen(charBufData);  ++i ) {
          charBufData[i] = (char)0;
        }
        mensajedatancritado = "";

        jsonBuffer = StaticJsonBuffer<500>();
        for ( int i = 0; i < sizeof(Data);  ++i ) {
          Data[i] = (char)0;
        }
        // send message to server when Connected
        //INS_WebSocketsClient.sendTXT("Connected");
        delay(100);
        getNtpTime();
        break;
      }
    case WStype_TEXT: {

#ifdef DEBUG
        USE_SERIAL.printf("[SOCKE] EVENTO MENSAJE\n");
#endif

        //StaticJsonBuffer<500> jsonBufferSocket;
        char* palabra;
        palabra = (char*) payload;

        String mensajeservidor = desencriptar(palabra);

        char charBufServer[mensajeservidor.length() + 1];

        mensajeservidor.toCharArray(charBufServer, mensajeservidor.length() + 1);

        JsonObject& rootSocket = jsonBufferSocket.parseObject(charBufServer);

        const char* dispositivo    = rootSocket["dispositivo"];
        const char* accion    = rootSocket["accion"];

        if (strcmp(accion, "DEL") == 0) {
          const char* data    = rootSocket["data"];
          CharToByte(data);
          if (Buscar_usuario(identificacion) == true) {
#ifdef DEBUG
            USE_SERIAL.println(Address);
            USE_SERIAL.println("Usuario eliminado del sistema");
#endif
            Borrar_usuario(Address);
          }
        }

        if (strcmp(accion, "INS") == 0) {
          const char* data    = rootSocket["data"];
          CharToByte(data);

          if (Buscar_usuario(identificacion) == false) {
            Espacio = Buscar_espacio_vacio();
            USE_SERIAL.println(Espacio);
            if (Espacio == 1025) {
#ifdef DEBUG
              USE_SERIAL.println("[MDPRG] No hay espacio en memoria");
#endif
            }
            else {
              Guardar_usuario(identificacion, Espacio);
              EEPROM.commit();
#ifdef DEBUG
              USE_SERIAL.println("[MDPRG] El nuevo usuario ha sido registrado");
#endif
            }
          }
          else {
#ifdef DEBUG
            USE_SERIAL.println("[MDPRG] El usuario ya se encuentra registrado");
#endif
          }
        }

        if (strcmp(accion, "UPD") == 0) {
          long longitud_data    = rootSocket["total"];
          long iteracion = 0;
          borar_memoria_EEPROM ();
          for (long i = 0; i < longitud_data; i++) {
            CharToByte(rootSocket["data"][i]);
            Guardar_usuario(identificacion, iteracion);
            iteracion = iteracion + 4;
          }
          EEPROM.commit();
#ifdef DEBUG
          USE_SERIAL.println("La actualizacion fue exitosa");
#endif
        }

        if (strcmp(accion, "HRO") == 0) {

#ifdef DEBUG
          USE_SERIAL.println("ESTA ES LA HORA");
          USE_SERIAL.println(mensajeservidor);
#endif
          setTime((int)rootSocket["data"][0], (int)rootSocket["data"][1], (int)rootSocket["data"][2], (int)rootSocket["data"][3], (int)rootSocket["data"][4], (int)rootSocket["data"][5]);
          prevDisplay = now();
          reset = 1;
          USE_SERIAL.println(prevDisplay);
        }

        if (strcmp(accion, "PER") == 0) {

          digitalWrite( Puerta , LOW );

          delay(1000);

          digitalWrite( Puerta , HIGH );

#ifdef DEBUG
          USE_SERIAL.println("SE LE DIO PERMISO");
#endif

        }
        jsonBufferSocket = StaticJsonBuffer<1000>();
         for ( int i = 0; i < sizeof(charBufServer);  ++i ) {
          charBufServer[i] = (char)0;
        }
        break;
      }
    case WStype_BIN: {

        USE_SERIAL.printf("[SOCKE] Mensaje binario obtenido: %u\n", lenght);
        hexdump(payload, lenght);

        // send data to server
        // INS_WebSocketsClient.sendBIN(payload, lenght);

        break;
      }
  }
#ifdef DEBUG
  USE_SERIAL.printf("[SOCKE] **********************************\n");
#endif

}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// METODO SETUP ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void setup() {

  pinMode(16, OUTPUT);

  USE_SERIAL.begin(115200); //Iniciamos La comunicacion serial

  SPI.begin();        //Iniciamos el Bus SPI

  INS_MFRC522.PCD_Init(); // Iniciamos el MFRC522

  EEPROM.begin(1024);

#ifdef DEBUG
  USE_SERIAL.println("[SETUP] Control de acceso:");
#endif

  delay(1000);

  digitalWrite( Puerta , HIGH );

  // USE_SERIAL.setDebugOutput(true);

  for (uint8_t t = 4; t > 0; t--) {
#ifdef DEBUG
    USE_SERIAL.printf("[SETUP] ESPERANDO INICIO %d...\n", t);
#endif
    USE_SERIAL.flush();
    delay(1000);
  }

#ifdef DEBUG
  USE_SERIAL.printf("[SETUP] Conectando a ");
  USE_SERIAL.printf(ssid);
#endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    USE_SERIAL.printf(".");
  }

#ifdef DEBUG
  USE_SERIAL.println("");

  USE_SERIAL.printf("[SETUP] WiFi conectado ");
  USE_SERIAL.printf("IP address: ");
  USE_SERIAL.println(WiFi.localIP());
#endif

  INS_WebSocketsClient.begin(ip_websocket, port_websocket);
  INS_WebSocketsClient.onEvent(webSocketEvent);

}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void loop() {
  time_t t = now();
  INS_WebSocketsClient.loop();

  //USE_SERIAL.println(WStype_TEXT);

  // Revisamos si hay nuevas tarjetas  presentes
  if ( INS_MFRC522.PICC_IsNewCardPresent()) {

    //Seleccionamos una tarjeta
    if ( INS_MFRC522.PICC_ReadCardSerial()) {
#ifdef DEBUG
      USE_SERIAL.println("[LOOP_] ********************************");
      // Enviamos serialemente su UID
      USE_SERIAL.printf("[LOOP_] Tarjeta leida UID:");
#endif
      for (byte i = 0; i < INS_MFRC522.uid.size; i++) {

        USE_SERIAL.print(INS_MFRC522.uid.uidByte[i] < 0x10 ? " 0" : " ");

        USE_SERIAL.print(INS_MFRC522.uid.uidByte[i], HEX);

        ActualUID[i] = INS_MFRC522.uid.uidByte[i];

      }

      USE_SERIAL.println("");

      //comparamos los UID para determinar si es uno de nuestros usuarios
      if (compareArray(ActualUID, Master)) {

#ifdef DEBUG
        USE_SERIAL.println("[LOOP_] Modo de programacion activo");
#endif

        digitalWrite( Programacion , HIGH );

        Modo_Programacion();

      }

      else {
#ifdef DEBUG
        USE_SERIAL.println("[LOOP_] Activacion de actuador");
#endif
        digitalWrite( Puerta , HIGH );
      }

      if (Buscar_usuario(ActualUID) == false) {
#ifdef DEBUG
        USE_SERIAL.println("[LOOP_] El usuario no existe");
#endif
        Id = ArrayToDecimal(ActualUID);

        UID = String(Id, HEX);

        JsonObject& root = jsonBuffer.createObject();

        root["dispositivo"] = "RFID";

        root["accion"] = "invitado";


        root["data"] = UID;

        root.printTo(Data, sizeof(Data));

#ifdef DEBUG
        USE_SERIAL.printf("[LOOP_] DATA: %s\n", Data);
#endif

        mensajedatancritado = encriptar((String)Data);

        mensajedatancritado.replace(".00", "");
        char charBufData[mensajedatancritado.length() + 1];
        mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);

        INS_WebSocketsClient.sendTXT(charBufData);

        for ( int i = 0; i < strlen(charBufData);  ++i ) {
          charBufData[i] = (char)0;
        }
        mensajedatancritado = "";

        jsonBuffer = StaticJsonBuffer<500>();
        for ( int i = 0; i < sizeof(Data);  ++i ) {
          Data[i] = (char)0;
        }

      } else {

#ifdef DEBUG
        USE_SERIAL.println("[LOOP_] Acceso concedido");
#endif

        Id = ArrayToDecimal(ActualUID);

        UID = String(Id, HEX);

        JsonObject& root = jsonBuffer.createObject();

        root["dispositivo"] = "RFID";

        root["accion"] = "acceso";


        root["data"] = UID;

        root.printTo(Data, sizeof(Data));

#ifdef DEBUG
        USE_SERIAL.printf("[LOOP_] DATA: %s\n", Data);
#endif

        mensajedatancritado = encriptar((String)Data);

        mensajedatancritado.replace(".00", "");
        char charBufData[mensajedatancritado.length() + 1];
        mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);

        INS_WebSocketsClient.sendTXT(charBufData);

        for ( int i = 0; i < strlen(charBufData);  ++i ) {
          charBufData[i] = (char)0;
        }
        mensajedatancritado = "";

        jsonBuffer = StaticJsonBuffer<500>();

#ifdef DEBUG
        USE_SERIAL.printf("[LOOP_] DATA FOR: %s\n", Data);
#endif

        digitalWrite( Puerta , LOW );

        delay(1000);

        digitalWrite( Puerta , HIGH );

        for ( int i = 0; i < sizeof(Data);  ++i ) {
          Data[i] = (char)0;
        }


      }

      digitalWrite( Programacion , LOW );

      //      // Terminamos la lectura de la tarjeta tarjeta  actual

      INS_MFRC522.PICC_HaltA();
      // USE_SERIAL.println("[LOOP_] ********************************");
    }

  }
  delay(100);
  if (now() >= prevDisplay+1200 && reset==1) {
    ESP.restart();
    reset=0;
  }

}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//////////////FUNCIÓN PARA GUARDAR Y BORRAR TARJETAS DE LA EEPROM///////////
///////////////////////////////////////////////////////////////////////////
void Modo_Programacion() {

  digitalWrite( Puerta , HIGH );

  while (true) {

    INS_MFRC522.PICC_HaltA();

    // Revisamos si hay nuevas tarjetas  presentes
    if ( INS_MFRC522.PICC_IsNewCardPresent()) {

      //Seleccionamos una tarjeta
      if ( INS_MFRC522.PICC_ReadCardSerial()) {

        // Enviamos serialemente su UID
#ifdef DEBUG
        USE_SERIAL.printf("[MDPRG] Card UID:");
#endif

        for (byte i = 0; i < INS_MFRC522.uid.size; i++) {

          USE_SERIAL.print(INS_MFRC522.uid.uidByte[i] < 0x10 ? " 0" : " ");

          USE_SERIAL.print(INS_MFRC522.uid.uidByte[i], HEX);

          ActualUID[i] = INS_MFRC522.uid.uidByte[i];

        }

        if (Buscar_usuario(ActualUID) == false) {

          Espacio = Buscar_espacio_vacio();

          USE_SERIAL.println(Espacio);

          if (Espacio == 1025) {
#ifdef DEBUG
            USE_SERIAL.println("[MDPRG] No hay espacio en memoria");
#endif

          }

          else {

            Guardar_usuario(ActualUID, Espacio);

#ifdef DEBUG
            USE_SERIAL.println("[MDPRG] El nuevo usuario ha sido registrado");
#endif

          }

        }

        else {

          Borrar_usuario(Address);
#ifdef DEBUG
          USE_SERIAL.println("[MDPRG] El usuario ha sido eliminado");
#endif

        }
      }


      INS_MFRC522.PICC_HaltA();

      digitalWrite( Programacion , LOW );

      return; // Terminamos la lectura de la tarjeta tarjeta  actual

    }

    delay(100);

  }

}
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
////////////////FUNCIÓN PARA GUARDAR USUARIOS EN LA EEPROM/////////////////
///////////////////////////////////////////////////////////////////////////
void Guardar_usuario(byte ActualUID[4], long Direccion) {
  for (long j = 0; j < 4; j++) {
    EEPROM.write(Direccion + j, ActualUID[j]);
  }
}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//////////////////FUNCIÓN PARA BORAR USUARIOS EN LA EEPROM/////////////////
///////////////////////////////////////////////////////////////////////////
void Borrar_usuario(long Direccion) {
  for (long j = 0; j < 4; j++) {
    EEPROM.write(Direccion + j, 0);
    EEPROM.commit();
  }
}
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
////////////////FUNCIÓN PARA BUSCAR USUARIOS EN LA EEPROM//////////////////
///////////////////////////////////////////////////////////////////////////
boolean Buscar_usuario(byte ActualUID[4]) {
  for (long j = 0; j <= 1020; j = j + 4) {
    if (EEPROM.read(j) == ActualUID[0]) {
      if (EEPROM.read(j + 1) == ActualUID[1]) {
        if (EEPROM.read(j + 2) == ActualUID[2]) {
          if (EEPROM.read(j + 3) == ActualUID[3]) {
            Address = j;
            return (true);
          }
        }
      }
    }
  }
  return (false);
}
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
/////////////FUNCIÓN PARA BUSCAR ESPACIOS VACIOS EN LA EEPROM//////////////
///////////////////////////////////////////////////////////////////////////
long Buscar_espacio_vacio() {
  for (long j = 0; j <= 1020; j = j + 4) {
    if (EEPROM.read(j) == 0) {
      if (EEPROM.read(j + 1) == 0) {
        if (EEPROM.read(j + 2) == 0) {
          if (EEPROM.read(j + 3) == 0) {
            USE_SERIAL.println(j);
            return (j);
          }
        }
      }
    }
  }
  return (1025);
}
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//////////////////////FUNCIÓN PARA COMPARAR 2 ARRAY////////////////////////
///////////////////////////////////////////////////////////////////////////
boolean compareArray(byte array1[], byte array2[])
{
  if (array1[0] != array2[0])return (false);
  if (array1[1] != array2[1])return (false);
  if (array1[2] != array2[2])return (false);
  if (array1[3] != array2[3])return (false);
  return (true);
}
///////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//////////////////FUNCION BORRAR TODOS LOS USUARIOS/////////////////////////
////////////////////////////////////////////////////////////////////////////

void borar_memoria_EEPROM () {
  for ( long i = 0; i < 1023; i++) {
    EEPROM.write(i, 0);
  }
  //EEPROM.commit();
}
///////////////////////////////////////////////////////////////////////////

//Función transformar un aray de 4 bytes en un uint32_t
unsigned long ArrayToDecimal(byte array1[])
{
  unsigned long Decimal = (array1[0] << 24) | (array1[1] << 16) | (array1[2] << 8) | (array1[3]);
  return (Decimal);
}
//////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
///////////////FUNCION PARA CONVERTIR UN CHAR A BYTE////////////////////////
////////////////////////////////////////////////////////////////////////////
void CharToByte(const char* tarjeta) {
  uint8_t iteracion = 0;
  uint8_t num1 = 0;
  uint8_t num2 = 0;

  for (int i = 0; i < 8; i = i + 2) {
    num1 = (uint8_t)tarjeta[i];
    num2 = (uint8_t)tarjeta[i + 1];
    if (num1 > 47 && num1 < 58) {
      num1 = num1 - 48;
    }
    else {
      num1 = num1 - 87;
    }
    if (num2 > 47 && num2 < 58) {
      num2 = num2 - 48;
    }
    else {
      num2 = num2 - 87;
    }
    identificacion[iteracion] = num2 + (num1 * 16);
    iteracion = iteracion + 1;
  }
  return;
}



////////////////////////////////////////////////////////////////////////////
///////////////FUNCION PARA ENCRIPTAR MENSAJES /////////////////////////////
////////////////////////////////////////////////////////////////////////////

String encriptar (String msn_orgin ) {

  char mensajeOriginal[msn_orgin.length() + 1];
  String mensajeEncriptado = "";
  String caracterBinarioCompleto;
  String cadenaCeros = "00000000";
  String complementoBinario;

  double auxiliarCaracterEncriptado;
  double auxdouble;

  // USE_SERIAL.println(msn_orgin);

  if (msn_orgin != "") {
    for (int m = 0; m < msn_orgin.length(); m++) {
      mensajeOriginal[m] = msn_orgin[m];
    }
    for ( int i = 0; i < sizeof(mensajeEncriptado);  ++i ) {
      mensajeEncriptado[i] = (char)0;
    }
    for (int i = 0 ; i < msn_orgin.length() ; i++ ) {

      complementoBinario = cadenaCeros.substring(0, 8 - String(mensajeOriginal[i], BIN).length());

      caracterBinarioCompleto = complementoBinario + String(mensajeOriginal[i], BIN);

      auxiliarCaracterEncriptado = 0;

      for (int j = 0; j < caracterBinarioCompleto.length(); j++) {
        if ((int)caracterBinarioCompleto[j] == 48) {
          auxdouble = 0;
        } else {
          auxdouble = 1;
        }
        auxiliarCaracterEncriptado = (auxdouble * claveMochila[j]) + auxiliarCaracterEncriptado;
      }
      mensajeEncriptado = (String)auxiliarCaracterEncriptado + " " + mensajeEncriptado;
    }
  }

  return mensajeEncriptado;
}


////////////////////////////////////////////////////////////////////////////
///////////////FUNCION PARA DESENCRIPTAR MENSAJES////////////////////////
////////////////////////////////////////////////////////////////////////////

String desencriptar (char* mensaje) {

  char* TAG_ERROR = "ERROR";
  double auxiliarComprobacion;
  String auxiliarBinario = "";

  int   caracterDecimalAscii;
  String mensajeCifrado;
  String mensajeDescifrado = "";

  String mensajeString = (String)mensaje;

  if (mensaje != "") {

    //USE_SERIAL.println(mensaje);

    while (mensajeString.length() > 0) {

      mensajeCifrado = mensajeString.substring(0, mensajeString.indexOf(" "));

      mensajeString = mensajeString.substring(mensajeString.indexOf(" ") + 1);


      auxiliarComprobacion = strtod(mensajeCifrado.c_str(), NULL);
      //Reinicia el valor del auxiliar Binario

      //Recorre la mochila para la decodificacion del mensaje
      auxiliarBinario = "";

      for (int j = 7; j >= 0; j--) {

        if (auxiliarComprobacion > 0) {

          if (auxiliarComprobacion >= claveMochila[j]) {

            auxiliarComprobacion = auxiliarComprobacion - claveMochila[j];

            auxiliarBinario = "1" + auxiliarBinario;

          } else {

            auxiliarBinario = "0" + auxiliarBinario;
          }
        }

      }

      if (auxiliarComprobacion == 0) {

        char charbinario[auxiliarBinario.length() + 1];

        auxiliarBinario.toCharArray(charbinario, auxiliarBinario.length() + 1);
        int num = 0;

        int k = 0;
        for (int i = strlen(charbinario) - 1; i >= 0; i--) {

          num = (((int)charbinario[i] - 48) * pow(2, k)) + num;
          k++;
        }
        mensajeDescifrado = (char)num + mensajeDescifrado;


      } else {

        return TAG_ERROR;

        break;

      }

    }
    return mensajeDescifrado;
  }
}


////////////////////////////////////////////////////////////////////////////
///////////////FUNCION PARA ACTUALIZAR LA HORA EN LA ESP////////////////////
////////////////////////////////////////////////////////////////////////////

void getNtpTime()
{
  JsonObject& root = jsonBuffer.createObject();
  root["dispositivo"] = "RFID";

  root["accion"] = "hora";


  root["data"] = "";

  root.printTo(Data, sizeof(Data));

  //USE_SERIAL.printf("[LOOP_] DATA: %s\n", Data);

  mensajedatancritado = encriptar((String)Data);

  mensajedatancritado.replace(".00", "");
  char charBufData[mensajedatancritado.length() + 1];
  mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);

  INS_WebSocketsClient.sendTXT(charBufData);
  jsonBuffer = StaticJsonBuffer<500>();
  for ( int i = 0; i < strlen(charBufData);  ++i ) {
    charBufData[i] = (char)0;
  }
  mensajedatancritado = "";
  //return 0;

}
