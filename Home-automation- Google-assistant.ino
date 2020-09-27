#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WIFI_SSID "Nonaberry"
#define WIFI_PASS "7756919600"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "Priyanka_123"
#define MQTT_PASS "68a1bda801b5417a8693e5617287272a"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe priya = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/priya");
Adafruit_MQTT_Subscribe priya1 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/priya1");
void setup()
{
   pinMode(16, OUTPUT); 
   pinMode(4, OUTPUT);
   pinMode(0, OUTPUT);
   pinMode(2, OUTPUT);
  Serial.begin(9600);

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
              digitalWrite(16,HIGH);
              delay(100);   
              digitalWrite(16,LOW);
    delay(100);
  }

            //On Successful connection to WiFi
            Serial.println("");
            Serial.println("WiFi successfully connected!");
            delay(500);
            

  //Subscribe to the onoff topic
  mqtt.subscribe(&priya);
  mqtt.subscribe(&priya1);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //Connect/Reconnect to MQTT
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &priya)
    {
      //Print the new value to the serial monitor
      Serial.print("priya: ");
      Serial.println((char*) priya.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) priya.lastread, "ON"))
      {
        //active low logic
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    if (subscription == &priya1)
    {
      //Print the new value to the serial monitor
      Serial.print("priya1: ");
      Serial.println((char*) priya1.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) priya1.lastread, "ON"))
      {
        //active low logic
        digitalWrite(16, LOW);
      }
      else
      {
        digitalWrite(16, HIGH);
      }
    }
  }

  // ping the server to keep the mqtt connection alive<br> 
if (!mqtt.ping()){ mqtt.disconnect();}}

  void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) 
    {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
                  //LED AT D2 WILL BLINK DENOTING SUCCESSFUL MQTT CONNECTION
                for(int i=0;i<3;i++)
                {
                digitalWrite(2,HIGH);
                delay(100);   
                digitalWrite(2,LOW);
                delay(100);
                }
}