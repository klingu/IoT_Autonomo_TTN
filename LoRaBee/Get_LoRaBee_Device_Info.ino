#include <rn2xx3.h>

// Autonomo Serial port definitions.
#define debugSerial SerialUSB
#define loraSerial Serial1

// create an instance of the Library.
rn2xx3 myLora(loraSerial);

void setup() {
  // Put power on the BeeSocket.
  digitalWrite(BEE_VCC, HIGH);

  // Beginning serial connections.
  debugSerial.begin(57600);
  loraSerial.begin(9600);

  // Set baud on lora
  myLora.autobaud();
  
  // Print the info on the Serial Monitor
  debugSerial.println(F("--------------------------------"));
  debugSerial.println("DevEUI? "); debugSerial.print(F("> "));
  debugSerial.println(myLora.hweui());
  debugSerial.println("Version? "); debugSerial.print(F("> "));
  debugSerial.println(myLora.sysver());
  debugSerial.println(F("--------------------------------"));
  
  SerialUSB.begin(9600);
}

void loop() { }
