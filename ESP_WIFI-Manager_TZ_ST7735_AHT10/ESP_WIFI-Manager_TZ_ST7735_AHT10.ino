/*
  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/
#include <SPI.h>
#include <WiFiManager.h>
#include <Adafruit_AHTX0.h>
#include <TFT_eSPI.h>
#include <ezTime.h>

#include <BlynkSimpleEsp8266.h>

// Set web server port number to 80
ESP8266WebServer server(80);

Timezone myTZ;

Adafruit_AHTX0 aht;
float aht10_Temperature;
float aht10_Humidity;
sensors_event_t humidity, temp;

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

char timeToString[8];
char timeToStr[8];

char auth[] = "Po-oapMzZwHgRlX5zVnI8OKSfkvxjZHC";

uint32_t apiChipId = ESP.getChipId();
int rssi;
int rssiMin = -91; // define maximum strength of signal in dBm
int rssiMax = -29; // define minimum strength of signal in dBm

WiFiManager wifiManager;

void setup(void)
{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Connect to");
  tft.print("ESP_");
  tft.print(apiChipId, HEX);
  tft.println("");

  aht.begin();

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
    Serial.print("IP number assigned by DHCP is ");
    Serial.println(WiFi.localIP());
  }
  else
    wifiManager.resetSettings(); // resetSettings
}

void loop()
{
  Blynk.run();
  aht.getEvent(&humidity, &temp);
  aht10_Temperature = temp.temperature - 1
  ;
  aht10_Humidity = humidity.relative_humidity + 3;

  Blynk.virtualWrite(V1, aht10_Temperature);  //Blynk V1 is for Temperature
  Blynk.virtualWrite(V2, aht10_Humidity);  //Blynk V2 is for Humidity

  printTime();
  printTempHum();
  printOther();

  rssi = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100); //rssi to %

  server.handleClient();
  delay(250);
}

void printTime()
{
  sprintf(timeToString, "%02d%02d", myTZ.hour(), myTZ.minute());
  sprintf(timeToStr, "%02d.%02d", myTZ.hour(), myTZ.minute());
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextFont(7);
  tft.print(timeToString);
  tft.setTextFont(1);
  tft.setCursor(61, 42);
  tft.print("o");
}

void printTempHum()
{
  tft.setTextFont(7);
  tft.setCursor(0, 50);
  tft.print(aht10_Temperature, 1);

  tft.setTextFont(2);
  tft.setCursor(115, 65);
  tft.print("o");
  tft.setCursor(115, 82);
  tft.print("C");

  tft.setCursor(0, 100);
  tft.setTextFont(7);
  tft.print(aht10_Humidity, 1);
  tft.setTextFont(2);
  tft.setCursor(115, 115);
  tft.print("%");
  tft.setCursor(115, 133);
  tft.print("H");
}
void printOther()
{
  tft.setTextFont(1);
  tft.setCursor(0, 150);
  tft.print(WiFi.localIP().toString());
  tft.setCursor(104, 150);
  tft.print(rssi);
  tft.print("%");
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
  //ptr += hour();
  //ptr += ":";
  //ptr += minute();
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
