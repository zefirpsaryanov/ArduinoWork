#include <Arduino.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_AHTX0.h>
#include <U8g2lib.h>
#include <ezTime.h>

U8G2_SH1107_64X128_F_HW_I2C u8g2(U8G2_R3, U8X8_PIN_NONE);

// Set web server port number to 80
ESP8266WebServer server(80);

Timezone myTZ;

Adafruit_AHTX0 aht;

float aht10_Temperature;
float aht10_Humidity;

char timeToString[8];
char dateToString[8];
char timeToStr[8];

String dayofweek;

// kcWrUMsFmGCQ4fzi_flQSY-UE11xFZgB - dian
// HAmeb2RAOKzCG68cbcDH26IBcbT6fYzt - zefir
// Po-oapMzZwHgRlX5zVnI8OKSfkvxjZHC - zdravko
// OZjty6US2AUwzBcQEmZIY1icI1O7LItm - georgi
// 1a589190cd0e42caa727ea338ed16790 - zefir local

char auth[] = "OZjty6US2AUwzBcQEmZIY1icI1O7LItm";

uint32_t apiChipId = ESP.getChipId();
int rssi;
int rssiMin = -90; // define maximum strength of signal in dBm
int rssiMax = -30; // define minimum strength of signal in dBm

WiFiManager wifiManager;
sensors_event_t humidity, temp;

void setup(void)
{
  Wire.begin(0, 2);

  u8g2.begin();
  aht.begin();

  u8g2.clearBuffer();
  u8g2.setContrast(16); // 0-255
  u8g2.setFont(u8g2_font_6x13_tf);

  u8g2.setCursor(0, 10);
  u8g2.print("Connect to ");
  u8g2.print("ESP_");
  u8g2.print(apiChipId, HEX);
  u8g2.sendBuffer();

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  wifiManager.autoConnect();

  if (wifiManager.autoConnect() != false) // reset if last wifi is not found :)
  {
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);
    server.begin();
    WiFi.hostname("ESP01-AHT10");
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
  aht.getEvent(&humidity, &temp);
  aht10_Temperature = temp.temperature - 1.0;
  aht10_Humidity = humidity.relative_humidity + 4.0;

  rssi = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100); //rssi to %

  sprints();
  oledPrint();

  server.handleClient();

  Blynk.run();
  Blynk.virtualWrite(V1, aht10_Temperature);  //Blynk V1 is for Temperature
  Blynk.virtualWrite(V2, aht10_Humidity);     //Blynk V2 is for Humidity
  Blynk.virtualWrite(V3, rssi);               //Blynk V3 is for RSSI
  delay(250);
}

void oledPrint()
{
  u8g2.clearBuffer();
  u8g2.setContrast(0); // 0-255
  u8g2.setFont(u8g2_font_logisoso26_tn);

  u8g2.setCursor(0, 26);
  u8g2.print(timeToString);

  u8g2.setCursor(0, 64);
  u8g2.print(aht10_Temperature, 1);

  u8g2.setCursor(83, 64);
  u8g2.print(aht10_Humidity, 0);

  //u8g2.setFont(u8g2_font_6x13_t_cyrillic);
  u8g2.setFont(u8g2_font_6x13_tf);

  u8g2.setCursor(83, 10);
  u8g2.print(dateToString);

  u8g2.setCursor(83, 23);
  //u8g2.print(year());
  u8g2.print(dayofweek);

  u8g2.setCursor(0, 36);
  u8g2.print(WiFi.localIP().toString());

  u8g2.setCursor(83, 36);
  u8g2.print("wifi ");
  u8g2.print(rssi);
  u8g2.sendBuffer();
}

void sprints()
{
  sprintf(timeToString, "%02d:%02d", myTZ.hour(), minute()); //OLED time
  sprintf(dateToString, "%02d.%02d", day(), month()); //OLED date
  sprintf(timeToStr, "%02d.%02d", myTZ.hour(), minute());    //WWW

  if (weekday() == 1) dayofweek = "Sun";
  if (weekday() == 2) dayofweek = "Mon";
  if (weekday() == 3) dayofweek = "Tue";
  if (weekday() == 4) dayofweek = "Wed";
  if (weekday() == 5) dayofweek = "Thu";
  if (weekday() == 6) dayofweek = "Fri";
  if (weekday() == 7) dayofweek = "Sat";
}

void handle_OnConnect()
{
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

  ptr += "<p>Humidity: ";
  ptr += h;
  ptr += " %H</p>";

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
