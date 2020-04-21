#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "8BgNruc0_ODfycCitMdO1RHa8PrrTWRo";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();
  WiFi.setPhyMode(WIFI_PHY_MODE_11B);
  WiFi.hostname("ESP01-outdoor");
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
}

void loop()
{
  Blynk.run();
  Blynk.virtualWrite(V1, dht.readHumidity() - 7);
  Blynk.virtualWrite(V2, dht.readTemperature() - 1);
}
