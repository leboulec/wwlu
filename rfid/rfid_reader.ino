/*
 * "We Will Lock You" project : Clément LEBOULENGER / Edouard MATHEU / Clément ROGER
 * 
 * Code made using example from esp32.io
 * For moree information visit : https://esp32io.com/tutorials/esp32-rfid-nfc
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  5  // ESP32 pin GIOP5 
#define RST_PIN 27 // ESP32 pin GIOP27 

#define DEBUG false // Enable debug to print UIDs of Tags on the console

MFRC522 rfid(SS_PIN, RST_PIN);

String current_tag = ""; // The tag currently being read
const String white_test = "35748615"; // Test tag
char temp[2];

void setup() {
  Serial.begin(115200);
  Serial.print("SETUP : ");
  Serial.println(white_test);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    
    current_tag = ""; // Resetting current tag value
    
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      for (int i = 0; i < rfid.uid.size; i++) {
        sprintf(temp, "%X", rfid.uid.uidByte[i]); 
        current_tag += String(temp);
      }
      
      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD

      if(DEBUG){ // Prints used for debug
        Serial.print("RFID/NFC Tag Type: ");
        Serial.println(rfid.PICC_GetTypeName(piccType));
        Serial.print("TAG UID : ");
        Serial.println(current_tag);
        Serial.println(white_test);
      }
      
      if(current_tag == white_test){ // Valid tag is read
        Serial.println("OPENING DOOR");
      }
      
      else{ // Invalid tag is read
        Serial.println("WRONG ID");
      }
    }
  }
}
