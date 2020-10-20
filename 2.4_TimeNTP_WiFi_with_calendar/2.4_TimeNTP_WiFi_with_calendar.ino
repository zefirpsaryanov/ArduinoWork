
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS D0  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_DC D8  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_RST -1 //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TS_CS D3   //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000
#define BOXSIZE 40

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS);

//#define TFT_BLACK       0x0000  ///<   0,   0,   0
//#define TFT_NAVY        0x000F  ///<   0,   0, 123
//#define TFT_DARKGREEN   0x03E0  ///<   0, 125,   0
//#define TFT_DARKCYAN    0x03EF  ///<   0, 125, 123
//#define TFT_MAROON      0x7800  ///< 123,   0,   0
//#define TFT_PURPLE      0x780F  ///< 123,   0, 123
//#define TFT_OLIVE       0x7BE0  ///< 123, 125,   0
//#define TFT_LIGHTGREY   0xC618  ///< 198, 195, 198
//#define TFT_DARKGREY    0x7BEF  ///< 123, 125, 123
//#define TFT_BLUE        0x001F  ///<   0,   0, 255
//#define TFT_GREEN       0x07E0  ///<   0, 255,   0
//#define TFT_CYAN        0x07FF  ///<   0, 255, 255
//#define TFT_RED         0xF800  ///< 255,   0,   0
//#define TFT_MAGENTA     0xF81F  ///< 255,   0, 255
//#define TFT_YELLOW      0xFFE0  ///< 255, 255,   0
//#define TFT_WHITE       0xFFFF  ///< 255, 255, 255
//#define TFT_ORANGE      0xFD20  ///< 255, 165,   0
//#define TFT_GREENYELLOW 0xAFE5  ///< 173, 255,  41
//#define TFT_PINK        0xFC18  ///< 255, 130, 198

const char ssid[] = "test";
const char pass[] = "1q2w3e4r";

static const char ntpServerName[] = "ntp.comnet.bg";

const int timeZone = 2;     // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;

time_t getNtpTime();

int w = tft.width();  // 240
int h = tft.height(); // 320
int clockCenterX = w / 2;
int clockCenterY = w / 2;
int fontW;
int fontH;

int startDay = 2; // Sunday's value is 1, Saturday is 7
String week1 = "";
String week2 = "";
String week3 = "";
String week4 = "";
String week5 = "";
int newWeekStart = 0;
int monthLength = 0;


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
  //  Serial.print("Local port: ");
  //  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(2);
  tft.setTextSize(2);

  fontW = 6;
  fontH = 11;


  if (!ts.begin())
  {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  ts.setRotation(0);



  drawMainDisplay();
}

time_t prevDisplay = 0;

void loop()
{
  if (ts.bufferEmpty())
  {
    return;
  }

  TS_Point p = ts.getPoint();

  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  p.x = p.x * (-1) + 240; //inverted drawing



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

      //calendarDraw();

    }
  }
}

void TFTClockDisplay()
{
  tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
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
  tft.setTextColor(ILI9341_GREENYELLOW, ILI9341_BLACK);

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

  tft.drawFastVLine(0, 0, 320, ILI9341_CYAN);
  tft.drawFastVLine(239, 0, 320, ILI9341_CYAN);

  //  tft.drawFastVLine(39, 239, 80, ILI9341_CYAN);
  //  tft.setCursor(clockCenterX - 110, clockCenterY + 140); tft.print((char)174);
  //  tft.setCursor(clockCenterX - 110, clockCenterY + 160); tft.print((char)174);
  //
  //  tft.drawFastVLine(199, 239, 80, ILI9341_CYAN);
  //  tft.setCursor(clockCenterX + 110, clockCenterY + 140); tft.print((char)175);
  //  tft.setCursor(clockCenterX + 110, clockCenterY + 160); tft.print((char)175);


  tft.drawFastHLine(0, 0, 239, ILI9341_CYAN);
  tft.drawFastHLine(0, 239, 239, ILI9341_CYAN);
  tft.drawFastHLine(0, 319, 239, ILI9341_CYAN);

  tft.fillCircle(clockCenterX, clockCenterY, clockCenterX - 1, ILI9341_CYAN);
  tft.fillCircle(clockCenterX, clockCenterY, clockCenterX - 5, ILI9341_BLACK);
  tft.fillCircle(clockCenterX, clockCenterY, 3, ILI9341_RED);

  tft.setTextColor(ILI9341_WHITE);

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
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, ILI9341_CYAN);

  float sx = 0, sy = 1;
  uint16_t xx0 = 0, xx1 = 0, yy0 = 0, yy1 = 0;

  for (int i = 0; i < 360; i += 6)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    xx0 = sx * 102 + 120;
    yy0 = sy * 102 + 120;
    // Draw minute markers
    tft.drawPixel(xx0, yy0, ILI9341_WHITE);

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
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, ILI9341_BLACK);
  s = s * 6;
  s = s + 270;
  x1 = 95 * cos(s * 0.0174532925);
  y1 = 95 * sin(s * 0.0174532925);
  x2 = 5 * cos(s * 0.0174532925);
  y2 = 5 * sin(s * 0.0174532925);
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, ILI9341_RED);
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
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, ILI9341_BLACK);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, ILI9341_BLACK);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, ILI9341_BLACK);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, ILI9341_BLACK);
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
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, ILI9341_WHITE);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, ILI9341_WHITE);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, ILI9341_WHITE);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, ILI9341_WHITE);
}

void drawHour(int h, int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int ph = h;
  int color = ILI9341_CYAN;
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
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, ILI9341_BLACK);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, ILI9341_BLACK);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, ILI9341_BLACK);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, ILI9341_BLACK);
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
  tft.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, ILI9341_WHITE);
  tft.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, ILI9341_WHITE);
  tft.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, ILI9341_WHITE);
  tft.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, ILI9341_WHITE);
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

// calculate first day of month

int startDayOfWeek(int y, int m, int d)
{
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d ) % 7;
}

void calendarDraw()
{

  int tempYear = 2019; //year();
  int tempMonth = 12; //month();
  int tempDay = 29; //day();

  int tmpDay;
  int tmpResultDay;

  // get number of days in month
  if (tempMonth == 1 || tempMonth == 3 || tempMonth == 5 || tempMonth == 7 || tempMonth == 8 || tempMonth == 10 || tempMonth == 12)
  {
    monthLength = 31;
  }
  else monthLength = 30;

  if (tempMonth == 2)
  {
    if (tempYear % 4 == 0 && tempYear % 100 != 0 ) monthLength = 29;
    else monthLength = 28;
  }

  startDay = startDayOfWeek(tempYear, tempMonth, tempDay); // Sunday's value is 0

  if (startDay == 6) tmpResultDay = 6;
  if (startDay == 5) tmpResultDay = 5;
  if (startDay == 4) tmpResultDay = 4;
  if (startDay == 3) tmpResultDay = 3;
  if (startDay == 2) tmpResultDay = 2;
  if (startDay == 1) tmpResultDay = 1;
  if (startDay == 0) tmpResultDay = 7;

  switch (startDay)
  {
    case 1:
      // Monday
      week1 = " 1  2  3  4  5  6  7";
      break;

    case 2:
      // Thursday
      week1 = "    1  2  3  4  5  6";
      break;

    case 3:
      // Wednesday
      week1 = "       1  2  3  4  5";
      break;

    case 4:
      // Thursday
      if (monthLength  < 30) week1 = "          1  2  3  4";
      if (monthLength == 30) week1 = "30        1  2  3  4";
      if (monthLength == 31) week1 = "30 31     1  2  3  4";
      break;

    case 5:
      // Friday
      if (monthLength  < 31) week1 = "             1  2  3";
      if (monthLength == 31) week1 = "31           1  2  3";
      break;

    case 6:
      // Saturday
      if (monthLength  < 31) week1 = "                1  2";
      if (monthLength == 31) week1 = "31              1  2";
      break;

    case 0:
      // Sunday
      if (monthLength <= 29) week1 = "                   1";
      if (monthLength == 30) week1 = "30                 1";
      if (monthLength == 31) week1 = "30 31              1";
      break;

  } // end first week

  newWeekStart = (7 - tmpResultDay) + 2;
  const char* newWeek1 = (const char*) week1.c_str();



  // display week 2
  week2 = "";
  for (int f = newWeekStart; f < newWeekStart + 7; f++)
  {
    if (f < 10)
    {
      week2 = week2 +  " " + String(f) + " ";
    }
    else
    {
      week2 = week2 + String(f) + " ";
    }
  }
  const char* newWeek2 = (const char*) week2.c_str();


  // display week 3
  newWeekStart = (14 - tmpResultDay) + 2;
  week3 = "";
  for (int f = newWeekStart; f < newWeekStart + 7; f++)
  {
    if (f < 10)
    {
      week3 = week3 +  " " + String(f) + " ";
    }
    else
    {
      week3 = week3 + String(f) + " ";
    }
  }
  const char* newWeek3 = (const char*) week3.c_str();


  // display week 4
  newWeekStart = (21 - tmpResultDay) + 2;
  week4 = "";
  for (int f = newWeekStart; f < newWeekStart + 7; f++) {
    if (f < 10) {
      week4 = week4 +  " " + String(f) + " ";
    }
    else {
      week4 = week4 + String(f) + " ";
    }
  }
  const char* newWeek4 = (const char*) week4.c_str();


  // display week 5
  week5 = "";
  newWeekStart = (28 - tmpResultDay) + 2;
  // is is February?
  if (newWeekStart > 28 && tempMonth == 2)
  {
    // do nothing unless its a leap year
    if (tempYear % 4 == 0 && tempYear % 100 != 0)
    { // its a leap year
      week5 = "29";
    }
  }
  else
  { // print up to 30 anyway
    if (tempMonth == 2)
    { // its February
      for (int f = newWeekStart; f < 29; f++)
      {
        week5 = week5 + String(f) + " ";
      }
      // is it a leap year
      if (tempYear % 4 == 0 && tempYear % 100 != 0)
      { // its a leap year
        week5 = week5 + "29";
      }
    }
    else
    {
      for (int f = newWeekStart; f < 31; f++)
      {
        week5 = week5 + String(f) + " ";
      }
      // are there 31 days
      if (monthLength == 31 && week5.length() < 7)
      {
        week5 = week5 + "31";
      }
    }
  }
  const char* newWeek5 = (const char*) week5.c_str();

  tft.setCursor(0, 0);
  tft.print("Mo Tu We Th Fr Sa Su");
  tft.println(" ");
  tft.print(week1);
  tft.println(" ");
  tft.println(week2);
  tft.println(week3);
  tft.println(week4);
  tft.println(week5);


  tft.println(" ");
  tft.print(day());
  tft.print(".");
  tft.print(month());
  tft.print(".");
  tft.print(year());

}
