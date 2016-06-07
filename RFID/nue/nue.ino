#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522 (SS_PIN, RST_PIN); // Crear MFRC522 instancia.

void setup () {
	Serial.begin (9600); // Inicializar comunicaciones serie con el PC
	SPI.begin (); // Bus SPI Init
	mfrc522.PCD_Init (); // Tarjeta Init MFRC522
	Serial.println ("Scan PICC para ver UID y escribir ...");
}

void loop () {
	// Buscar las nuevas tarjetas
	if (! mfrc522.PICC_IsNewCardPresent ()) {
		volver;
	}

	// Seleccione una de las tarjetas
	if (! mfrc522.PICC_ReadCardSerial ()) {
		volver;
	}

	// Volcar información de depuración acerca de la tarjeta. PICC_HaltA () es llamado automáticamente.
	mfrc522.PICC_DumpToSerial (& (mfrc522.uid));
}
