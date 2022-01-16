#define BLYNK_PRINT Serial

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_INA219.h>

#define SDA 0
#define SCL 2

Adafruit_INA219 ina219;

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

float currentFrequency;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;
bool sent;
float minBatt = 11.00;

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  ina219.begin();
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
  WiFi.hostname("ESP01_INA_219_NET");
}

void loop()
{
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Blynk.run();

  Blynk.virtualWrite(V26, current_mA);
  Blynk.virtualWrite(V27, power_mW);
  Blynk.virtualWrite(V28, loadvoltage);

  if (loadvoltage <= minBatt && sent == false)
  {
    Blynk.email("zefir.psaryanov@gmail.com", "router LOW Battery", ".");
    Blynk.notify("router LOW Battery");
    sent = true;
  }
  if (loadvoltage > minBatt)
  {
    sent = false;
  }
}