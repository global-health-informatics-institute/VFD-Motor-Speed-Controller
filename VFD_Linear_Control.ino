#include <SoftwareSerial.h>

// RS485 connections
// D8 = DI (TX)
// D9 = DE
// D10 = RE
// D11 = RO (RX)

// adjust speed

char* change_speed = ":010503030BB8";

char* VFD_Speed_40 = ":0105030FA000";
char* VFD_Speed_20 = ":01050307D000";
int VFD_Speed = 2000;
 int frequency ;
char* VFD_Stop = ":01030108";
char* VFD_Start = ":01030101";
float gain; 
float pressureInput = A0 ; //analog pin
const int pressureZero = 78;
const int pressureMax = 938;
const int pressuretransducerMaxPSI = 60;
float pressureValue ;
float kp = 1;
float Pressure_error = 0; 
float Target_Pressure = 30 ;
 
SoftwareSerial myserial(11, 8); // RX, TX

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
  myserial.begin(9600);
  digitalWrite(9, LOW); digitalWrite(10, LOW);
 query(VFD_Start);
 /* setSpeed(1500);
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
  n[7] = toHexa((v / 256) >> 4);  // I changed these four lines to get the function to work.
  n[8] = toHexa((v / 256) & 0xf);
  n[9] = toHexa((v & 0xff) >> 4);
  n[10] = toHexa((v & 0xff) & 0xf);
  query(n); // set new speed
}

void loop() {
  
 // setSpeed(3300);
   //  query(VFD_Speed_25);
 // float reading = analogRead(pressureInput) ;
   float Current_Pressure= get_pressureValue(); 
    Serial.println(Current_Pressure);
    //delay(2000);
    
  Pressure_error = Current_Pressure - Target_Pressure ;
  
  if (Pressure_error > 0) {
    VFD_Speed = VFD_Speed - 1000; 
  }
  else if (Pressure_error < 0) {
    VFD_Speed= VFD_Speed + 1000;
    setSpeed(VFD_Speed);
  }
  
  
  //  query(VFD_Stop);
  delay(500);
}

float get_pressureValue(){//calculate pressure in psi
  pressureValue = analogRead(pressureInput); //read pressure
  pressureValue = ((pressureValue - pressureZero)*pressuretransducerMaxPSI)/(pressureMax - pressureZero); //convert read pressure to psi
  if (pressureValue < 0) //set all negative pressures to zero
    pressureValue = 0;
    return pressureValue;
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
