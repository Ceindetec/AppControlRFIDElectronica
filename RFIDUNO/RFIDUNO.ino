#include <MFRC522.h>


#include <SPI.h>
#include <Wire.h>
/*
Pins	SPI	   UNO
1 (NSS) SAD (SS)   10
2       SCK        13
3       MOSI       11
4       MISO       12
5       IRQ        *
6       GND       GND
7       RST        5
8      +3.3V (VCC) 3V3
* Not needed
1 on ICPS header
*/
#define SAD 10
#define RST 9
MFRC522 nfc(SAD, RST);
#define ledPinAbierto    5
#define ledPinCerrado  6

void setup() {

  pinMode(ledPinAbierto  , OUTPUT);
  pinMode(ledPinCerrado, OUTPUT);
  SPI.begin();
  Serial.begin(115200);
  Serial.println("BUSCANDO MFRC522.");
  nfc.begin();
  byte version = nfc.getFirmwareVersion();
  if (! version) {
    Serial.print("NO SE ENCONTRO MFRC522 ");
    while (1); //halt
  }
  Serial.print("BUSCANDO CHIP MFRC522 ");
  Serial.print("FIRMWARE VERSION. 0x");
  Serial.print(version, HEX);
  Serial.println(".");
}

#define TARJETA 1
#define LLAVE 1
byte Autorizado[TARJETA][6] = {{0x50, 0x8E, 0x80, 0xE5, 0xFF, 0xFF, }};
byte Autorizado3[TARJETA][6] = {{0xD4, 0x79, 0x6F, 0x16, 0xFF, 0xFF, }};
byte Autorizado2[LLAVE][6] = {{0xE3, 0xE1, 0xFB, 0xA4, 0xFF, 0xFF, }};
void imprimeClave(byte *serial);
boolean esIgual(byte *key, byte *serial);
boolean chekaKey(byte *serial);

void loop() {
  byte status;
  byte data[MAX_LEN];
  byte serial[5];
  boolean Abierto = false;

  status = nfc.requestTag(MF1_REQIDL, data);
  myservo.write(180);

  if (status == MI_OK) {
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);
    if (chekaKey(serial)) {
      Serial.println("AUTORIZADO");
      Abierto = true;
    } else {
      imprimeClave(serial);
      Serial.println("NO AUTORIZADO");
      Abierto = false;
    }
    nfc.haltTag();
    digitalWrite(ledPinAbierto, Abierto);
    digitalWrite(ledPinCerrado, !Abierto);
    delay(2000);
  }
  delay(70);
}

boolean esIgual(byte *key, byte *serial) {
  for (int i = 0; i < 4; i++) {
    if (key[i] != serial[i]) {
      return false;
    }
  }
  return true;
}

boolean chekaKey(byte *serial)
{
  for (int i = 0; i < TARJETA; i++)
  {
    if (esIgual(serial, Autorizado[i])) {
      Serial.println  ("  Ing Lorena  ");
      return true;
    }
  }
  for (int i = 0; i < TARJETA; i++)
  {
    if (esIgual(serial, Autorizado3[i])) {
      Serial.println("  Ing Cesar  ");
      return true;
    }
  }
  for (int i = 0; i < LLAVE; i++)
  {
    if (esIgual(serial, Autorizado2[i])) {
      Serial.println("  Ing Miguel  ");
      return true;
    }
  }
  return false;
}

void imprimeClave(byte *serial)
{
  Serial.print("CLAVE: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(serial[i], HEX);
    Serial.print(" ");
  } |
