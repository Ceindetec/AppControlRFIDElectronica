
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
// DEFINICION DE PINES Y VARIABLES ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#define DEBUG 1
#define TEST_RFID
#define RST_PIN  2    //Pin 9 para el reset del RC522
#define SS_PIN  15   //Pin 10 para el SS (SDA) del RC522
#define Programacion 5
#define Conexion 4
#define Puerta 16
#define Numero_de_usuarios 255
#define Tamano_id 4
#define USE_SERIAL Serial

///////////////////////////////////////////////////////////////////////////
// DEFINICION DE VARIABLES GLOBALES ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
boolean Address;
long Espacio;
unsigned long xy;
byte ActualUID[4]; //almacenará el código del Tag leído
byte Master[4] = {0x8B, 0x23, 0x80, 0xE5} ; //código de la tarjeta Master
byte identificacion[4];
byte usuario[4];
uint32_t Id;
String UID;
time_t prevDisplay = 0;//variable que guarda cuando se va hacer el reset
bool reset = 0;
bool webconectado = 0;
time_t timedesconectado = 0;

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

MFRC522 INS_MFRC522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
WebSocketsClient INS_WebSocketsClient;


///////////////////////////////////////////////////////////////////////////
// DEFINICION DE METODOS //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
boolean Guardar_Usuario(byte Array[]) ;
boolean Buscar_Usuario(byte i_bActualUID[]);
boolean Borrar_usuario(byte i_bActualUID[]);
boolean compareArray(byte array1[], byte array2[]);
boolean Insertar_Usuario(byte i_bActualUID[]);
unsigned long ArrayToDecimal(byte array1[]);
void CharToByte(const char* tarjeta);
void borar_memoria_EEPROM ();
String encriptar(String msn_orgin);
String desencriptar (char* mensaje);
void  getNtpTime();
void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght);
long Buscar_espacio_vacio();
byte leer_id(int direccion, int posicion);
byte leer_num_users();
bool clientIsConnected(WSclient_t * client);

///////////////////////////////////////////////////////////////////////////
// METODO SETUP ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void setup() 
{
  delay(1000);
  pinMode(Programacion, OUTPUT);
  pinMode(Conexion, OUTPUT);
  pinMode(Puerta, OUTPUT);
  digitalWrite( Programacion , LOW );
  digitalWrite( Puerta , HIGH );
  digitalWrite( Conexion , HIGH );
 
  Serial.begin(115200); //Iniciamos la comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  EEPROM.begin((Numero_de_usuarios*4)+1);  //Iniciamos la Memoria EPROM
  INS_MFRC522.PCD_Init(SS_PIN, RST_PIN); // Iniciamos el MFRC522
  
  Serial.println();
  Serial.println("**********************************************");
  Serial.println("*****SISTEMA DE CONTROL DE ACCESO CUSTODE*****");
  Serial.println("*****  DESAROLLADO POR CEINDETEC LLANOS  *****");
  Serial.println("*****        WWW.CEINDETEC.ORG.CO        *****");
  Serial.println("*****      VILLAVICENCIO - COLOMBIA      *****");
  Serial.println("**********************************************");
  delay(1000);

  #ifdef TEST_RFID
  Serial.println("Control de acceso:");
  Serial.print("Numero de usuarios registrados: ");
  Serial.println(EEPROM.read(0));
  Serial.print("Numero maximo de usuarios: ");
  Serial.println(Numero_de_usuarios);
  delay(1000);
  #endif

  #ifdef DEBUG
  USE_SERIAL.printf("[SETUP] Conectando a: ");
  USE_SERIAL.printf(ssid);
  USE_SERIAL.println();  
  #endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);

    #ifdef DEBUG
    USE_SERIAL.printf(".");
    #endif
  }
  digitalWrite( Conexion , LOW );

  #ifdef DEBUG
  USE_SERIAL.println("");
  USE_SERIAL.printf("[SETUP] WiFi conectado ");
  USE_SERIAL.printf("IP address: ");
  USE_SERIAL.println(WiFi.localIP());
  #endif

  INS_WebSocketsClient.begin(ip_websocket, port_websocket);
  INS_WebSocketsClient.onEvent(webSocketEvent);
  INS_WebSocketsClient.disconnect();
}

///////////////////////////////////////////////////////////////////////////
// METODO LOOP ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void loop() 
{
  time_t t = now();
  //INS_WebSocketsClient.loop();
  if(WiFi.status() != WL_CONNECTED){
    digitalWrite( Conexion , HIGH );
  }
  if(webconectado==0 && timedesconectado==0){
    INS_WebSocketsClient.loop();
  }
  if(webconectado==1){
    INS_WebSocketsClient.loop();
  }else if(now() >= timedesconectado){
    timedesconectado = 0;
    INS_WebSocketsClient.loop();
  }
  
  // Revisamos si hay nuevas tarjetas  presentes
  if ( INS_MFRC522.PICC_IsNewCardPresent()) 
  {
    //Seleccionamos una tarjeta
    digitalWrite( Programacion , HIGH );    
    if ( INS_MFRC522.PICC_ReadCardSerial()) {
      #ifdef TEST_RFID
      USE_SERIAL.println("[LOOP_] ********************************");
      USE_SERIAL.printf("[LOOP_] Tarjeta leida UID:");
      #endif
      for (byte i = 0; i < INS_MFRC522.uid.size; i++) {
        #ifdef TEST_RFID
        USE_SERIAL.print(INS_MFRC522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        USE_SERIAL.print(INS_MFRC522.uid.uidByte[i], HEX);
        #endif
        ActualUID[i] = INS_MFRC522.uid.uidByte[i];
      }
      #ifdef TEST_RFID
      USE_SERIAL.println();
      #endif
      //comparamos los UID para determinar si es uno de nuestros usuarios
      if (Buscar_Usuario(ActualUID) == false) 
      {
        #ifdef TEST_RFID
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
        USE_SERIAL.println();
        #endif

        mensajedatancritado = encriptar((String)Data);
        mensajedatancritado.replace(".00", "");
        char charBufData[mensajedatancritado.length() + 1];
        mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);
        
        if(webconectado == 1)
         {
            USE_SERIAL.println("ENVIANDO MENSAJE");
            if(INS_WebSocketsClient.sendTXT(charBufData)==1)
            {
              USE_SERIAL.println("MENSAJE ENVIADO");
            }
            else
            {
               USE_SERIAL.println("ERROR MENSAJE");
                INS_WebSocketsClient.disconnect();
                delay(100);
                ESP.restart();
            }
        }
        
        for ( int i = 0; i < strlen(charBufData);  ++i ) 
        {
          charBufData[i] = (char)0;
        }
        mensajedatancritado = "";
        jsonBuffer = StaticJsonBuffer<500>();
        for ( int i = 0; i < sizeof(Data);  ++i ) 
        {
          Data[i] = (char)0;
        }
      }
      else 
      {
        #ifdef TEST_RFID
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
        USE_SERIAL.println();
        #endif

        mensajedatancritado = encriptar((String)Data);
        mensajedatancritado.replace(".00", "");
        char charBufData[mensajedatancritado.length() + 1];
        mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);
        digitalWrite( Puerta , LOW );
        delay(1000);
        digitalWrite( Puerta , HIGH );
        
        if(webconectado == 1)
         {
            USE_SERIAL.println("ENVIANDO MENSAJE");
            if(INS_WebSocketsClient.sendTXT(charBufData)==1)
            {
              USE_SERIAL.println("MENSAJE ENVIADO");
            }
            else
            {
               USE_SERIAL.println("ERROR MENSAJE");
               INS_WebSocketsClient.disconnect();
               delay(100);
               ESP.restart();
            }
        }
       

        for ( int i = 0; i < strlen(charBufData);  ++i ) 
        {
          charBufData[i] = (char)0;
        }
        mensajedatancritado = "";
        jsonBuffer = StaticJsonBuffer<500>();
        #ifdef DEBUG
        USE_SERIAL.printf("[LOOP_] DATA FOR: %s\n", Data);
        #endif

        for ( int i = 0; i < sizeof(Data);  ++i ) 
        {
          Data[i] = (char)0;
        }
        for ( int i = 0; i < sizeof(charBufData);  ++i ) 
        {
          charBufData[i] = (char)0;
        }
        mensajedatancritado="";
      }
      INS_MFRC522.PICC_HaltA();// Terminamos la lectura de la tarjeta tarjeta  actual
    }
    digitalWrite( Programacion , LOW);
  }
  delay(100);
  if (now() >= prevDisplay && reset == 1) {
    INS_WebSocketsClient.disconnect();
    delay(100);
    ESP.restart();
    reset = 0;
  }
}


///////////////////////////////////////////////////////////////////////////
////////////////FUNCION PARA GUARDAR USUARIOS EN LA EEPROM/////////////////
///////////////////////////////////////////////////////////////////////////
boolean Guardar_Usuario(byte i_Array[]) {
  int numero_usuarios = EEPROM.read(0);
  if(numero_usuarios< Numero_de_usuarios)
  {
    for (byte i = 0; i < Tamano_id; i++) 
    {
      EEPROM.write((numero_usuarios * Tamano_id) + 1 + i, i_Array[i]);
    }
    EEPROM.write(0, numero_usuarios + 1);
    EEPROM.commit();
    return true;
  }
  else
  {
    return false;
  }
   
}

///////////////////////////////////////////////////////////////////////////
//////////////////FUNCIÓN PARA BORAR USUARIOS EN LA EEPROM/////////////////
///////////////////////////////////////////////////////////////////////////
boolean Borrar_usuario(byte i_bActualUID[]) {
  byte j;
  byte k;
  int numero_usuarios = EEPROM.read(0);
  
  for ( j = 0; j < numero_usuarios; j++)
  {
    for ( k = 0; k < Tamano_id; k++) 
    {
      if (i_bActualUID[k] != leer_id( j, k))
      {
        k=Tamano_id;
      }
    }
    if (k == 4) 
    {
      int direccion=0;
      for (k = j; k < numero_usuarios; k++)
      {
        direccion= k*Tamano_id+1;
        EEPROM.write(direccion + 0, leer_id( k+1, 0));
        EEPROM.write(direccion + 1, leer_id( k+1, 1));
        EEPROM.write(direccion + 2, leer_id( k+1, 2));
        EEPROM.write(direccion + 3, leer_id( k+1, 3));
      }
      EEPROM.write(0,numero_usuarios-1);
      EEPROM.commit();
      return (true);
    }
  }
  return (false);
}

///////////////////////////////////////////////////////////////////////////
////////////////FUNCIÓN PARA BUSCAR USUARIOS EN LA EEPROM//////////////////
///////////////////////////////////////////////////////////////////////////
boolean Buscar_Usuario(byte i_bActualUID[]) {
  byte j;
  byte k;
  int numero_usuarios = EEPROM.read(0);
  
  #ifdef TEST_RFID
  Serial.print("Buscando usuarios");
  #endif
  for ( j = 0; j < numero_usuarios; j++)
  {
    #ifdef TEST_RFID
    Serial.print(".");
    #endif
    for ( k = 0; k < Tamano_id; k++) 
    {
      if (i_bActualUID[k] != leer_id( j, k))
      {
        k=Tamano_id;
      }
    }
    if (k == 4) 
    {
      #ifdef TEST_RFID
      Serial.println();
      Serial.println("Usuario encontrado");
      #endif
      return (true);
    }
  }
  #ifdef TEST_RFID
  Serial.println();
  Serial.println("Usuario NO encontrado");
  #endif
  return (false);
}

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

////////////////////////////////////////////////////////////////////////////
//////////////////FUNCION BORRAR TODOS LOS USUARIOS/////////////////////////
////////////////////////////////////////////////////////////////////////////

void borar_memoria_EEPROM () 
{
  int registrosDeMemoria= (Numero_de_usuarios)*4+1;
  for ( int i = 0; i < registrosDeMemoria; i++) 
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}
//////////////////////////////////////////////////////////////////////////////////////
///////////////////FUNCION LEER NUMERO DE USUARIOS////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
byte leer_num_users()
{
  return EEPROM.read(0);
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////FUNCION LEER ID DE USUARIOS///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
byte leer_id(int direccion, int posicion)
{
  if (direccion < Numero_de_usuarios && posicion < Tamano_id) 
  {
    EEPROM.read(direccion * Tamano_id + posicion + 1 );
  }
}

///////////////////////////////////////////////////////////////////////////
///////////////FUNCION INSERTAR USUARIO////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
boolean Insertar_Usuario(byte i_bActualUID[])
{
   if(Buscar_Usuario(i_bActualUID)==false)
    {
      if(Guardar_Usuario(i_bActualUID))
      {
        #ifdef TEST_RFID
        Serial.println("El nuevo usuario ha sido registrado exitosamente");
        #endif
      }
      else
      {
        #ifdef TEST_RFID
        Serial.println("El usuario NO fue registrado. Memoria LLENA");
        #endif
      }    
    }
    else
    {
      #ifdef TEST_RFID
      Serial.println("El usuario ya esta registrado");   
      #endif
    }
  
}

///////////////////////////////////////////////////////////////////////////
///////////////FUNCION TRANSFORMAR ARRAY 4 BYTE EN uint32_t////////////////
////////////////////////////////////////////////////////////////////////////
unsigned long ArrayToDecimal(byte array1[])
{
  unsigned long Decimal = (array1[0] << 24) | (array1[1] << 16) | (array1[2] << 8) | (array1[3]);
  return (Decimal);
}

////////////////////////////////////////////////////////////////////////////
///////////////FUNCION PARA CONVERTIR UN CHAR A BYTE////////////////////////
////////////////////////////////////////////////////////////////////////////
void CharToByte(const char* tarjeta) {
  uint8_t iteracion = 0;
  uint8_t num1 = 0;
  uint8_t num2 = 0;

  for (int i = 0; i < 8; i = i + 2) 
  {
    num1 = (uint8_t)tarjeta[i];
    num2 = (uint8_t)tarjeta[i + 1];
    if (num1 > 47 && num1 < 58) 
    {
      num1 = num1 - 48;
    }
    else 
    {
      num1 = num1 - 87;
    }
    if (num2 > 47 && num2 < 58)
    {
      num2 = num2 - 48;
    }
    else 
    {
      num2 = num2 - 87;
    }
    identificacion[iteracion] = num2 + (num1 * 16);
    iteracion = iteracion + 1;
  }
  return;
}

////////////////////////////////////////////////////////////////////////////
///////////////FUNCION PARA ENCRIPTAR MENSAJES//////////////////////////////
////////////////////////////////////////////////////////////////////////////
String encriptar (String msn_orgin ) 
{
  char mensajeOriginal[msn_orgin.length() + 1];
  String mensajeEncriptado = "";
  String caracterBinarioCompleto;
  String cadenaCeros = "00000000";
  String complementoBinario;
  double auxiliarCaracterEncriptado;
  double auxdouble;

  if (msn_orgin != "") 
  {
    for (int m = 0; m < msn_orgin.length(); m++) 
    {
      mensajeOriginal[m] = msn_orgin[m];
    }
    for ( int i = 0; i < sizeof(mensajeEncriptado);  ++i )
    {
      mensajeEncriptado[i] = (char)0;
    }
    for (int i = 0 ; i < msn_orgin.length() ; i++ ) 
    {
      complementoBinario = cadenaCeros.substring(0, 8 - String(mensajeOriginal[i], BIN).length());
      caracterBinarioCompleto = complementoBinario + String(mensajeOriginal[i], BIN);
      auxiliarCaracterEncriptado = 0;

      for (int j = 0; j < caracterBinarioCompleto.length(); j++) 
      {
        if ((int)caracterBinarioCompleto[j] == 48) 
        {
          auxdouble = 0;
        } 
        else 
        {
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
///////////////FUNCION PARA DESENCRIPTAR MENSAJES//////////////////////////
////////////////////////////////////////////////////////////////////////////
String desencriptar (char* mensaje) 
{
  char* TAG_ERROR = "ERROR";
  double auxiliarComprobacion;
  String auxiliarBinario = "";
  int   caracterDecimalAscii;
  String mensajeCifrado;
  String mensajeDescifrado = "";
  String mensajeString = (String)mensaje;

  if (mensaje != "") 
  {
    while (mensajeString.length() > 0) 
    {
      mensajeCifrado = mensajeString.substring(0, mensajeString.indexOf(" "));
      mensajeString = mensajeString.substring(mensajeString.indexOf(" ") + 1);
      auxiliarComprobacion = strtod(mensajeCifrado.c_str(), NULL);
      //Reinicia el valor del auxiliar Binario
      //Recorre la mochila para la decodificacion del mensaje
      auxiliarBinario = "";
      for (int j = 7; j >= 0; j--) 
      {
        if (auxiliarComprobacion > 0) 
        {
          if (auxiliarComprobacion >= claveMochila[j]) 
          {
            auxiliarComprobacion = auxiliarComprobacion - claveMochila[j];
            auxiliarBinario = "1" + auxiliarBinario;
          } 
          else 
          {
            auxiliarBinario = "0" + auxiliarBinario;
          }
        }
      }
      if (auxiliarComprobacion == 0) 
      {
        char charbinario[auxiliarBinario.length() + 1];
        auxiliarBinario.toCharArray(charbinario, auxiliarBinario.length() + 1);
        int num = 0;
        int k = 0;
        for (int i = strlen(charbinario) - 1; i >= 0; i--)
        {
          num = (((int)charbinario[i] - 48) * pow(2, k)) + num;
          k++;
        }
        mensajeDescifrado = (char)num + mensajeDescifrado;
      } 
      else
      {
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
  mensajedatancritado = encriptar((String)Data);
  mensajedatancritado.replace(".00", "");
  
  char charBufData[mensajedatancritado.length() + 1];
  mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);
  INS_WebSocketsClient.sendTXT(charBufData);
  jsonBuffer = StaticJsonBuffer<500>();
  
  for ( int i = 0; i < strlen(charBufData);  ++i ) 
  {
    charBufData[i] = (char)0;
  }
  mensajedatancritado = "";
}
///////////////////////////////////////////////////////////////////////////
// METODO WEBSOCKECTEVENT QUE MANEJA LOS EVENTOS DEL WEBSOCKET ////////////
///////////////////////////////////////////////////////////////////////////

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) 
{
  #ifdef DEBUG
  USE_SERIAL.printf("[SOCKE] **********************************\n");
  USE_SERIAL.printf("[SOCKE] TIPO: %d\n", type);
  USE_SERIAL.printf("[SOCKE] PAYLOAD: %s\n", payload);
  USE_SERIAL.printf("[SOCKE] LONGITUD: %d\n", lenght);
  #endif
  
  switch (type) 
  {
  ///////////////////////////////////////////////////////////////////////////
    case WStype_DISCONNECTED: 
    {
        #ifdef DEBUG
          USE_SERIAL.printf("[SOCKE] EVENTO DESCONEXION\n");
        #endif
        webconectado = 0;
        if(timedesconectado==0){
          timedesconectado = now()+10;
        }
        break;
      }
    ///////////////////////////////////////////////////////////////////////////
    case WStype_CONNECTED: 
    {
        webconectado = 1;
        JsonObject& root = jsonBuffer.createObject();
        root["dispositivo"] = "cccccccc";
        root["accion"] = "presentacion";
        root["data"] = "";
        root.printTo(Data, sizeof(Data));

        mensajedatancritado = encriptar((String)Data);

        mensajedatancritado.replace(".00", "");
        char charBufData[mensajedatancritado.length() + 1];
        mensajedatancritado.toCharArray(charBufData, mensajedatancritado.length() + 1);

        INS_WebSocketsClient.sendTXT(charBufData);

        for ( int i = 0; i < strlen(charBufData);  ++i ) 
        {
          charBufData[i] = (char)0;
        }
        
        mensajedatancritado = "";
        jsonBuffer = StaticJsonBuffer<500>();
        for ( int i = 0; i < sizeof(Data);  ++i ) 
        {
          Data[i] = (char)0;
        }
      
        delay(100);
        getNtpTime();
        break;
      }
    ///////////////////////////////////////////////////////////////////////////
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
        ///////////////////////////////////////////////////////////////////////////
        if (strcmp(accion, "DEL") == 0) 
        {
          const char* data    = rootSocket["data"];
          CharToByte(data);
          
          #ifdef TEST_RFID
          Serial.println("El usuario sera eliminado");
          #endif
          
         if(Borrar_usuario(identificacion))
         {
          #ifdef TEST_RFID
          Serial.println("El usuario ha sido eliminado");
          #endif
         }
         else
         {
          #ifdef TEST_RFID
          Serial.println("El usuario no esta registrado");
          #endif
         }
          
        }
        ///////////////////////////////////////////////////////////////////////////
        else if (strcmp(accion, "INS") == 0) 
        {
          const char* data    = rootSocket["data"];
          CharToByte(data);
          Insertar_Usuario(identificacion);
        }
        ///////////////////////////////////////////////////////////////////////////
        else if (strcmp(accion, "UPD") == 0) 
        {
          long longitud_data    = rootSocket["total"];
          long iteracion = 0;
          int direccion=0;
          int registrosDeMemoria= (Numero_de_usuarios)*4+1;
          borar_memoria_EEPROM ();
          for (long i = 0; i < longitud_data; i++) 
          {
            CharToByte(rootSocket["data"][i]);
            if(i<longitud_data)
            {
              direccion= i*Tamano_id+1;
              EEPROM.write(direccion, identificacion[0]);
              EEPROM.write(direccion+1, identificacion[1]);
              EEPROM.write(direccion+2, identificacion[2]);
              EEPROM.write(direccion+3, identificacion[3]);
            }
            else
            {
              direccion= i*Tamano_id+1;
              EEPROM.write(direccion  , 0);
              EEPROM.write(direccion+1, 0);
              EEPROM.write(direccion+2, 0);
              EEPROM.write(direccion+3, 0);          
            }
            
          }
          EEPROM.write(0,(byte)longitud_data);
          EEPROM.commit();
          #ifdef DEBUG
          USE_SERIAL.println("La actualizacion fue exitosa");
          #endif
        }
        ///////////////////////////////////////////////////////////////////////////
        else if (strcmp(accion, "HRO") == 0) 
        {
          #ifdef DEBUG
          USE_SERIAL.print("ESTA ES LA HORA: ");
          USE_SERIAL.println(mensajeservidor);
          #endif
          
          setTime((int)rootSocket["data"][0], (int)rootSocket["data"][1], (int)rootSocket["data"][2], (int)rootSocket["data"][3], (int)rootSocket["data"][4], (int)rootSocket["data"][5]);
          prevDisplay = now() + (1200 + (random(5400)));
          #ifdef DEBUG
          USE_SERIAL.println("Tiempo para reset......");
          USE_SERIAL.println((prevDisplay-now())/60);
          #endif
          reset = 1;
        }
        ///////////////////////////////////////////////////////////////////////////
        else if (strcmp(accion, "PER") == 0) 
        {
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
    ///////////////////////////////////////////////////////////////////////////
    case WStype_BIN: 
    {
        USE_SERIAL.printf("[SOCKE] Mensaje binario obtenido: %u\n", lenght);
        hexdump(payload, lenght);
        // send data to server
        // INS_WebSocketsClient.sendBIN(payload, lenght);
        break;
    }
     ///////////////////////////////////////////////////////////////////////////
    case WStype_ERROR:
    {
       USE_SERIAL.printf("error en la comunicacion", lenght);
       webconectado=0;
    }
  }
  #ifdef DEBUG
  USE_SERIAL.printf("[SOCKE] **********************************\n");
  #endif
}

