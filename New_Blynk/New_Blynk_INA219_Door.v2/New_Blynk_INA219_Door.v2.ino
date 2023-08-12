#define BLYNK_TEMPLATE_ID "TMPL6YWeYnWi"
#define BLYNK_DEVICE_NAME "Device2"
#define BLYNK_AUTH_TOKEN "6KkSIRkUCGAQ8Z7rJyV8jDZGWzm1Uyx6"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_INA219.h>
#include <ezTime.h>

#define SDA 0
#define SCL 2

Adafruit_INA219 ina219;
Timezone myTZ;

char timeToString[8];
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "test1";
char pass[] = "1q2w3e4r";

float currentFrequency;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float min_current_mA = 100;
float loadvoltage = 0;
float power_mW = 0;
bool isTimeSet = false;

void setup()
{
  Wire.begin(SDA, SCL);
  ina219.begin();
  Blynk.begin(auth, ssid, pass);
  WiFi.hostname("ESP01_INA_219_Door");
  waitForSync();
  myTZ.setLocation(F("Europe/Sofia"));
}

void loop()
{
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  sprints();

  Blynk.run();

  Blynk.virtualWrite(V1, current_mA);
  Blynk.virtualWrite(V2, power_mW);
  Blynk.virtualWrite(V3, loadvoltage);
  Blynk.virtualWrite(V4, busvoltage);
  Blynk.virtualWrite(V5, shuntvoltage);

  if (current_mA > min_current_mA && isTimeSet == false)
  {
    Blynk.virtualWrite(V6, timeToString);
    isTimeSet = true;
  }

  if (current_mA <= min_current_mA)
  {
    Blynk.virtualWrite(V6, "220V");
    isTimeSet = false;
  }

}
void sprints()
{
  sprintf(timeToString, "%02d:%02d", myTZ.hour(), minute());
}
