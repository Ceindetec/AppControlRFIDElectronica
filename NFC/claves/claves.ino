#include <Arduino.h>

#include <PN532.h>                        
#define SCK         13
#define MOSI        11
#define SS          10
#define MISO        12
#define LEDv         9
#define LEDr         8
#define Hugo  4091727095
#define Carlos  4145250229
#define Lorena  3564728086
#define German_Londono  2191558117
#define David  2158589007
#define Cesar  4081680309
#define Nohelia  4270894805
#define Vicent 2772169413
#define Jorge 3069804773
#define Victor 2493351397
#define Pineda 2334359781
#define German_Mora 2928771301
#define Miguel 497058021
#define Natalia 1766425061
#define Jorge_Luis 4095574245
#define Fabio 224036325
#define Christian 105873893
#define Jesus 2836955621

PN532 nfc(SCK, MISO, MOSI, SS);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  nfc.begin();
  nfc.SAMConfig();

  pinMode(LEDv, OUTPUT);
  pinMode(LEDr, OUTPUT);

  Serial.println("Placa preparada para leer!");
}


void loop()
{
  uint32_t id=0;
  id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);
  if ( id == Hugo )
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Carlos )
  { 
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Lorena )
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == German_Londono )
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == David )
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }

  if ( id == Cesar )
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }

  if ( id == Nohelia )
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }

  if ( id == Vicent)
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == German_Mora)
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Miguel)
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Natalia)
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Jorge_Luis )
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Fabio)
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Christian)
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }
  if ( id == Jesus)
  {
    digitalWrite( LEDv , HIGH );
    Serial.println("Placa preparada para leer!");
    delay(2000);
    digitalWrite( LEDv , LOW );
  }

  if ( id > 0 )
  {
    digitalWrite( LEDr , HIGH );
    delay(2000);
    digitalWrite( LEDr , LOW );
  }
  id=0;
}
