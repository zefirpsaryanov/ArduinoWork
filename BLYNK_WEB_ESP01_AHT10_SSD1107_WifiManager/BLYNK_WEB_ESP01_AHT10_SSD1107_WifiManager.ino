#include <Arduino.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <TimeLib.h>
#include <Adafruit_AHT10.h>
#include <BlynkSimpleEsp8266.h>
#include <U8g2lib.h>
#include <Adafruit_BMP085.h>

#define SDA 0
#define SCL 2

//U8G2_R0  No rotation, landscape
//U8G2_R1 90 degree clockwise rotation
//U8G2_R2 180 degree clockwise rotation
//U8G2_R3 270 degree clockwise rotation

U8G2_SH1107_64X128_F_HW_I2C u8g2(U8G2_R1, U8X8_PIN_NONE);

// Set web server port number to 80
ESP8266WebServer server(80);

static const char ntpServerName[] = "ntp.comnet.bg";

int timeZone = 2; // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
//void sendNTPpacket(IPAddress &address);

time_t prevDisplay = 0; // when the digital clock was displayed

Adafruit_AHT10 aht;
sensors_event_t humidity, temp;

float aht10_Temperature;
float aht10_Humidity;

char timeToString[8];
char dateToString[8];
char timeToStr[8];

//char auth[] = "Po-oapMzZwHgRlX5zVnI8OKSfkvxjZHC"; //zdrawko
char auth[] = "OZjty6US2AUwzBcQEmZIY1icI1O7LItm"; //georgi
//char auth[] = "1a589190cd0e42caa727ea338ed16790"; // zefir

uint32_t apiChipId = ESP.getChipId();
int rssi;
int rssiMin = -90; // define maximum strength of signal in dBm
int rssiMax = -30; // define minimum strength of signal in dBm

WiFiManager wifiManager;

void setup(void)
{
  Serial.begin(115200);
  Wire.begin(SDA, SCL);

  aht.begin();
  u8g2.begin();

  u8g2.clearBuffer();
  u8g2.setContrast(0); // 0-255
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
    Udp.begin(localPort);
    setSyncProvider(getNtpTime);
    setSyncInterval(300);
    Blynk.config(auth);
    Blynk.connect();
  }

  else
    wifiManager.resetSettings(); // resetSettings
}

void loop(void)
{
  aht.getEvent(&humidity, &temp);
  aht10_Temperature = temp.temperature - 1;
  aht10_Humidity = humidity.relative_humidity;

  rssi = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100); //rssi to %

  getTimeNow();
  sprints();
  oledPrint();

  server.handleClient();

  Blynk.run();
  Blynk.virtualWrite(V1, aht10_Temperature);  //Blynk V1 is for Temperature
  Blynk.virtualWrite(V2, aht10_Humidity);     //Blynk V2 is for Humidity
  Blynk.virtualWrite(V3, rssi);               //Blynk V2 is for RSSI
  delay(100);
}

void oledPrint()
{
  u8g2.clearBuffer();
  u8g2.setContrast(16); // 0-255
  u8g2.setFont(u8g2_font_logisoso26_tn);

  u8g2.setCursor(0, 26);
  u8g2.print(timeToString);

  u8g2.setCursor(0, 64);
  u8g2.print(aht10_Temperature, 1);

  u8g2.setCursor(85, 64);
  u8g2.print(aht10_Humidity, 0);

  u8g2.setFont(u8g2_font_6x13_tf);
  u8g2.setCursor(85, 10);
  u8g2.print(dateToString);

  u8g2.setCursor(85, 23);
  u8g2.print(year());

  u8g2.setCursor(0, 36);
  u8g2.print(WiFi.localIP().toString());

  u8g2.setCursor(85, 36);
  u8g2.print(rssi);
  u8g2.print("%");
  u8g2.sendBuffer();
}

void getTimeNow()
{
  if (timeStatus() != timeNotSet)
  {
    if (now() != prevDisplay)
    { //update the display only if time has changed
      prevDisplay = now();
    }
  }
}
/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];

      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress & address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void sprints()
{
  sprintf(timeToString, "%02d:%02d", hour(), minute()); //OLED time
  sprintf(dateToString, "%02d.%02d", day(), month());   //OLED date
  sprintf(timeToStr, "%02d.%02d", hour(), minute());    //WWW
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
