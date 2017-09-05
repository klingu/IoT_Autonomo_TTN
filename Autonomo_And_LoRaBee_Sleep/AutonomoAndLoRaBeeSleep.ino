/*
      Basic sketch for connecting
      a Sodaq Autonomo equiped with a
      LoRaBee and a TPH_v2 sensor 
      to The Things Network

      Author: Jeffrey Klink Logeman - https://github.com/klingu/
*/

#include <Sodaq_RN2483.h>

// Autonomo Serial port definitions.
#define debugSerial SerialUSB
#define loraSerial Serial1

//LoRa Device Settings !!! Use your own
const uint8_t devAddr[4] = { 0x00, 0x00, 0x00, 0x00 };
const uint8_t nwkSKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t appSKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <RTCZero.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
RTCZero rtc;

void setup() {
  // Put power on the BeeSocket.
  digitalWrite(BEE_VCC, HIGH);

  // built_in led
  pinMode(LED_BUILTIN, OUTPUT);
  led_on();

  // make sure usb serial connection is available,
  // or after 10s go on anyway for 'headless' use of the node.
  while ((!debugSerial) && (millis() < 10000));

  // setup deep sleep
  setupDeepSleep();

  // beginning serial connections.
  debugSerial.begin(57600);
  loraSerial.begin(9600);

  debugSerial.println(F("--------------------------------"));
  debugSerial.println(F("Basic sketch for connecting "));
  debugSerial.println(F("to The ThingsNetwork"));
  debugSerial.println(F("--------------------------------"));

  led_off();

  // Connect to the LoRa Network (The Things Network)
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

  if (reading != " nan,  nan,  nan")
  {
    // turn on the LED on the Autonomo so the user notice when data is send
    led_on();

    // send the data
    SerialUSB.println("Start sending data");
    LoRaBee.send(1, (uint8_t*)reading.c_str(), reading.length());
    SerialUSB.println("Done sending data");

    if (debugSerial)
    {
      SerialUSB.println("Delay 10 sec for developer interuption");
      delay(10000);
    }

    // turn off the LED on the Autonomo so the user notice when data is sent
    led_off();

    SerialUSB.println("Start sleep");

    // Set LoRaBee to sleep
    LoRaBee.sleep();

    // Disable USB
    if (!debugSerial) // keep connected to the Serial monitor when debuging
    {
      USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
    }

    //Enter Autonomo sleep mode
    __WFI();

    // ...Sleep

    // Enable USB
    if (!debugSerial)
    {
      USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE;
    }

    // Wake up LoRaBee
    LoRaBee.wakeUp();
    SerialUSB.println("End wake up");
  }
  else {
    SerialUSB.println("Delay 1 sec before retry...");
    delay(1000);
  }
}

void setupDeepSleep()
{
  SerialUSB.println("Setup deep sleep");
  led_on();

  // initialize RTC 24H format. The dual option is H12
  rtc.begin('H24');

  // set the RTC time: minutes and seconds
  rtc.setMinutes(0);
  rtc.setSeconds(0);

  // Set the alarm at the 15 minuts mark
  rtc.setAlarmSeconds(0);
  rtc.setAlarmMinutes(15);

  // Enable the alarm when matching the minutes and seconds
  rtc.enableAlarm(RTCZero::MATCH_MMSS);

  // Method to trigger when alarm is triggered
  rtc.attachInterrupt(setNewAlarm);

  // Set sleep mode
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  led_off();

  // Sleep sketch startup delay
  delay(5000);

  SerialUSB.println("Done Setup deep sleep");
}

void setNewAlarm()
{
  // Reste the RTC-time minutes to '0'
  rtc.setMinutes(0);
}

String takeTPH2Reading()
{
  String data = String(bme.readTemperature()) + ", ";
  data += String(bme.readPressure() / 100)  + ", ";
  data += String(bme.readHumidity());

  SerialUSB.println("Retreived data from BME20: " + data);

  return data;

  ////MOCK data string if you do not have a TPH_v2 sensor connected to your Autonomo
  //return "21.46, 1037.18, 29.08";
}

void initialize_radio()
{
  loraSerial.begin(LoRaBee.getDefaultBaudRate());

  debugSerial.println(F("Trying to join TTN"));
  bool join_result = false;

  //// Way to use ABP Authentication
  join_result = LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey);
  
  while (!join_result)
  {
    debugSerial.println("\u2A2F Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(30000); //delay 30s before retry
    join_result = LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey);
  }
  debugSerial.println("\u2713 Successfully joined TTN");
}

void led_on()
{
  SerialUSB.println("Led is turned on");
  digitalWrite(LED_BUILTIN, 1);
}

void led_off()
{
  SerialUSB.println("Led is turned off");
  digitalWrite(LED_BUILTIN, 0);
}