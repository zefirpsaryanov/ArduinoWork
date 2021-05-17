#include <WiFi.h>

#include <TimeLib.h>

#include <WiFiUdp.h>

#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();

const char ssid[] = "TP-LINK_90F2";
const char pass[] = "30768017";

static const char ntpServerName[] = "ntp.comnet.bg";

const int timeZone = 3;     // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;

time_t getNtpTime();

int w = tft.width();  // 240
int h = tft.height(); // 320
int clockCenterX = w / 2;
int clockCenterY = w / 2;
int fontW;
int fontH;

#define TFT_BLACK       0x0000  ///<   0,   0,   0
#define TFT_NAVY        0x000F  ///<   0,   0, 123
#define TFT_DARKGREEN   0x03E0  ///<   0, 125,   0
#define TFT_DARKCYAN    0x03EF  ///<   0, 125, 123
#define TFT_MAROON      0x7800  ///< 123,   0,   0
#define TFT_PURPLE      0x780F  ///< 123,   0, 123
#define TFT_OLIVE       0x7BE0  ///< 123, 125,   0
#define TFT_LIGHTGREY   0xC618  ///< 198, 195, 198
#define TFT_DARKGREY    0x7BEF  ///< 123, 125, 123
#define TFT_BLUE        0x001F  ///<   0,   0, 255
#define TFT_GREEN       0x07E0  ///<   0, 255,   0
#define TFT_CYAN        0x07FF  ///<   0, 255, 255
#define TFT_RED         0xF800  ///< 255,   0,   0
#define TFT_MAGENTA     0xF81F  ///< 255,   0, 255
#define TFT_YELLOW      0xFFE0  ///< 255, 255,   0
#define TFT_WHITE       0xFFFF  ///< 255, 255, 255
#define TFT_ORANGE      0xFD20  ///< 255, 165,   0
#define TFT_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define TFT_PINK        0xFC18  ///< 255, 130, 198

void setup()
{
  Serial.begin(9600);
  Serial.println("TimeNTP Example");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(0);
  tft.setTextSize(2);
  drawMainDisplay();
  fontW = 6;
  fontH = 11;
}

time_t prevDisplay = 0;

void loop()
{
  if (timeStatus() != timeNotSet)
  {
    if (now() != prevDisplay)
    { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();

      //TFTClockDisplay();

      digitalClock();

      drawSec(second());
      drawMin(minute());
      drawHour(hour(), minute());
    }
  }
}

void TFTClockDisplay()
{
  tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);
  tft.setCursor(0, 0);

  hour() < 10 ?  tft.print("0" + (String)hour()) : tft.print(hour());
  tft.print(":");
  minute() < 10 ?  tft.print("0" + (String)minute()) : tft.print(minute());
  tft.print(":");
  second() < 10 ?  tft.print("0" + (String)second()) : tft.print(second());
  tft.println(" ");

  day() < 10 ?  tft.print("0" + (String)day()) : tft.print(day());
  tft.print(".");
  month() < 10 ?  tft.print("0" + (String)month()) : tft.print(month());
  tft.print(".");
  year() < 10 ?  tft.print("0" + (String)year()) : tft.print(year());
  tft.println(" ");

  if (weekday() == 1) tft.println("Sunday");
  if (weekday() == 2) tft.println("Monday");
  if (weekday() == 3) tft.println("Tuesday");
  if (weekday() == 4) tft.println("Wednesday");
  if (weekday() == 5) tft.println("Thursday");
  if (weekday() == 6) tft.println("Friday");
  if (weekday() == 7) tft.println("Saturday");
  tft.println(" ");

  printTftDigit(hour());
  tft.print(":");
  printTftDigit(minute());
  tft.print(":");
  printTftDigit(second());
  tft.println(" ");

  printTftDigit(day());
  tft.print(".");
  printTftDigit(month());
  tft.print(".");
  printTftDigit(year());
  tft.println(" ");
}

void digitalClock()
{
  tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);

  tft.setCursor(w / 4 + 2 * fontW, w + fontH);
  printTftDigit(hour());
  tft.print(":");
  printTftDigit(minute());
  tft.print(":");
  printTftDigit(second());

  tft.setCursor(w / 4, w + 3 * fontH);
  printTftDigit(day());
  tft.print("/");
  printTftDigit(month());
  tft.print("/");
  printTftDigit(year());

  tft.setCursor(w / 4 + 7 * fontW, w + 5 * fontH);
  if (weekday() == 1) tft.println("SUN");
  if (weekday() == 2) tft.println("MON");
  if (weekday() == 3) tft.println("TUE");
  if (weekday() == 4) tft.println("WED");
  if (weekday() == 5) tft.println("THU");
  if (weekday() == 6) tft.println("FRI");
  if (weekday() == 7) tft.println("SAT");

}

void drawMainDisplay()
{
  tft.drawFastVLine(0, 0, 320, TFT_DARKCYAN);
  tft.drawFastVLine(239, 0, 320, TFT_DARKCYAN);
  tft.drawFastHLine(0, 0, 240, TFT_DARKCYAN);
  tft.drawFastHLine(0, 240, 240, TFT_DARKCYAN);
  tft.drawFastHLine(0, 319, 240, TFT_DARKCYAN);

  for (int i = 0; i < 5; i++)
  {
    tft.drawCircle(clockCenterX, clockCenterY, 119 - i, TFT_DARKCYAN);
  }

  for (int i = 2; i < 5; i++)
  {
    tft.drawCircle(clockCenterX, clockCenterY, i, TFT_DARKCYAN);
  }

  tft.setTextColor(TFT_WHITE);

  tft.setCursor(clockCenterX + 99, clockCenterY - 7); tft.print("3");
  tft.setCursor(clockCenterX - 7, clockCenterY + 95); tft.print("6");
  tft.setCursor(clockCenterX - 109, clockCenterY - 7); tft.print("9");
  tft.setCursor(clockCenterX - 9, clockCenterY - 109); tft.print("12");

  for (int i = 0; i < 12; i++)
  {
    if ((i % 3) != 0)
      drawMark(i);
  }
}

void drawMark(int h)
{
  float x1, y1, x2, y2;

  h = h * 30;
  h = h + 270;
  x1 = 114 * cos(h * 0.0174532925);
  y1 = 114 * sin(h * 0.0174532925);
  x2 = 108 * cos(h * 0.0174532925);
  y2 = 108 * sin(h * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, TFT_DARKCYAN);

  float sx = 0, sy = 1;
  uint16_t xx0 = 0, xx1 = 0, yy0 = 0, yy1 = 0;

  for (int i = 0; i < 360; i += 6)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    xx0 = sx * 102 + 120;
    yy0 = sy * 102 + 120;
    // Draw minute markers
    tft.drawPixel(xx0, yy0, TFT_WHITE);

    // Draw main quadrant dots
    //if(i==0 || i==180) tft.fillCircle(xx0, yy0, 2, ILI9341_CYAN);
    //if(i==90 || i==270) tft.fillCircle(xx0, yy0, 2, ILI9341_CYAN);

  }

}

void drawSec(int s)
{
  float x1, y1, x2, y2;
  int ps = s - 1;
  if (ps == -1)
    ps = 59;
  ps = ps * 6;
  ps = ps + 270;
  x1 = 95 * cos(ps * 0.0174532925);
  y1 = 95 * sin(ps * 0.0174532925);
  x2 = 5 * cos(ps * 0.0174532925);
  y2 = 5 * sin(ps * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, TFT_BLACK);
  s = s * 6;
  s = s + 270;
  x1 = 95 * cos(s * 0.0174532925);
  y1 = 95 * sin(s * 0.0174532925);
  x2 = 5 * cos(s * 0.0174532925);
  y2 = 5 * sin(s * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, TFT_WHITE);
}

void drawMin(int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int pm = m - 1;
  int w = 5;
  if (pm == -1)
    pm = 59;
  pm = pm * 6;
  pm = pm + 270;
  x1 = 80 * cos(pm * 0.0174532925);
  y1 = 80 * sin(pm * 0.0174532925);
  x2 = 5 * cos(pm * 0.0174532925);
  y2 = 5 * sin(pm * 0.0174532925);
  x3 = 30 * cos((pm + w) * 0.0174532925);
  y3 = 30 * sin((pm + w) * 0.0174532925);
  x4 = 30 * cos((pm - w) * 0.0174532925);
  y4 = 30 * sin((pm - w) * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, TFT_BLACK);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, TFT_BLACK);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, TFT_BLACK);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, TFT_BLACK);
  m = m * 6;
  m = m + 270;
  x1 = 80 * cos(m * 0.0174532925);
  y1 = 80 * sin(m * 0.0174532925);
  x2 =  5 * cos(m * 0.0174532925);
  y2 =  5 * sin(m * 0.0174532925);
  x3 = 30 * cos((m + w) * 0.0174532925);
  y3 = 30 * sin((m + w) * 0.0174532925);
  x4 = 30 * cos((m - w) * 0.0174532925);
  y4 = 30 * sin((m - w) * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, TFT_WHITE);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, TFT_WHITE);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, TFT_WHITE);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, TFT_WHITE);
}

void drawHour(int h, int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int ph = h;
  int color = TFT_DARKCYAN;
  int w = 7;
  if (m == 0) {
    ph = ((ph - 1) * 30) + ((m + 59) / 2);
  }
  else {
    ph = (ph * 30) + ((m - 1) / 2);
  }
  ph = ph + 270;
  x1 = 60 * cos(ph * 0.0174532925);
  y1 = 60 * sin(ph * 0.0174532925);
  x2 = 5 * cos(ph * 0.0174532925);
  y2 = 5 * sin(ph * 0.0174532925);
  x3 = 20 * cos((ph + w) * 0.0174532925);
  y3 = 20 * sin((ph + w) * 0.0174532925);
  x4 = 20 * cos((ph - w) * 0.0174532925);
  y4 = 20 * sin((ph - w) * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, TFT_BLACK);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, TFT_BLACK);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, TFT_BLACK);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, TFT_BLACK);
  h = (h * 30) + (m / 2);
  h = h + 270;
  x1 = 60 * cos(h * 0.0174532925);
  y1 = 60 * sin(h * 0.0174532925);
  x2 = 5 * cos(h * 0.0174532925);
  y2 = 5 * sin(h * 0.0174532925);
  x3 = 20 * cos((h + w) * 0.0174532925);
  y3 = 20 * sin((h + w) * 0.0174532925);
  x4 = 20 * cos((h - w) * 0.0174532925);
  y4 = 20 * sin((h - w) * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, TFT_WHITE);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, TFT_WHITE);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, TFT_WHITE);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, TFT_WHITE);
}

void digitalClockDisplay()
{
  printDigits(hour());
  Serial.print(":");
  printDigits(minute());
  Serial.print(":");
  printDigits(second());
  Serial.print(" ");
  printDigits(day());
  Serial.print(".");
  printDigits(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int number)
{
  if (number < 10)
    Serial.print('0');
  Serial.print(number);
}

void printTftDigit(int number)
{
  if (number < 10)
    tft.print('0');
  tft.print(number);
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
void sendNTPpacket(IPAddress &address)
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
