#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "5172b8b7bf814e63a2139c1dfe47a8a5";
char ssid[] = "test";
char pass[] = "1q2w3e4r";
const int GP4 = 1;        //D4
WidgetMap myMap(V1);

void setup()
{

  Serial.begin(9600);

  // Initialize the output variables as outputs

  pinMode(GP4, OUTPUT);

  // Set outputs to HIGH (LED OFF)

  digitalWrite(GP4, HIGH);

  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);


  int index = 0;
  float lat = 42.505586;
  float lon = 27.473711;
  myMap.location(index, lat, lon, "value");

}

void loop()
{
  Blynk.run();
}
