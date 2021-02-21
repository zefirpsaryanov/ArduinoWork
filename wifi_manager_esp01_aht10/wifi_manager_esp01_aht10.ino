#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>

#include <Adafruit_AHT10.h>
#define SDA 0
#define SCL 2

Adafruit_AHT10 aht;

// Set web server port number to 80
ESP8266WebServer server(80);
float aht10_Temperature;
float aht10_Humidity;

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  aht.begin();

  WiFiManager wifiManager;

  wifiManager.autoConnect();

  if (wifiManager.autoConnect() != false) // reset if last wifi is not found :)
  {
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);
    server.begin();
    WiFi.hostname("ESP01-AHT10");
  }

  else
    wifiManager.resetSettings(); //connect last known wifi
}

void loop()
{
  server.handleClient();
}

void handle_OnConnect()
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  aht10_Temperature = temp.temperature; // Gets the values of the temperature
  aht10_Humidity = humidity.relative_humidity; // Gets the values of the humidity

  server.send(200, "text/html", SendHTML(aht10_Temperature, aht10_Humidity));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float t, float h)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP01 AHT10 Weather Report</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP01 AHT10 Weather Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += t;
  ptr += " *C</p>";

  ptr += "<p>Humidity: ";
  ptr += h;
  ptr += " %H</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
