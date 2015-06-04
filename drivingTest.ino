/*
*  Tom Dodge  
*  3/5/15
*  last updated: 3/5/15
*
*  Testing out some driving of the roomba: forward, reverse,  
*  CCW, and CW at one speed. Room to add left turn, right turn,
*  and increase/decrease speed.
* forking for my directory - raphaelphys
*/

#include <SoftwareSerial.h>

// pin assignments:
byte ledPin = 13;
byte ddPin = 12;
byte txPin = 11;
byte rxPin = 10;

SoftwareSerial rooSerial(rxPin, txPin);

// roomba codes used
const  byte START = 128;
const byte CONTROL = 130;
const byte SAFE = 131;
const byte DRIVE = 137;
const byte SENSORS = 142;
const byte POWER = 133;


int command = 0;  // input from computer to drive roomba
int ZERO = 0;   // fix to send 0x00
int wb = 258;   // roomba wheelbase diameter

void setup() {
  Serial.begin(9600);
  rooSerial.begin(57600);
  pinMode(ddPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // wake up roomba
  on();
  
  safe();
// roomba should now be in SAFE mode

  // blink led to let know we are ready?  
  for (int i=0; i<5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    
    if (command == '8') {
      // move forward 30 cm
      forward(300); // currently one speed
    }
    
    if (command == '2') {
      reverse();
    }
    
    if (command == '5') {
      Stop();
    }
    
    if (command == '4') {
      rotate90ccw();
    }
    
    if (command == '6') {
      rotate90cw();
    }
    
    if (command == '7') {
      forLeft();
    }
    
    if (command == '9') {
      forRight();
    }
    
    if (command == '1') {
      revLeft();
    }
    
    if (command == '3') {
      revRight();
    }
    
    if (command == '+') {
      on();
    }
    
    if (command == '-') {
      off();
    }
    
    if (command == '*') {
      safe();
    } 
  }
}

// currently only one speed (300 mm/s = 012c in hex)
// distance in mm
void forward(int distance) {
  Serial.println("forward");
  rooSerial.write(DRIVE);
  rooSerial.write(0x01);
  rooSerial.write(0x2c);
  rooSerial.write(0x80);
  rooSerial.write(ZERO);
  delay(distance*10/3);  // times 1000 for msec and /300 for speed
  Stop();
}

// one speed (-300 mm/s = fed4 in hex)
void reverse() {
  rooSerial.write(DRIVE);
  rooSerial.write(0xfe);
  rooSerial.write(0xd4);
  rooSerial.write(0x80);
  rooSerial.write(ZERO);
}

void Stop() {
  Serial.println("stopped");
  rooSerial.write(DRIVE);
  rooSerial.write(ZERO);
  rooSerial.write(ZERO);
  rooSerial.write(0x80);
  rooSerial.write(ZERO);
}

void rotate90cw() {
  rooSerial.write(DRIVE);
  rooSerial.write(0x01);
  rooSerial.write(0xf4);
  rooSerial.write(0xff);
  rooSerial.write(0xff);
  delay(1000*PI*wb/4/300);
  Stop();
}

void rotate90ccw() {
  rooSerial.write(DRIVE);
  rooSerial.write(0x01);
  rooSerial.write(0xf4);
  rooSerial.write(ZERO);
  rooSerial.write(0x01);
  delay(1000*PI*wb/4/300);
  Stop();
}
  
void forLeft() {
}

void forRight() {
}

void revLeft() {
}

void revRight() {
}

void safe() {
  rooSerial.write(START);
  delay(50);
  rooSerial.write(CONTROL);
  delay(50);
  Serial.println("safe mode");
}

void off() {
  rooSerial.write(POWER);
  delay(50);
  Serial.println("powering off");
}

void on() {
  digitalWrite(ddPin, LOW);
  delay(100);
  digitalWrite(ddPin, HIGH);
  delay(100);
  Serial.println("powering on");
}
