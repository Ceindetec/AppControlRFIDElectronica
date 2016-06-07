#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 5
#define	uchar	unsigned char
#define	uint	unsigned int

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.
uchar serNum[5];

void setup() {
	Serial.begin(9600);	// Initialize serial communications with the PC
	SPI.begin();		// Init SPI bus
     
	mfrc522.PCD_Init();	// Init MFRC522 card
}

void loop(){
       
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
        }

        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
                return;
        }
        Serial.print("Card UID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
                Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mfrc522.uid.uidByte[i], HEX);
                serNum[i]=mfrc522.uid.uidByte[i];
        } 
        Serial.println();
        
        // Checking the UID
        if(serNum[0] == 153 && serNum[1] == 154 && serNum[2]==85 && serNum[3]==45 && serNum[4]==202) {
           
        }
}
