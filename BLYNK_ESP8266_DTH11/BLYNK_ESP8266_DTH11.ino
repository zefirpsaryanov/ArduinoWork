#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


char auth[] = "1a589190cd0e42caa727ea338ed16790"; //room1
char ssid[] = "test";  //Enter your WIFI Name
char pass[] = "1q2w3e4r";  //Enter your WIFI Password

#define DHTPIN 2          // Digital pin 2

//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
float h = 0;
float t = 0;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
  dht.begin();
}

void loop()
{
  Blynk.run(); // Initiates Blynk

  h = dht.readHumidity();
  t = dht.readTemperature();

  Blynk.virtualWrite(V15, h);  //V5 is for Humidity
  Blynk.virtualWrite(V16, t);  //V6 is for Temperature
}
