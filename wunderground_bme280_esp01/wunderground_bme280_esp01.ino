int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial

#include <Wire.h>
//#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

const char* ssid     = "test";
const char* password = "1q2w3e4r";

//char SERVER[] = "rtupdate.wunderground.com";           // Realtime update server - RapidFire
char SERVER [] = "weatherstation.wunderground.com";  //standard server
char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";
char ID [] = "IBURGA20";
char PASSWORD [] = "IqBZAjAY";

Adafruit_BME280 bme;
WiFiClient client;

void setup(void)
{
  Serial.begin(9600);
  Wire.begin();
  bme.begin(0x76);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}

void loop(void)
{
  double tempc = bme.readTemperature();
  double tempf =  (tempc * 9.0) / 5.0 + 32.0;
  double humidity = bme.readHumidity();
  double baromin = bme.readPressure() * 0.0002953;
  double dewptf = (dewPoint((tempf), (humidity)));

  if (DEBUG)
  {
    Serial.println("+++++++++++++++++++++++++");
    Serial.print("tempC= ");
    Serial.print(tempc);
    Serial.println(" *C");
    Serial.print("temp= ");
    Serial.print(tempf);
    Serial.println(" *F");
    Serial.print("baro= ");
    Serial.print(baromin);
    Serial.println(" inHg");
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
    client.print("&baromin=");
    client.print(baromin);
    client.print("&dewptf=");
    client.print(dewptf);
    client.print("&humidity=");
    client.print(humidity);
    //client.print("&action=updateraw");//Standard update
    client.print("&softwaretype=Wemos%20D1%20Mini%20Pro%20+%20BME280&action=updateraw&realtime=1&rtfreq=2.5");//Rapid Fire
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
