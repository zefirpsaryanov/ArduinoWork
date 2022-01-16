#define BLYNK_PRINT Serial
#include <Arduino.h>
#include <Wire.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
#define SDA 0
#define SCL 2

float ahtX0_Temperature;
float ahtX0_Humidity;

char auth[] = "mVaRSybNBd7HBaXqsGP3YsgijTXJ-vkY";

WiFiManager wifiManager;
sensors_event_t humidity, temp;

void setup(void)
{
  Wire.begin(SDA, SCL);
  aht.begin();

  WiFi.mode(WIFI_STA);
  wifiManager.autoConnect();

  if (wifiManager.autoConnect() != false) // reset if last wifi is not found :)
  {
    WiFi.hostname("AHTX0_1-2");
    Blynk.config(auth, "blynk.techbg.net", 18080); // oop@techbg.net mVaRSybNBd7HBaXqsGP3YsgijTXJ-vkY

    //Blynk.config(auth, "psaryanov.org", 18080); // ptg@ptg.bg f2fiWSpg66hTKVEJIF7DqxZk7Dd1gTVx
    //Blynk.config(auth, IPAddress(192,168,10,10), 8080);

    Blynk.connect();
  }
  else
    wifiManager.resetSettings(); // resetSettings
}

void loop(void)
{
  aht.getEvent(&humidity, &temp);
  ahtX0_Temperature = temp.temperature - 1.5;
  ahtX0_Humidity = humidity.relative_humidity + 0.5;

  Blynk.run();
  Blynk.virtualWrite(V1, ahtX0_Temperature);  //Blynk V1 is for Temperature
  Blynk.virtualWrite(V2, ahtX0_Humidity);     //Blynk V2 is for Humidity
  delay(250);
}