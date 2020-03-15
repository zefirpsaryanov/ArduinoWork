#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
}

void loop()
{
  Blynk.run();

  Blynk.virtualWrite(V5, dht.readHumidity());
  Blynk.virtualWrite(V6, dht.readTemperature());
  //Serial.println(dht.readHumidity());
  //Serial.println(dht.readTemperature());
}
