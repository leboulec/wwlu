#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>

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

const String server_address = "http://1406-2a04-cec0-10bb-250-5105-a332-9a5c-1cea.ngrok.io/api/open";
bool flag = true;

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

  Serial.println( digitalRead(SWITCH_PIN) );

  if( digitalRead(KEY_PIN) == 0 && flag ){

    Serial.println("Clef tournee !");
    
    if( digitalRead( SWITCH_PIN ) == 0 ){
      Serial.println("J'ouvre la porte !");
      unlock();
      delay(1000);
      lock();  
    }
    else{
      //add card
      Serial.println("J'ajoute une carte !");
      for(int i = 0; i < 10; i++){
        displayColor(BLUE);
        delay(100);
        displayColor(BLACK);
        delay(100);
      }
    }
    flag = false;
  }

  if( digitalRead(KEY_PIN) == 1 ){
    Serial.println("Clef reset !");
    flag = true;
  }
  
  if( WiFi.status() == WL_CONNECTED ){
      HTTPClient http;
      http.begin( server_address );
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
}

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

#define DEBUG true // Enable debug to print UIDs of Tags on the console

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
