/*
  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################

  define TFT_CS   15 // CS -> D8
  define TFT_DC   2  // DC -> D4
  define TFT_RST  -1 // RES ->RES
  define TFT_MOSI 13 // SDA ->D7
  define TFT_SCLK 14 // SCL ->D5
  GND-GND
  VCC-3,3V

  #define ST7735_DRIVER      // Define additional parameters below for this display
  #define ST7735_GREENTAB2   // 1.8 128*160
*/

#include <SPI.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <TimeLib.h>
#include <Adafruit_AHT10.h>
#include <TFT_eSPI.h>

#define resetKey 12 //D6

// Set web server port number to 80
ESP8266WebServer server(80);

// NTP Servers:
static const char ntpServerName[] = "ntp.comnet.bg";
int timeZone = 2;     // Central European Time
int hourDST = 0; //summer time hour()

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
//void sendNTPpacket(IPAddress &address);

time_t prevDisplay = 0; // when the digital clock was displayed

Adafruit_AHT10 aht;
float aht10_Temperature;
float aht10_Humidity;
sensors_event_t humidity, temp;

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

char timeToString[8];

uint32_t apiChipId = ESP.getChipId();
int rssi;
int rssiMin = -85; // define maximum strength of signal in dBm
int rssiMax = -30; // define minimum strength of signal in dBm

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
    WiFi.hostname("ESP12-AHT10");
  }

  else
    wifiManager.resetSettings(); // resetSettings

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  pinMode(resetKey, INPUT_PULLUP);// set pin as input
}

void DSTTime()
{
  if (month() >= 3 && month() < 11 && day() >= 25 && weekday() == 1 ) hourDST = hour() + 1;
  else hourDST = hour();
}

void loop()
{
  int resetKeyStatus = digitalRead(resetKey); // read if resetKey is pressed
  if (!resetKeyStatus)
  {
    wifiManager.resetSettings(); // wipe settings
    ESP.restart();
  }

  aht.getEvent(&humidity, &temp);
  aht10_Temperature = temp.temperature;
  aht10_Humidity = humidity.relative_humidity;

  getTimeNow();
  DSTTime();
  printTime();
  printTempHum();
  printOther();

  rssi = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100); //rssi to %

  server.handleClient();
  delay(250);
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

void printTime()
{
  sprintf(timeToString, "%02d%02d", hourDST, minute());
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
  tft.print(temp.temperature, 1);

  tft.setTextFont(2);
  tft.setCursor(115, 65);
  tft.print("o");
  tft.setCursor(115, 82);
  tft.print("C");

  tft.setCursor(0, 100);
  tft.setTextFont(7);
  tft.print(humidity.relative_humidity, 1);
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
  ptr += "<meta http-equiv=\"refresh\" content=\"5\">\n";
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
  ptr += hour();
  ptr += ".";
  ptr += minute();
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
