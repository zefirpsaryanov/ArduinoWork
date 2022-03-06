#define BLYNK_TEMPLATE_ID           "TMPL6YWeYnWi"
#define BLYNK_DEVICE_NAME           "Device1"
#define BLYNK_AUTH_TOKEN            "-mN5tWEg5hhTmdFxNYp_ZYRaSMIviIA-"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_BME280.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "test";
char pass[] = "1q2w3e4r";

Adafruit_BME280 bme;

float h = 0;
float t = 0;

void setup()
{
  // Debug console
  Serial.begin(9600);
  bme.begin(0x76);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  h = bme.readHumidity();
  t = bme.readTemperature() - 1;
  Blynk.virtualWrite(V1, t);  //V1 is for Temperature
  Blynk.virtualWrite(V2, h);  //V2 is for Humidity
}
