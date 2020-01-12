#include <ESP32Lib.h>
#include <Ressources/CodePage437_8x19.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>


//pin configuration
const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

const char ssid[] = "test";
const char pass[] = "1q2w3e4r";

static const char ntpServerName[] = "ntp.comnet.bg";
const int timeZone = 2;     // Central European Time
WiFiUDP Udp;
unsigned int localPort = 8888;
time_t getNtpTime();

int w = 400;
int h = 300;

int clockCenterX = w / 2;
int clockCenterY = w / 2;

int fontW;
int fontH;

//VGA Device
VGA3Bit vga;
void setup()
{
  Serial.begin(9600);
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(CodePage437_8x19);
 
  vga.clear();
  vga.rect(0, 0, w, h, 6);


//  vga.setCursor(2, 2);
//  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
//  vga.print("WELLCOME TO OPEL");



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


  fontW = 6;
  fontH = 11;
  

}
time_t prevDisplay = 0;

void TFTClockDisplay()
{
//  tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
//  tft.setCursor(0, 0);
//
//  hour() < 10 ?  tft.print("0" + (String)hour()) : tft.print(hour());
//  tft.print(":");
//  minute() < 10 ?  tft.print("0" + (String)minute()) : tft.print(minute());
//  tft.print(":");
//  second() < 10 ?  tft.print("0" + (String)second()) : tft.print(second());
//  tft.println(" ");
//
//  day() < 10 ?  tft.print("0" + (String)day()) : tft.print(day());
//  tft.print(".");
//  month() < 10 ?  tft.print("0" + (String)month()) : tft.print(month());
//  tft.print(".");
//  year() < 10 ?  tft.print("0" + (String)year()) : tft.print(year());
//  tft.println(" ");
//
//  if (weekday() == 1) tft.println("Sunday");
//  if (weekday() == 2) tft.println("Monday");
//  if (weekday() == 3) tft.println("Tuesday");
//  if (weekday() == 4) tft.println("Wednesday");
//  if (weekday() == 5) tft.println("Thursday");
//  if (weekday() == 6) tft.println("Friday");
//  if (weekday() == 7) tft.println("Saturday");
//  tft.println(" ");
//
//  printTftDigit(hour());
//  tft.print(":");
//  printTftDigit(minute());
//  tft.print(":");
//  printTftDigit(second());
//  tft.println(" ");
//
//  printTftDigit(day());
//  tft.print(".");
//  printTftDigit(month());
//  tft.print(".");
//  printTftDigit(year());
//  tft.println(" ");
}

void digitalClock()
{
//  tft.setTextColor(ILI9341_GREENYELLOW, ILI9341_BLACK);
//
//  tft.setCursor(w / 4 + 2 * fontW, w + fontH);
//  printTftDigit(hour());
//  tft.print(":");
//  printTftDigit(minute());
//  tft.print(":");
//  printTftDigit(second());
//
//  tft.setCursor(w / 4, w + 3 * fontH);
//  printTftDigit(day());
//  tft.print("/");
//  printTftDigit(month());
//  tft.print("/");
//  printTftDigit(year());
//
//  tft.setCursor(w / 4 + 7 * fontW, w + 5 * fontH);
//  if (weekday() == 1) tft.println("SUN");
//  if (weekday() == 2) tft.println("MON");
//  if (weekday() == 3) tft.println("TUE");
//  if (weekday() == 4) tft.println("WED");
//  if (weekday() == 5) tft.println("THU");
//  if (weekday() == 6) tft.println("FRI");
//  if (weekday() == 7) tft.println("SAT");

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
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 6);

  float sx = 0, sy = 1;
  uint16_t xx0 = 0, xx1 = 0, yy0 = 0, yy1 = 0;

  for (int i = 0; i < 360; i += 6)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    xx0 = sx * 102 + 120;
    yy0 = sy * 102 + 120;
    // Draw minute markers
    vga.dot(xx0, yy0, 6);

    // Draw main quadrant dots
    //if(i==0 || i==180) tft.fillCircle(xx0, yy0, 2, ILI9341_CYAN);
    //if(i==90 || i==270) tft.fillCircle(xx0, yy0, 2, ILI9341_CYAN);

  }

}


void drawMainDisplay()
{

  for (int i = 0; i < 5; i++)
  {
   vga.circle(clockCenterX, clockCenterY, 119 - i, 6);
  }

  for (int i = 2; i < 5; i++)
  {
    vga.circle(clockCenterX, clockCenterY, i, 6);
  }

  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font

  vga.setCursor(clockCenterX + 99, clockCenterY - 7); vga.print("3");
  vga.setCursor(clockCenterX - 7, clockCenterY + 95); vga.print("6");
  vga.setCursor(clockCenterX - 109, clockCenterY - 7); vga.print("9");
  vga.setCursor(clockCenterX - 9, clockCenterY - 109); vga.print("12");

  for (int i = 0; i < 12; i++)
  {
    if ((i % 3) != 0)
      drawMark(i);
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
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  s = s * 6;
  s = s + 270;
  x1 = 95 * cos(s * 0.0174532925);
  y1 = 95 * sin(s * 0.0174532925);
  x2 = 5 * cos(s * 0.0174532925);
  y2 = 5 * sin(s * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 6);
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
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 0);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 0);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 0);
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
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 7);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 7);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 7);
}

void drawHour(int h, int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int ph = h;
  int color = 6;
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
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 0);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 0);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 0);
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
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 7);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 7);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 7);
}

void digitalClockDisplay()
{
//  printDigits(hour());
//  Serial.print(":");
//  printDigits(minute());
//  Serial.print(":");
//  printDigits(second());
//  Serial.print(" ");
//  printDigits(day());
//  Serial.print(".");
//  printDigits(month());
//  Serial.print(".");
//  Serial.print(year());
//  Serial.println();
}

void printDigits(int number)
{
  if (number < 10)
    Serial.print('0');
  Serial.print(number);
}

void printTftDigit(int number)
{
//  if (number < 10)
//    tft.print('0');
//  tft.print(number);
//
}

void loop()
{
  if (timeStatus() != timeNotSet)
  {
    if (now() != prevDisplay)
    { //update the display only if time has changed
      prevDisplay = now();
      //digitalClockDisplay();

      //TFTClockDisplay();

      //digitalClock();

      drawSec(second());
      drawMin(minute());
      drawHour(hour(), minute());
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
