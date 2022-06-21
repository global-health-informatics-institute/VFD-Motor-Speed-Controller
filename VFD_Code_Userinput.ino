#include <SoftwareSerial.h>

// RS485 connections
// D8 = DI (TX)
// D9 = DE
// D10 = RE
// D11 = RO (RX)

// adjust speed

char* change_speed = ":010503030BB8";

char* VFD_Speed_40 = ":0105030FA000";
char* VFD_Speed_25 = ":01050309C400";

 int frequency ;
char* VFD_Stop = ":01030108";
char* VFD_Start = ":01030101";


SoftwareSerial myserial(11, 8); // RX, TX

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
  myserial.begin(9600);
  digitalWrite(9, LOW); digitalWrite(10, LOW);

  /*setSpeed(1500);
  delay(2000);
  setSpeed(2000);
  delay(2000);
  setSpeed(2500);
  delay(2000);
  setSpeed(3000);
  delay(2000);
  setSpeed(3500);
  delay(2000);
  setSpeed(4000);
  delay(2000);
  setSpeed(4500);
  delay(2000);
  setSpeed(5000);
  delay(2000);*/
}

void setSpeed(int v) {
  char* n = change_speed;
  n[7] = toHexa((v / 256) >> 4);  // I changed htese four lines to get the function to work.
  n[8] = toHexa((v / 256) & 0xf);
  n[9] = toHexa((v & 0xff) >> 4);
  n[10] = toHexa((v & 0xff) & 0xf);
  query(n); // set new speed
}

void loop() {
   query(VFD_Start);
  delay(1000);
  setSpeed(3300);
  delay(2000);
  
  if (Serial.available() > 0) {
      frequency = Serial.parseInt();
      setSpeed(frequency);
     myserial.print(frequency);
      delay(6000);
  }
  //  query(VFD_Speed_25);
  
 
  //  query(VFD_Stop);
 
}

void transmit(String msg) {
  // activate driver
  digitalWrite(9, HIGH); digitalWrite(10, HIGH);
  delay(50);
  myserial.print(msg);
  Serial.print(msg);
  delay(1);
  digitalWrite(9, LOW); digitalWrite(10, LOW);
}

char hexa(char byte) { // return hexa value of that ASCII char
  if (byte <= '9') return byte & 0x0f;
  if (byte <= 'F') return (byte & 0x0f) + 9; // A=0x41 --> 0x0a
}

char toHexa(int i) {
  if (i < 10) return 0x30 | i;
  return 0x41 + i - 10;
}

char crc(char* buffer) {
  int l = strlen(buffer);
  int i = 1;
  int chk = 0;
  while (i < l) {
    //   Serial.print(hexa(buffer[i]) << 4 | hexa(buffer[i + 1]), HEX);
    chk += ( hexa(buffer[i]) << 4 | hexa(buffer[i + 1]) );
    i += 2;
    Serial.print(" ");
  }
  return (0x100 - chk) & 0xff;
}

void query(char* cmd) {
  char lrc = crc(cmd);
  String msg = cmd;
  msg += toHexa((lrc & 0xf0) >> 4);
  msg += toHexa(lrc & 0x0f);
  msg += "\r\n";
  //  Serial.print(msg);
  transmit(msg);
}
