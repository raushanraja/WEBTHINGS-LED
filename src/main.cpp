#define ARDUINOJSON_USE_LONG_LONG 1
#define LARGE_JSON_BUFFERS 1



#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char *ssid = "ssid";
const char *password = "password";
bool ledCurrentState = false;


WebThingAdapter *adapter;


const char *deviceTypes[] = {"OnOffSwitch", nullptr};

ThingDevice device("switc", "Led One", deviceTypes);
ThingProperty on("on", "On/Off", BOOLEAN, "OnOffProperty");





void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);


  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }


  adapter = new WebThingAdapter("leds", WiFi.localIP());

  device.addProperty(&on);
  adapter->addDevice(&device);

  adapter->begin();

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
}

void loop()
{
  adapter->update();
  bool one = on.getValue().boolean;
  digitalWrite(LED_BUILTIN, one ? LOW : HIGH);
  ledCurrentState = one;
}