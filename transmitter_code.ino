
int incomingByte = 0; // for incoming serial data
int enablePin = 8;


void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

  pinMode(enablePin, OUTPUT);
  delay(10);
  digitalWrite(enablePin, HIGH);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available()) {
    // read the incoming byte:
    incomingByte = Serial.read();
    char aChar = incomingByte;
    delay(100);

    // say what you got:
     Serial.print(aChar);
    //Serial.println(" incoming byte received");
    //Serial.println(incomingByte,HEX);
  }
}
