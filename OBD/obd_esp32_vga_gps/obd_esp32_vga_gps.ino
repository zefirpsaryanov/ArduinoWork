#include <Wire.h>
#include <OBD.h> // PIN 21 22
#include <TinyGPS++.h> // com2 / uart2
#include <HardwareSerial.h> // PIN 16 17
#include <ESP32Lib.h>
#include <Ressources/CodePage437_8x19.h>
#include "opel.h"
#include "combi.h"
#include <OLED_I2C.h>


#define key1 15 //connect wire 4 to pin 15 --> 1
#define key3 0  //connect wire 3 to pin  0 --> 3
#define key4 4  //connect wire 2 to pin  4 --> 4

const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;
const int w = 200;
const int h = 150;
const int clockCenterX = h / 2;
const int clockCenterY = h / 2;

VGA3Bit vga;
COBDI2C obd;
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);

OLED  myOLED(21, 22);
extern uint8_t MediumNumbers[];

double speedTMP;
double distance;
double fuelRate;
double fuelTMP;

int RPM;
int COOLANT;
double ENGINE_FUEL_RATE;
int RUNTIME;
double VOLTAGE;
int AMBIENT_TEMP;
int FUEL_LEVEL;
int ENGINE_OIL_TEMP;

/*
     0 -> black
     1 -> red
     2 -> green
     3 -> yellow
     4 -> blue
     5 -> pink
     6 -> lite blue
     7 -> white


  vga.line(0, 0, 198, 0, 4); // - up
  vga.line(0, 150, 0, 0, 4);  // - left
  vga.line(0, 149, 198, 149, 4); // - down
  vga.line(198, 0, 198, 150, 4); // - right
  vga.line(0, 96, 198, 96, 6); // - line

  draw a line
  vga.print("line(x0,y0,x1,y1,c)");
  draw a rectangle with the given width and height
  vga.print("rect(x, y, w, h, c)");
  draw a filled rectangle
  vga.print("fillRect(x, y, w, h, c)");
  draw a circle with the given radius
  vga.print("circle(x,y,r,c)");
  draw a filled circle
  vga.print("fillCircle(x,y,r,c)");
  draw an ellipse
  vga.print("ellipse(x,y,rx,ry,c)");
  draw a filled ellipse
  vga.print("fillEllipse(x,y,rx,ry,c)");

*/

void showDTC()
{
  uint16_t codes[6];
  int dtcCount = obd.readDTC(codes, 6);

  if (dtcCount == 0)
  {
    vga.setCursor(110, 130);
    vga.print("DTC ERR:NO");
  }
  else
  {
    vga.setCursor(110, 130);
    vga.print("DTC ERR: ");
    vga.print(dtcCount);
  }
}

void setColorByValue(int value1, int threshold1, int threshold2, int threshold3)
{
  if (value1 >= 0 && value1 < threshold1) vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // white
  if (value1 >= threshold1 && value1 < threshold2) vga.setTextColor(vga.RGB(0, 255, 0), vga.RGB(0, 0, 0)); // green
  if (value1 >= threshold2 && value1 < threshold3) vga.setTextColor(vga.RGB(255, 255, 0), vga.RGB(0, 0, 0)); // yellow
  if (value1 >= threshold3) vga.setTextColor(vga.RGB(255, 0, 0), vga.RGB(0, 0, 0)); // red
}

void circlesInfo()
{

  //speed
  vga.circle(50, 51, 50, 6);
  vga.circle(50, 51, 49, 1);
  vga.fillCircle(50, 51, 5, 6);

  vga.line(50, 51, 6, 51, 3); // - line

  //RPM
  vga.circle(149, 51, 50, 6);
  vga.circle(149, 51, 49, 1);
  vga.fillCircle(149, 51, 5, 6);

}

void key15Func() // TO DO
{
  // TO DO
}

void key0Func() // TO DO
{
  // TO DO
}

void key4Func() // TO DO
{
  // TO DO
}

void speedDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(10, 4);
  vga.print("KM/H");
  if (gps.speed.isValid())
  {
    vga.setCursor(10, 28);
    int speedTemp = gps.speed.kmph();
    setColorByValue(speedTemp, 51, 91, 121);
    vga.print(speedTemp);
    if (speedTMP < 10) vga.print("  ");
  }

  if (!gps.speed.isValid())
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 28);
    vga.print("***");
  }
}

void rpmDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(60, 4);
  vga.print("RPM");
  vga.setCursor(60, 28);
  setColorByValue(RPM, 2500, 3000, 3500);
  vga.print(RPM, 4);
}

void coolantDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(110, 4);
  vga.print("WATER");
  vga.setCursor(110, 28);
  vga.print(COOLANT);
}

void fuelCalcs()
{
  if (speedTMP <= 0) speedTMP = 0.1;
  if (fuelTMP >= 30.00) fuelTMP = 30.00;
  if (fuelRate <= 0) fuelRate = 0.01;
  fuelTMP = (fuelRate / speedTMP) / 0.036;
}

void fuelDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(160, 4);
  vga.print("L/KM");
  if (fuelTMP < 10) vga.print("  ");
  vga.setCursor(160, 28);
  setColorByValue(fuelTMP, 5, 7, 9);
  vga.print(fuelTMP, 2);
}

void runtimeDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(10, 52);
  vga.print("RUN");
  vga.setCursor(10, 76);
  vga.print(RUNTIME / 60, 3);
}

void distanceDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(60, 52);
  vga.print("DIST");
  vga.setCursor(60, 76);
  vga.print(distance / 36000, 1);
}

void voltsDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(110, 52);
  vga.print("V+/-");
  vga.setCursor(110, 76);
  vga.print(VOLTAGE, 2);
}

void ambientDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(160, 52);
  vga.print("C *");
  vga.setCursor(160, 76);
  vga.print(AMBIENT_TEMP, 2);
}

void bottom()
{
  if (gps.satellites.isValid()) //print valid GPS Sat Count
  {
    vga.setCursor(10, 98);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.print("GPS FIX:");
    setColorByValue(gps.satellites.value(), 4, 8, 15);
    vga.print(gps.satellites.value());
    if (gps.satellites.value() < 10) vga.print(" ");
  }

  if (!gps.satellites.isValid() || gps.satellites.value() < 1 ) //print NOFIX GPS Sat Count
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 98);
    vga.print("GPS FIX:NO");
  }



  if (gps.location.isValid()) //print valid GPS Location
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 114);
    vga.print(gps.location.lat(), 6);
    vga.setCursor(10, 130);
    vga.print(gps.location.lng(), 6);
  }

  if (!gps.location.isValid()) //print NOFIX GPS Location
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 114);
    vga.print("NOFIX");
  }



  if (gps.date.isValid()) //print valid GPS Date
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 98);
    if (gps.date.day() < 10) vga.print("0");
    vga.print(gps.date.day());
    vga.print(":");
    if (gps.date.month() < 10) Serial.print("0");
    vga.print(gps.date.month());
    vga.print(":");
    vga.print(gps.date.year());
  }

  if (!gps.date.isValid()) //print NOFIX GPS Date
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 98);
    vga.print("NOFIX");
  }

  if (gps.time.isValid()) //print valid GPS Time
  {
    vga.setCursor(110, 114);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    if (gps.time.hour() + 2 < 10) vga.print("0");
    vga.print(gps.time.hour() + 2);
    vga.print(":");
    if (gps.time.minute() < 10) vga.print("0");
    vga.print(gps.time.minute());
    vga.print(":");
    if (gps.time.second() < 10) vga.print("0");
    vga.print(gps.time.second());
  }

  if (!gps.time.isValid()) //print NOFIX GPS Time
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 114);
    vga.print("NOFIX");
  }
}

void oledDisp()
{
  int HH = gps.time.hour();
  int MM = gps.time.minute();
  int SS = gps.time.second();

  String strHH;
  String strMM;
  String strSS;
  String timeToString;

  HH < 10 ? strHH = "0" + String(HH) : strHH = String(HH);
  MM < 10 ? strMM = "0" + String(MM) : strMM = String(MM);
  SS < 10 ? strSS = "0" + String(SS) : strSS = String(SS);

  timeToString =  strHH + "." + strMM + "." + strSS;

  myOLED.update();
  myOLED.setBrightness(255);
  myOLED.setFont(MediumNumbers);

  myOLED.print(timeToString, CENTER, 0);  // time

  myOLED.printNumF(fuelTMP, 1, LEFT, 16);
  myOLED.printNumI(AMBIENT_TEMP, CENTER, 16);
  myOLED.printNumI(COOLANT, RIGHT, 16);
}

void drawMark(int h)
{
  float x1, y1, x2, y2;

  h = h * 30;
  h = h + 270;
  x1 = 45 * cos(h * 0.0174532925);
  y1 = 45 * sin(h * 0.0174532925);
  x2 = 42 * cos(h * 0.0174532925);
  y2 = 42 * sin(h * 0.0174532925);

  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 6);

  float sx = 0, sy = 1;
  uint16_t xx0 = 0, xx1 = 0, yy0 = 0, yy1 = 0;

  for (int i = 0; i < 360; i += 6)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    xx0 = sx * 45 + 47; //??
    yy0 = sy * 45 + 47; //??

    vga.dot(xx0, yy0, 6);
  }

}

void drawClockFrame()
{
  vga.fillCircle(clockCenterX, clockCenterY, clockCenterX - 1, 6); // lite blue
  vga.fillCircle(clockCenterX, clockCenterY, clockCenterX - 3, 0); // black
  vga.fillCircle(clockCenterX, clockCenterY, 3, 1);                // red

  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // white
  vga.setCursor(clockCenterX + 45, clockCenterY - 5); vga.print("3");
  vga.setCursor(clockCenterX - 5, clockCenterY + 45); vga.print("6");
  vga.setCursor(clockCenterX - 45, clockCenterY - 5); vga.print("9");
  vga.setCursor(clockCenterX - 5, clockCenterY - 45); vga.print("12");

  for (int i = 0; i < 12; i++)
  {
    if ((i % 3) != 0)
      drawMark(i);
  }
}

void drawSec(int s) //done
{
  float x1, y1, x2, y2;
  int ps = s - 1;
  if (ps == -1)
    ps = 59;
  ps = ps * 6;
  ps = ps + 270;
  x1 = 40 * cos(ps * 0.0174532925);
  y1 = 40 * sin(ps * 0.0174532925);
  x2 = 2 * cos(ps * 0.0174532925);
  y2 = 2 * sin(ps * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  s = s * 6;
  s = s + 270;
  x1 = 40 * cos(s * 0.0174532925);
  y1 = 40 * sin(s * 0.0174532925);
  x2 = 2 * cos(s * 0.0174532925);
  y2 = 2 * sin(s * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
}

void drawMin(int m) //done
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int pm = m - 1;
  int w = 5;
  if (pm == -1)
    pm = 59;
  pm = pm * 6;
  pm = pm + 270;
  x1 = 35 * cos(pm * 0.0174532925);
  y1 = 35 * sin(pm * 0.0174532925);
  x2 = 2 * cos(pm * 0.0174532925);
  y2 = 2 * sin(pm * 0.0174532925);
  x3 = 10 * cos((pm + w) * 0.0174532925);
  y3 = 10 * sin((pm + w) * 0.0174532925);
  x4 = 10 * cos((pm - w) * 0.0174532925);
  y4 = 10 * sin((pm - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 0);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 0);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 0);
  m = m * 6;
  m = m + 270;
  x1 = 35 * cos(m * 0.0174532925);
  y1 = 35 * sin(m * 0.0174532925);
  x2 =  2 * cos(m * 0.0174532925);
  y2 =  2 * sin(m * 0.0174532925);
  x3 = 10 * cos((m + w) * 0.0174532925);
  y3 = 10 * sin((m + w) * 0.0174532925);
  x4 = 10 * cos((m - w) * 0.0174532925);
  y4 = 10 * sin((m - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 7);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 7);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 7);
}

void drawHour(int h, int m) //done
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int ph = h;
  //int color = 6;
  int w = 7;
  if (m == 0) {
    ph = ((ph - 1) * 30) + ((m + 59) / 2);
  }
  else {
    ph = (ph * 30) + ((m - 1) / 2);
  }
  ph = ph + 270;
  x1 = 30 * cos(ph * 0.0174532925);
  y1 = 30 * sin(ph * 0.0174532925);
  x2 = 2  * cos(ph * 0.0174532925);
  y2 = 2  * sin(ph * 0.0174532925);
  x3 = 10 * cos((ph + w) * 0.0174532925);
  y3 = 10 * sin((ph + w) * 0.0174532925);
  x4 = 10 * cos((ph - w) * 0.0174532925);
  y4 = 10 * sin((ph - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 0);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 0);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 0);
  h = (h * 30) + (m / 2);
  h = h + 270;
  x1 = 30 * cos(h * 0.0174532925);
  y1 = 30 * sin(h * 0.0174532925);
  x2 = 2  * cos(h * 0.0174532925);
  y2 = 2  * sin(h * 0.0174532925);
  x3 = 10 * cos((h + w) * 0.0174532925);
  y3 = 10 * sin((h + w) * 0.0174532925);
  x4 = 10 * cos((h - w) * 0.0174532925);
  y4 = 10 * sin((h - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 7);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 7);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 7);
}

void mainClock()
{
  drawClockFrame();
  drawSec(gps.time.second());
  drawMin(gps.time.minute());
  drawHour(gps.time.hour() + 2, gps.time.minute());
}

void mainFrame()
{
  vga.rect(0, 96, 200, 54, 6);  //??
  vga.rect(0, 0, 200, 96, 6);   //??
}

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600, SERIAL_8N1, 17, 16);
  myOLED.begin(SSD1306_128X32);

  pinMode(key1, INPUT_PULLUP);// set pin as input
  //pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key3, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input

  //obd.begin();

  vga.setFrameBufferCount(2);
  vga.init(vga.MODE200x150, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(CodePage437_8x19);
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  sprites.draw(vga, (millis() / 50) % 20, vga.xres / 2, vga.yres / 2);
  //combi.draw(vga, (millis() / 50) % 20, vga.xres / 2, vga.yres / 2);
  delay(5000);
  vga.clear();

  //while (!obd.init());

  vga.clear();
  mainFrame();

}

void reconnect()
{
  vga.clear();
  vga.setCursor(55, 70);
  vga.print("Reconnecting");
  for (uint16_t i = 0; !obd.init(); i++)
  {
    if (i == 3)
    {
      vga.clear();
    }
    delay(500);
  }
}

void readData(byte pid, int value)
{
  char buf[8];
  switch (pid)
  {
    case PID_RPM:
      if (value < 9000) RPM = value;
      break;

    case PID_COOLANT_TEMP:
      if (value < 200) COOLANT = value;
      break;

    case PID_ENGINE_FUEL_RATE:
      if (value < 3200) fuelRate = (double)value;
      break;

    case PID_RUNTIME:
      RUNTIME = value;
      break;

    case PID_CONTROL_MODULE_VOLTAGE:
      VOLTAGE = (double)value;
      break;

    case PID_AMBIENT_TEMP:
      AMBIENT_TEMP = value;
      break;

    case PID_FUEL_LEVEL:
      FUEL_LEVEL = value;
      break;

    case PID_ENGINE_OIL_TEMP:
      ENGINE_OIL_TEMP = value;
      break;
  }
}

void debugInfo() // GPS Debug Info
{
  Serial.print(F("Sat`s: "));
  if (gps.satellites.isValid()) Serial.print(gps.satellites.value());

  if (!gps.satellites.isValid())
  {
    Serial.print(F(" "));
    Serial.print(F("NO FIX"));
  }

  Serial.print(F(" | "));

  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" / "));
    Serial.print(gps.location.lng(), 6);
  }

  if (!gps.location.isValid()) Serial.print(F("NOFIX"));

  Serial.print(F(" | "));

  if (gps.date.isValid())
  {
    if (gps.date.day() < 10) Serial.print(F("0"));
    Serial.print(gps.date.day());
    Serial.print(F(":"));
    if (gps.date.month() < 10) Serial.print(F("0"));
    Serial.print(gps.date.month());
    Serial.print(F(":"));
    Serial.print(gps.date.year());
  }

  if (!gps.date.isValid()) Serial.print(F("NOFIX"));

  Serial.print(F(" | "));

  if (gps.time.isValid())
  {
    if (gps.time.hour() + 2 < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour() + 2);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
  }

  if (!gps.time.isValid()) Serial.print(F("NOFIX"));

  Serial.print(F(" | "));
  Serial.print(F("Speed: "));

  if (gps.speed.isValid())
  {
    Serial.print(gps.speed.kmph());

  }

  if (!gps.speed.isValid()) Serial.print(F("NO FIX"));

  Serial.print(F(" | "));

  Serial.print(distance / 36000, 3);
  Serial.print(F(" | "));

  Serial.print(fuelRate);
  Serial.print(F(" | "));
  Serial.print(fuelTMP);
  Serial.println(F(" | "));
}

void loop()
{
  mainFrame();
  
  int key1S = digitalRead(key1); // read if key1 is pressed
  int key3S = digitalRead(key3); // read if key3 is pressed
  int key4S = digitalRead(key4); // read if key4 is pressed

  if (!key1S) key15Func();
  if (!key3S) key0Func();
  if (!key4S) key4Func();

  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      distance += gps.speed.kmph();
      oledDisp();
      debugInfo();
      fuelCalcs();
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }

  static byte pids[] = {PID_RPM, PID_RUNTIME, PID_FUEL_LEVEL, PID_ENGINE_FUEL_RATE, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP, PID_COOLANT_TEMP, PID_ENGINE_OIL_TEMP};
  static byte index = 0;
  byte pid = pids[index];
  int value;

  // send a query to OBD adapter for specified OBD-II pid

  if (obd.readPID(pid, value))
  {
    readData(pid, value);
  }

  index = (index + 1) % sizeof(pids);

  if (obd.errors >= 100)
  {
    reconnect();
    setup();
  }
}
