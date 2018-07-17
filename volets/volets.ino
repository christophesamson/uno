/*
This example shows how to connect to Cayenne using an Ethernet W5100 shield and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
2. Compile and upload the sketch.
3. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>
#include <DiOremote.h> //commande 433mhz
#include "confVolets.h" //conf externalisée

const int TX_PIN = 3; //pin tx 433

DiOremote myRemote = DiOremote(TX_PIN);

#define VIRTUAL_CHANNEL_SALON_UP 1
#define VIRTUAL_CHANNEL_SALON_DOWN 2
#define VIRTUAL_CHANNEL_TERRASSE_UP 3
#define VIRTUAL_CHANNEL_TERRASSE_DOWN 4
#define ACTUATOR_PIN 4 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.

void setup() {
  Serial.begin(9600);
  pinMode(ACTUATOR_PIN, OUTPUT);
  Cayenne.begin(username, password, clientID);
}

void loop() {
  Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  Cayenne.virtualWrite(0, millis());
  // Some examples of other functions you can use to send data.
  //Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

// This function is called when data is sent from Cayenne.
CAYENNE_IN(VIRTUAL_CHANNEL_SALON_UP)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL_SALON_UP, ACTUATOR_PIN, value);
  Serial.println("ok");
  myRemote.send(SALON_ON_CODE);
  // Write the value received to the digital pin.
//  digitalWrite(ACTUATOR_PIN, value);
}

CAYENNE_IN(VIRTUAL_CHANNEL_SALON_DOWN)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d", "salon down", value);
  Serial.println("ok");
  myRemote.send(SALON_OFF_CODE);
  // Write the value received to the digital pin.
//  digitalWrite(ACTUATOR_PIN, value);
}

CAYENNE_IN(VIRTUAL_CHANNEL_TERRASSE_UP)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL_SALON_UP, ACTUATOR_PIN, value);
  Serial.println("ok");
  myRemote.send(TERRASSE_ON_CODE);
  // Write the value received to the digital pin.
//  digitalWrite(ACTUATOR_PIN, value);
}

CAYENNE_IN(VIRTUAL_CHANNEL_TERRASSE_DOWN)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d", "salon down", value);
  Serial.println("ok");
  myRemote.send(TERRASSE_OFF_CODE);
  // Write the value received to the digital pin.
//  digitalWrite(ACTUATOR_PIN, value);
}
