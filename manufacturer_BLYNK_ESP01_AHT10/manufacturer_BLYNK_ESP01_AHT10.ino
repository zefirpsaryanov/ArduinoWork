#include <Wire.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_AHT10.h>

#define BLYNK_PRINT Serial
#define SDA 0
#define SCL 2

char auth[] = "kcWrUMsFmGCQ4fzi_flQSY-UE11xFZgB";

// kcWrUMsFmGCQ4fzi_flQSY-UE11xFZgB - dian
// HAmeb2RAOKzCG68cbcDH26IBcbT6fYzt - zefir
// Po-oapMzZwHgRlX5zVnI8OKSfkvxjZHC - zdravko
// OZjty6US2AUwzBcQEmZIY1icI1O7LItm - georgi
// 1a589190cd0e42caa727ea338ed16790 - zefir local
// xDpp7zoQs7FYqGUXxCxfgXQOrktaPHoc - st.bubalov

Adafruit_AHT10 aht;

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  aht.begin();

  WiFi.mode(WIFI_STA);
  WiFiManager wm;

  //reset settings - wipe credentials for testing
  //wm.resetSettings();

  //bool res;
  bool res = wm.autoConnect(); // auto generated AP name from chipid

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    WiFi.hostname("ESP01-AHT10");
    Blynk.config(auth);
    Blynk.connect();
  }
}

void loop()
{
  Blynk.run();
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  Blynk.virtualWrite(V1, temp.temperature - 4.0); //Blynk V1 is for Humidity
  Blynk.virtualWrite(V2, humidity.relative_humidity - 4.0); //Blynk V2 is for Temperature
}
