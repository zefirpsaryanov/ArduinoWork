#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "8BgNruc0_ODfycCitMdO1RHa8PrrTWRo";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

char SERVER[] = "rtupdate.wunderground.com";           // Realtime update server - RapidFire
//char SERVER [] = "weatherstation.wunderground.com";  //standard server
char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";
char ID [] = "IBURGA20";
char PASSWORD [] = "IqBZAjAY";

void setup(void)
{
  Serial.begin(9600);
  dht.begin();
  WiFi.hostname("ESP01-outdoor");
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
}

void loop(void)
{
  Blynk.run();
  Blynk.virtualWrite(V1, dht.readHumidity() - 6);
  Blynk.virtualWrite(V2, dht.readTemperature() - 1);

  double tempc = dht.readTemperature() - 1;
  double tempf =  (tempc * 9.0) / 5.0 + 32.0;
  double humidity = dht.readHumidity() - 6;
  double dewptf = (dewPoint((tempf), (humidity)));

  if (client.connect(SERVER, 80))
  {
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
    //client.print("&action=updateraw");//Standard update
    client.print("&softwaretype=Wemos%20D1%20Mini%20Pro%20+%20BME280&action=updateraw&realtime=1&rtfreq=2.5");//Rapid Fire
    client.print(" HTTP/1.0\r\n");
    client.print("Accept: text/html\r\n");
    client.print("Host: ");
    client.print(SERVER);
    client.print("\r\n\r\n");
    client.println();
  }
  else
  {
    return;
  }
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
