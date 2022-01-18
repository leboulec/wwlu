#define PIN_RELAY 4
#define PIN_BUZZER 5    
#define PIN_GREEN_LED 21
#define PIN_RED_LED 22
#define PIN_READ_LOCK 15

void setup() {
  Serial.begin(115200);       
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);     
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
  
    
  unlock();
  delay(200);
  lock();
  delay(10000);                      
}
