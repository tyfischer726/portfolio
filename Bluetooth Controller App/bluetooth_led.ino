char inputByte;
int ledPin = 13;

void setup() {
 Serial.begin(9600);
 pinMode(ledPin,OUTPUT);
}

void loop() {
  
  while(Serial.available()>0) {
    inputByte = Serial.read();
    Serial.println(inputByte);
    
    if (inputByte=='a') {
      digitalWrite(ledPin,HIGH);
      }
    else if (inputByte=='b') {
      digitalWrite(ledPin,LOW);
      }
  }
}
