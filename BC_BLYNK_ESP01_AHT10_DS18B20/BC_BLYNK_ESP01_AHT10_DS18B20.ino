#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_AHTX0.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SDA 4
#define SCL 5

char auth[] = "92byd6RJCSrutGpQ7JSwTEAKjYaSBc4e";
char ssid[] = "O2";
char pass[] = "1q2w3e4r";
int rssi0 = 0;
int rssiMin = -85;  // define minimum strength of signal in dBm
int rssiMax = -25;  // define maximum strength of signal in dBm
String station_name = "station_2_1";
float aht10_Humidity = 0;

const int SENSOR_PIN = 13; // Arduino pin connected to DS18B20 sensor's DQ pin

OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library

float tempCelsius;    // temperature in Celsius


Adafruit_AHTX0 aht;
ESP8266WebServer server(80);

void setup()
{
  Serial.begin(9600);
  Wire.begin(SDA, SCL);
  aht.begin();
  tempSensor.begin();    // initialize the sensor

  WiFi.hostname(station_name);

  Blynk.begin(auth, ssid, pass, IPAddress(84, 54, 187, 239), 8080);

  server.begin();
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
}

void loop()
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  aht10_Humidity = humidity.relative_humidity -4; // Gets the values of the humidity

  tempSensor.requestTemperatures();             // send the command to get temperatures
  tempCelsius = tempSensor.getTempCByIndex(0); // read temperature in Celsius

  rssi0 = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100);  //rssi to %

  Blynk.run();
  // (1,2,3)(4,5,6)(7,8,9)(10,11,12)
  // (13,14,15)(16,17,18)(19,20,21)(22,23,24)(25,26,27,28)
  Blynk.virtualWrite(V1, tempCelsius);
  Blynk.virtualWrite(V2, aht10_Humidity);
  Blynk.virtualWrite(V3, rssi0);

  server.handleClient();
}

void handle_OnConnect()
{
  server.send(200, "text/html", SendHTML(tempCelsius, aht10_Humidity, station_name));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float t, float h, String s_name)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>";
  ptr += s_name;
  ptr += "</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>";
  ptr += s_name;
  ptr += "</h1>\n";

  ptr += "<p>t: ";
  ptr += t;
  ptr += " ";

  ptr += "h: ";
  ptr += h;
  ptr += " </p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}