#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

const byte BLACK = 0b000;
const byte RED = 0b100;
const byte GREEN = 0b010;
const byte BLUE = 0b001;
const byte MAGENTA = 0b101;
const byte CYAN = 0b011;
const byte YELLOW = 0b110;
const byte WHITE = 0b111;

#define PIN_RELAY 4
#define PIN_BUZZER 32    
#define PIN_BLUE_LED 21
#define PIN_GREEN_LED 22  
#define PIN_RED_LED 25
#define KEY_PIN 34
#define SWITCH_PIN 14
#define RECONNEXION_DELAY 5000
#define SS_PIN  5  // ESP32 pin GIOP5 
#define RST_PIN 27 // ESP32 pin GIOP27 
#define DEBUG false // Enable debug to print UIDs of Tags on the console

MFRC522 rfid(SS_PIN, RST_PIN);

const String server_address = "http://43f5-2a04-cec0-11f0-501c-e205-9934-4922-485b.ngrok.io";
const String server_address_post = server_address + "/api/users";
const String server_address_get_userid = server_address + "/api/users?userid=";
const String server_address_get_open = server_address + "/api/open";
bool flag = true;
String current_tag = ""; // The tag currently being read
const String white_test = "35748615"; // Test tag
char temp[2];

void displayColor(byte color) {

  digitalWrite(PIN_RED_LED, bitRead(color, 2) );
  digitalWrite(PIN_GREEN_LED, bitRead(color, 1) );
  digitalWrite(PIN_BLUE_LED, bitRead(color, 0) );
}

void connect_to_wifi(char* SSID , char* password){
  WiFi.begin(SSID, password);
  Serial.print("Connecting");
  int debut = millis();
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); 
    int fin = millis();
    if( fin - debut > RECONNEXION_DELAY ){
      WiFi.begin(SSID, password);
      Serial.println("\nNo connexion found ... Reconnexion in progress ...");
      debut = millis();
    }
  }
}

void setup() {
  Serial.begin(115200);
  cli();
  sei();   
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BLUE_LED, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(KEY_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  connect_to_wifi( "iPhone de Clément Roger" , "123456789" ); 
  Serial.println("\nConnected to WiFi");
  displayColor(YELLOW);
  delay(2000);
  displayColor(BLACK);
} 

void unlock(){

  digitalWrite(PIN_RELAY, HIGH);
  for(int i = 0; i < 3; i++){
      digitalWrite(PIN_BUZZER, HIGH);
      displayColor(GREEN);     
      delay(100);
      digitalWrite(PIN_BUZZER, LOW);
      displayColor(BLACK);
      delay(100);
  }
}

void lock(){
  
  digitalWrite(PIN_RELAY, LOW);
  displayColor(RED);
  delay(1000);
  displayColor(BLACK);
}

void loop() {

  if( digitalRead(KEY_PIN) == 0 && flag ){
    
    if( digitalRead( SWITCH_PIN ) == 0 ){
      unlock();
      delay(1000);
      lock();  
    }
    else{
      //add card to database
      HTTPClient http;
      http.begin( server_address_post );
      http.addHeader("Content-Type", "application/json");

      while( !rfid.PICC_IsNewCardPresent() && digitalRead(KEY_PIN) == 0 ); // new tag is available
      
      current_tag = ""; // Resetting current tag value
      if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        for (int i = 0; i < rfid.uid.size; i++) {
          sprintf(temp, "%X", rfid.uid.uidByte[i]); 
          current_tag += String(temp);
        }
      
        rfid.PICC_HaltA(); // halt PICC
        rfid.PCD_StopCrypto1(); // stop encryption on PCD
        int httpResponseCode = http.POST( "{\"userid\" : \"" + current_tag + "\"}" );

        for(int i = 0; i < 3; i++){
          displayColor(BLUE);
          delay(100);
          displayColor(BLACK);
          delay(100);
        }
      }
      http.end();    
    }
    flag = false;
  }  

  if( digitalRead(KEY_PIN) == 1 ){
    flag = true;
  }
  
  if( WiFi.status() == WL_CONNECTED ){
      HTTPClient http;
      http.begin( server_address_get_open );
      int httpResponseCode = http.GET();
      
      if (httpResponseCode > 0) {
       
        if( httpResponseCode == 301 ){
          unlock();
          delay(1000);
          lock();
        }
      }
      else{
        Serial.print("Error code : ");
        Serial.println(httpResponseCode);
      }
      http.end();
  }

  for(int i = 0; i < 20; i++){
    if (rfid.PICC_IsNewCardPresent() && digitalRead(KEY_PIN) != 0) { // new tag is available
      
      current_tag = ""; // Resetting current tag value
      
      if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        for (int i = 0; i < rfid.uid.size; i++) {
          sprintf(temp, "%X", rfid.uid.uidByte[i]); 
          current_tag += String(temp);
        }
        
        rfid.PICC_HaltA(); // halt PICC
        rfid.PCD_StopCrypto1(); // stop encryption on PCD
  
        HTTPClient http;
        http.begin( server_address_get_userid + current_tag ); 
        int httpResponseCode = http.GET();
        
        if(httpResponseCode == 200){ // Valid tag is read
          unlock();
          delay(1000);
          lock();
        }
        else{
          lock();
        }
        http.end();
      }
    }
  }  
}
