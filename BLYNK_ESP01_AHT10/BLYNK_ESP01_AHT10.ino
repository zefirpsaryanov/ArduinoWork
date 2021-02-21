#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_AHT10.h>
#define SDA 0
#define SCL 2

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

Adafruit_AHT10 aht;

void setup()
{
  Serial.begin(9600);
  Wire.begin(SDA, SCL);
  aht.begin();
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
  //  Blynk.begin(auth, ssid, pass, IPAddress(217, 174, 61, 217), 9444);
  WiFi.hostname("ESP01-Kitchen");
}

void loop()
{
  Blynk.run();
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  Blynk.virtualWrite(V10, temp.temperature);  //Blynk V10 is for Humidity
  Blynk.virtualWrite(V11, humidity.relative_humidity);  //Blynk V11 is for Temperature
}
