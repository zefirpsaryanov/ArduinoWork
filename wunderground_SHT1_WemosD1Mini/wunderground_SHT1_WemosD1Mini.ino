int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial

#include <Wire.h>
#include <ESP8266WiFi.h>
#include "Adafruit_SHT31.h"
#include <BlynkSimpleEsp8266.h>


char auth[] = "1a589190cd0e42caa727ea338ed16790";
const char* ssid     = "test";
const char* password = "1q2w3e4r";

//char SERVER[] = "rtupdate.wunderground.com";           // Realtime update server - RapidFire
char SERVER [] = "weatherstation.wunderground.com";  //standard server
char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";
char ID [] = "IBURGA20";
char PASSWORD [] = "IqBZAjAY";

Adafruit_SHT31 sht31 = Adafruit_SHT31();
WiFiClient client;

void setup(void)
{
  Serial.begin(115200);
  Wire.begin();
  sht31.begin(0x44);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  WiFi.hostname("ESP12_Wunderground");
  Blynk.config(auth, IPAddress(192, 168, 10, 10), 8080);
  Blynk.connect();

}

void loop(void)
{
  double tempc = sht31.readTemperature();
  double tempf =  (tempc * 9.0) / 5.0 + 32.0;
  double humidity = sht31.readHumidity();
  double dewptf = (dewPoint((tempf), (humidity)));
  Blynk.run();
  Blynk.virtualWrite(V26, tempc);  //Blynk V1 is for Temperature
  Blynk.virtualWrite(V27, humidity);     //Blynk V2 is for Humidity

  if (DEBUG)
  {
    Serial.println("+++++++++++++++++++++++++");
    Serial.print("tempC= ");
    Serial.print(tempc);
    Serial.println(" *C");
    Serial.print("temp= ");
    Serial.print(tempf);
    Serial.println(" *F");
    Serial.print("dew point= ");
    Serial.println(dewptf);
    Serial.print("humidity= ");
    Serial.println(humidity);
  }

  //Send data to Weather Underground

  if (client.connect(SERVER, 80))
  {
    if (DEBUG)
    {
      Serial.println("Sending DATA ");
    }
    client.print(WEBPAGE);
    client.print("ID=");
    client.print(ID);
    client.print("&PASSWORD=");
    client.print(PASSWORD);
    client.print("&dateutc=");
    client.print("now");

    client.print("&tempf=");
    client.print(tempf);
    client.print("&dewptf=");
    client.print(dewptf);
    client.print("&humidity=");
    client.print(humidity);
    client.print("&action=updateraw");//Standard update
    client.print("&softwaretype=Wemos%20D1%20Mini%20+%20SHT31&action=updateraw&realtime=1&rtfreq=2.5");//Rapid Fire
    client.print(" HTTP/1.0\r\n");
    client.print("Accept: text/html\r\n");
    client.print("Host: ");
    client.print(SERVER);
    client.print("\r\n\r\n");
    client.println();

    if (DEBUG)
    {
      Serial.println("Upload complete");
    }

  }
  else
  {
    if (DEBUG)
    {
      Serial.println(F("Connection failed"));
    }
    return;
  }
  delay(2500);

}

double dewPoint(double tempf, double humidity)
{
  double A0 = 373.15 / (273.15 + tempf);
  double SUM = -7.90298 * (A0 - 1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1) ;
  SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1) ;
  SUM += log10(1013.246);
  double VP = pow(10, SUM - 3) * humidity;
  double T = log(VP / 0.61078);
  return (241.88 * T) / (17.558 - T);
}
