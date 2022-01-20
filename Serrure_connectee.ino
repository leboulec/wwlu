#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>

#define PIN_RELAY 4
#define PIN_BUZZER 5    
#define PIN_GREEN_LED 21
#define PIN_RED_LED 22
#define PIN_READ_LOCK 15
#define KEY_PIN 19
#define SWITCH_PIN 18
#define RECONNEXION_DELAY 5000

const String server_address = "http://1406-2a04-cec0-10bb-250-5105-a332-9a5c-1cea.ngrok.io/api/open";
int flag = 0;

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

void interrupt_key(){
    flag = 1;
}

void setup() {
  Serial.begin(115200);
  cli();
  sei();
  connect_to_wifi( "Juliette Venel" , "toutenmajuscule" ); 
  Serial.println("\nConnected to WiFi");      
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(KEY_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(KEY_PIN), interrupt_key, FALLING);     
} 

void unlock(){

  digitalWrite(PIN_RELAY, HIGH);
  for(int i = 0; i < 3; i++){
      digitalWrite(PIN_BUZZER, HIGH);
      digitalWrite(PIN_GREEN_LED, HIGH);     
      delay(100);
      digitalWrite(PIN_BUZZER, LOW);
      digitalWrite(PIN_GREEN_LED, LOW);
      delay(100);
  }
}

void lock(){
  
  digitalWrite(PIN_RELAY, LOW);
  digitalWrite(PIN_RED_LED, HIGH);
  delay(1000);
  digitalWrite(PIN_RED_LED, LOW);
}

void loop() {

  if( flag == 1 ){
    
    if( digitalRead( SWITCH_PIN ) == 0 ){
      unlock();
      delay(1000);
      lock();  
    }
    else{
      //add card
      for(int i = 0; i < 10; i++){
        digitalWrite(PIN_RED_LED, HIGH);
        delay(100);
        digitalWrite(PIN_RED_LED, LOW);
        delay(100);
      }
    }
      
    flag = 0;
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
      //http.end();
  }                         
}
