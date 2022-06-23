
int masterByte ;
int enablePin = 8;


void setup() {
  Serial.begin(9600);
  pinMode(enablePin, OUTPUT);
  delay(10);
  digitalWrite(enablePin,LOW);
  
}

void loop() {
  while (Serial.available()){
 
   masterByte = Serial.parseInt();
    //Serial.println(masterByte);
    //char aChar  = masterByte;
   // Serial.println(aChar);
      if (masterByte != 0) {
         Serial.println (masterByte, HEX);
      }
  
   delay(100);
    
    
    }

}
