#include <Arduino.h>
#include <Wire.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_BMP085.h>
#include <ezTime.h>

// Set web server port number to 80
ESP8266WebServer server(80);

Timezone myTZ;

Adafruit_BMP085 bmp;

char timeToStr[8];
float bmp_Temperature;
float bmp_Pressure;

// kcWrUMsFmGCQ4fzi_flQSY-UE11xFZgB - dian
// HAmeb2RAOKzCG68cbcDH26IBcbT6fYzt - zefir
// Po-oapMzZwHgRlX5zVnI8OKSfkvxjZHC - zdravko
// OZjty6US2AUwzBcQEmZIY1icI1O7LItm - georgi
// 1a589190cd0e42caa727ea338ed16790 - zefir local
// xDpp7zoQs7FYqGUXxCxfgXQOrktaPHoc - st.bubalov

char auth[] = "xDpp7zoQs7FYqGUXxCxfgXQOrktaPHoc";

uint32_t apiChipId = ESP.getChipId();
int rssi;
int rssiMin = -92; // define maximum strength of signal in dBm
int rssiMax = -27; // define minimum strength of signal in dBm

WiFiManager wifiManager;

void setup(void)
{
  Wire.begin(0, 2);

  bmp.begin();

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  wifiManager.autoConnect();

  if (wifiManager.autoConnect() != false) // reset if last wifi is not found :)
  {
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);
    server.begin();
    WiFi.hostname("ESP01-AHT10_2");
    Blynk.config(auth);
    Blynk.connect();
    waitForSync();
    myTZ.setLocation(F("Europe/Sofia"));
  }
  else
    wifiManager.resetSettings(); // resetSettings
}

void loop(void)
{
  bmp_Temperature = bmp.readTemperature() - 1.5;
  bmp_Pressure = bmp.readPressure() / 100.00;

  rssi = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100); //rssi to %

  server.handleClient();

  Blynk.run();
  Blynk.virtualWrite(V4, bmp_Temperature);  //Blynk V1 is for Temperature
  Blynk.virtualWrite(V5, bmp_Pressure);     //Blynk V2 is for Humidity
  Blynk.virtualWrite(V6, rssi);             //Blynk V3 is for RSSI
  
  sprintf(timeToStr, "%02d.%02d", myTZ.hour(), myTZ.minute());    //WWW
  
  delay(250);
}

void handle_OnConnect()
{
  server.send(200, "text/html", SendHTML(bmp_Temperature, bmp_Pressure));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float t, float p)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta http-equiv=\"refresh\" content=\"60\">\n";
  ptr += "<meta name=\"author\" content=\"Zefir\">\n";
  ptr += "<title>Weather Report</title>\n";
  ptr += "<style>html { font-family: Cursive; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Weather Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += t;
  ptr += " *C</p>";

  ptr += "<p>Pressure: ";
  ptr += p;
  ptr += " Pa</p>";

  ptr += "<p>Wifi Signal: ";
  ptr += rssi;
  ptr += " %</p>";

  ptr += "<p>Time: ";
  ptr += timeToStr;
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
