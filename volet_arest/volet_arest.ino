/*
  This a simple example of the aREST Library for Arduino (Uno/Mega/Due/Teensy)
  using the Ethernet library (for example to be used with the Ethernet shield).
  See the README file for more details.
  Written in 2014 by Marco Schwartz under a GPL license.
  appel type http://192.168.1.73/led?params=coucou
*/

// Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>
#include <DiOremote.h> //commande 433mhz
#include "confVolets.h" //conf externalisée

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// IP address in case DHCP fails
IPAddress ip(192,168,2,2);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

// Variables to be exposed to the API
int temperature;
int humidity;

// Declare functions to be exposed to the API
int ledControl(String command);
int voletControl(String command);

//433mhz
const int TX_PIN = 3; //pin tx 433
DiOremote myRemote = DiOremote(TX_PIN);

//temps pour éviter les rebond de requête
unsigned long time=millis();

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Function to be exposed
  rest.function("led",ledControl);
  rest.function("volet",voletControl);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("008");
  rest.set_name("dapper_drake");

  // Start the Ethernet connection and the server
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  // Start watchdog
  wdt_enable(WDTO_4S);
}

void loop() {

  // listen for incoming clients
  EthernetClient client = server.available();
  rest.handle(client);
  wdt_reset();

}

// Custom function accessible by the API
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(6,state);
  Serial.println("command=");
  Serial.println(command);
  
  return 1;
  
}

// custom function volet
int voletControl(String command){
  Serial.println(command);
  Serial.println (time);
  Serial.println (millis());
    
  if (millis()>time) { //évite le rebond
    time=millis()+1000; // met à jour la dernier date de commande
    Serial.println ("ok");
    if(command=="salonUp") myRemote.send(SALON_UP_CODE);
    if(command=="salonDown") myRemote.send(SALON_DOWN_CODE);
    if(command=="terrasseUp") myRemote.send(TERRASSE_UP_CODE);
    if(command=="terrasseDown") myRemote.send(TERRASSE_DOWN_CODE);
    if(command=="allUp"){
      myRemote.send(SALON_UP_CODE);
      myRemote.send(TERRASSE_UP_CODE);
    }
    if(command=="allDown"){
      myRemote.send(SALON_DOWN_CODE);
      myRemote.send(TERRASSE_DOWN_CODE);
    }
  }
  return 1;
}

