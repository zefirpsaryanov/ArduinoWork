#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "8BgNruc0_ODfycCitMdO1RHa8PrrTWRo";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

#define DHTPIN 2          // Digital pin 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
//  Blynk.begin(auth, ssid, pass, IPAddress(217, 174, 61, 217), 9444);
  dht.begin();
}

void loop()
{
  Blynk.run();
  Blynk.virtualWrite(V1, dht.readHumidity());
  Blynk.virtualWrite(V2, dht.readTemperature());
}
