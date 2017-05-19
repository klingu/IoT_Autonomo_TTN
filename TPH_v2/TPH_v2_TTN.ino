/*
      Basic sketch for connecting
      a Sodaq Autonomo equiped with a
      LoRaBee to The Things Network

      Author: Jeffrey Klink Logeman - https://github.com/klingu/
*/

#include <rn2xx3.h>

// Autonomo Serial port definitions.
#define debugSerial SerialUSB
#define loraSerial Serial1

// create an instance of the Library.
rn2xx3 myLora(loraSerial);

//klingu LoRa Device Settings // Replace with your own ;-)
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

void setup() {
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

  debugSerial.println(F("--------------------------------"));
  debugSerial.println(devAddr);
  debugSerial.println(F("--------------------------------"));
  debugSerial.println(F("Basic sketch for connecting "));
  debugSerial.println(F("to The ThingsNetwork"));
  debugSerial.println(F("--------------------------------"));
  led_off();

  initialize_radio();

  SerialUSB.begin(9600);
  SerialUSB.println(F("BME280 test"));

  if (!bme.begin()) {
    SerialUSB.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  // Get all readings of the TPH_v2 sensor in a comma seperated string
  String reading = takeTPH2Reading();

  // turn on the LED on the Autonomo so the user notice when data is send
  led_on();

  // send the data
  myLora.txUncnf(reading);

  // turn off the LED on the Autonomo so the user notice when data is sent
  led_off();

  // delay it a little bit (5 minutes)
  // but the library manages the real dutycycle check.
  delay(300000);
}


String takeTPH2Reading()
{
  String data = String(bme.readTemperature()) + ", ";
  data += String(bme.readPressure() / 100)  + ", ";
  data += String(bme.readHumidity());
  debugSerial.println(data);

  return data;

  ////MOCK data string if you do not have a TPH_v2 sensor connected to your Autonomo
  //return "21.46, 1037.18, 29.08";
}

void initialize_radio()
{

  myLora.autobaud();

  debugSerial.println("DevEUI? "); debugSerial.print(F("> "));
  debugSerial.println(myLora.hweui());
  debugSerial.println("Version? "); debugSerial.print(F("> "));
  debugSerial.println(myLora.sysver());
  debugSerial.println(F("--------------------------------"));

  debugSerial.println(F("Trying to join TTN"));
  bool join_result = false;

  //// Way to use ABP Authentication
  join_result = myLora.initABP(devAddr, appSKey, nwkSKey);

  //// Way to use OTAA Authentication (More secure)
  //OTAA: initOTAA(String AppEUI, String AppKey);
  //join_result = myLora.initOTAA("0000000000000000", "00000000000000000000000000000000"); // Replace with your own ;-)

  while (!join_result)
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
