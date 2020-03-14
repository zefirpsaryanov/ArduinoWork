#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_BME280.h>

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

Adafruit_BME280 bme;
float h = 0;
float t = 0;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
  //  Blynk.begin(auth, ssid, pass, IPAddress(217, 174, 61, 217), 9444);
  bme.begin(0x76);
}

void loop()
{
  Blynk.run();
  t = bme.readTemperature();
  h = bme.readHumidity();
  Serial.println(h);
  Serial.println(t);
  Blynk.virtualWrite(V15, h);  //V5 is for Humidity
  Blynk.virtualWrite(V16, t);  //V6 is for Temperature
}
