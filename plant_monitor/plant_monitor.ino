#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHTPIN 22       // Digital pin 22
#define soil_sensor 32  // Digital pin 32
#define DHTTYPE DHT11   // DHT 11

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

DHT dht(DHTPIN, DHTTYPE);

float h = 0;
float t = 0;
int sensorValue = 0;
int percent = 0;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
  dht.begin();
  WiFi.setHostname("ESP32-Mango");
}

int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 3250, 150, 0, 100);
  return percentValue;
}

void loop()
{
  Blynk.run();

  h = dht.readHumidity() + 5;
  t = dht.readTemperature() - 3;
  sensorValue = analogRead (soil_sensor);
  percent = convertToPercent(sensorValue);


  Blynk.virtualWrite(V17, h);  //V17 is for Humidity
  Blynk.virtualWrite(V18, t);  //V18 is for Temperature
  Blynk.virtualWrite(V19, percent);  //V19 is for Moisture
  Serial.print(percent);
  Serial.print("  ");
  Serial.println(sensorValue);
  delay(300);
}
