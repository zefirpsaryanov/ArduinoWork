#include <Arduino.h>
#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  Serial.println(dht.readHumidity());
  Serial.println(dht.readTemperature());
  delay(1000);
}
