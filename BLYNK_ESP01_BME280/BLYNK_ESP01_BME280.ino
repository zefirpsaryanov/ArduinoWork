#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_BME280.h>
#define SDA 0 // pin declaration
#define SCL 2

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

Adafruit_BME280 bme;

float h = 0;
float t = 0;

void setup()
{
  Serial.begin(9600);
  
  Wire.pins(SDA, SCL);  //I2C declare pins
  Wire.begin(SDA, SCL); //I2C begin
  
  bme.begin(0x76);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
  //  Blynk.begin(auth, ssid, pass, IPAddress(217, 174, 61, 217), 9444);
}

void loop()
{
  Blynk.run();
  h = bme.readHumidity();
  t = bme.readTemperature() - 1;
  Blynk.virtualWrite(V15, h);  //Blynk V15 is for Humidity
  Blynk.virtualWrite(V16, t);  //Blynk V16 is for Temperature
}
