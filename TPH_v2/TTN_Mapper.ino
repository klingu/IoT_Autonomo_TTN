/*
 *    Basic sketch for connecting
 *    a Sodaq Autonomo equiped with a
 *    LoRaBee to The Things Network
 *
 *    Author: Richard Verbruggen - http://vannut.nl
 */

#include <rn2xx3.h>

// Autonomo Serial port definitions.
#define debugSerial SerialUSB
#define loraSerial Serial1

// create an instance of the Library.
rn2xx3 myLora(loraSerial);

//LoRa Device Settings // Replace with your own ;-)
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

void setup()
{
  // Put power on the BeeSocket.
  digitalWrite(BEE_VCC, HIGH);

  // built_in led
  pinMode(LED_BUILTIN, OUTPUT);
  led_on();

  // make sure usb serial connection is available,
  // or after 10s go on anyway for 'headless' use of the
  // node.
  while ((!debugSerial) && (millis() < 10000));

  // beginning serial connections.
  debugSerial.begin(57600);
  loraSerial.begin(9600);

  //
  debugSerial.println(F("--------------------------------"));
  debugSerial.println(F("Basic sketch for connecting "));
  debugSerial.println(F("to The ThingsNetwork"));
  debugSerial.println(F("--------------------------------"));
  led_off();

  initialize_radio();
}

void loop()
{
    led_on();
    debugSerial.println(F("> TXing"));
    myLora.tx("!"); //one byte, blocking function
    led_off();

    // delay it a little bit
    // but the library manages the real dutycycle check.
    delay(200);
}

void initialize_radio()
{
  myLora.autobaud();

  debugSerial.println("DevEUI? ");debugSerial.print(F("> "));
  debugSerial.println(myLora.hweui());
  debugSerial.println("Version?");debugSerial.print(F("> "));
  debugSerial.println(myLora.sysver());
  debugSerial.println(F("--------------------------------"));

  debugSerial.println(F("Trying to join TTN"));
  bool join_result = false;

  //// Way to use ABP Authentication
  join_result = myLora.initABP(devAddr, appSKey, nwkSKey);

  //// Way to use OTAA Authentication (More secure)
  //OTAA: initOTAA(String AppEUI, String AppKey);
  //join_result = myLora.initOTAA("0000000000000000", "00000000000000000000000000000000"); // Replace with your own ;-)

  while(!join_result)
  {
    debugSerial.println("\u2A2F Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(30000); //delay 30s before retry
    join_result = myLora.init();
  }
  debugSerial.println("\u2713 Successfully joined TTN");
}

void led_on()
{
  digitalWrite(LED_BUILTIN, 1);
}

void led_off()
{
  digitalWrite(LED_BUILTIN, 0);
}
