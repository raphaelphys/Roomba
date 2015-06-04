// Roomba Test 2
/* diagnotic test of roomba
*
*  reads the sensors and displays the results to the computer.
*  user chooses the package (0 not available).
*
*  similar to test except adding a new softserial so I may 
*  talk to the computer and the roomba at the same time.
*  This is important for testing. 
*/

#include <SoftwareSerial.h>

// note that the roomba pins are opposite
int rxPin = 10;  // tx for roomba
int txPin = 11;  // rx for roomba
int ddPin = 12;
int ledPin = 13;

SoftwareSerial rooSerial(rxPin, txPin);

int inByte = 0;
byte sensorBytes[10] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1};
int binaryOut[8] = {0,0,0,0, 0,0,0,0};    // number in binary

// roomba commands:
const int START = 128;
const int CONTROL = 130;
const int POWER = 133;

const int LEDS = 139;
const int SENSORS = 142;

const int LEFT = 1;
const int LEFTFRONT = 2;
const int RIGHTFRONT = 3;
const int RIGHT = 4;


void setup() {
  Serial.begin(9600);
  rooSerial.begin(57600);
  pinMode(ddPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(ddPin, LOW);
  delay(100);
  digitalWrite(ddPin, HIGH);
  delay(2000);
  
  rooSerial.write(START);
  delay(50);
  rooSerial.write(CONTROL);
  delay(50);
  
}

void loop() {
  if (Serial.available() > 0) {
    inByte = Serial.read();

// user must type 1,2, or 3 to read the sensor (designates package)
// not available to read entire package (0);
    if (inByte == '1') {
      digitalWrite(ledPin, HIGH);
      reset();
      updateSensors(1);
      digitalWrite(ledPin, LOW);
      pack1Print();
    }
    
    if (inByte == '2') {
      digitalWrite(ledPin, HIGH);
      reset();
      updateSensors(2);
      digitalWrite(ledPin, LOW);
      pack2Print();
    }

    if (inByte == '3') {
      digitalWrite(ledPin, HIGH);
      reset();
      updateSensors(3);
      digitalWrite(ledPin, LOW);
      pack3Print();
    }

  }
}

// reset the variable sensorBytes to -1 before running any update
void reset() {
  for (int i=0; i<10; i++) {
    sensorBytes[i] = -1;
  }
}

// store sensor values in array sensorBytes
// sensor packages: 0=all
//                  1=10bytes (bump/wheeldrop, wall, cliffs(L,LF, RF, R), virt wall, motor I, dirt(L,R))
//                  2=6 bytes (remote, buttons, distance, angle)
//                  3=10bytes (charge status, voltage, current, temp, charge, capacity)
void updateSensors(int package) {
  rooSerial.write(SENSORS);
  rooSerial.write(package);      // choose which sensor package
  delay(100);
  
  int counter = 0;
  while(rooSerial.available()) {
    byte in = rooSerial.read();
    if (in == -1) { // error
    // blink the led to show error
      for (int i=0; i<5; i++) {
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        delay(250);
      }
    }
    sensorBytes[counter++] = in;
  }
}


void pack1Print() {
  Serial.print("wheeldrop(C,L,R) bumps(L,R): ");
//  Serial.print(sensorBytes[0], BIN);
  toBinary(sensorBytes[0]);
  printBinaryOut(5);

  Serial.println();
  Serial.print("                       wall: ");
  Serial.println(sensorBytes[1]);
  Serial.print("                 cliff left: ");
  Serial.println(sensorBytes[2]);
  Serial.print("           cliff front left: ");
  Serial.println(sensorBytes[3]);
  Serial.print("          cliff front right: ");
  Serial.println(sensorBytes[4]);
  Serial.print("                cliff right: ");
  Serial.println(sensorBytes[5]);
  Serial.print("               virtual wall: ");
  Serial.println(sensorBytes[6]);
  Serial.print("          motor overcurrent: ");
  Serial.println(sensorBytes[7]);
  Serial.print("         dirt detector left: ");
  Serial.println(sensorBytes[8]);
  Serial.print("        dirt detector right: ");
  Serial.println(sensorBytes[9]);
  Serial.println();
}

void pack2Print() {
  Serial.print("remote command: ");
  Serial.println(sensorBytes[0]);
  Serial.print("       buttons: ");
  Serial.println(sensorBytes[1]);
  Serial.print("      distance: ");
  Serial.print(sensorBytes[2]);
  Serial.println(sensorBytes[3]);
  Serial.print("         angle: ");
  Serial.print(sensorBytes[4]);
  Serial.println(sensorBytes[5]);
  Serial.println();
}

void pack3Print() {
  Serial.print("charging state: ");
  Serial.println(sensorBytes[0]);
  Serial.print("       voltage: ");
  Serial.print(sensorBytes[1]);
  Serial.println(sensorBytes[2]);
  Serial.print("       current: ");
  Serial.print(sensorBytes[3]);
  Serial.println(sensorBytes[4]);
  Serial.print("   temperature: ");
  Serial.println(sensorBytes[5]);
  Serial.print("        charge: ");
  Serial.print(sensorBytes[6]);
  Serial.println(sensorBytes[7]);
  Serial.print("      capacity: ");
  Serial.print(sensorBytes[8]);
  Serial.println(sensorBytes[9]);
  Serial.println();
}

void toBinary(byte number) {
  for (int i=7; i>0; i--) {
    binaryOut[i] = 1 & number; // saves in reverse order (easy for printing)
    number >>= 1;
  }
}

void printBinaryOut(int number) {
  for (int i=(8-number); i<8; i++) {
    Serial.print(binaryOut[i]);
  }
}
