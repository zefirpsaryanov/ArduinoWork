#include <Wire.h>
#include <OBD.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <ESP32Lib.h>
#include <Ressources/CodePage437_8x19.h>
#include "opel.h"
#include "combi.h"

const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

//VGA Device
VGA3Bit vga;

COBDI2C obd;
double speedTMP;
double distance;
double fuelRate;
double fuelTMP;
int RPM;
int COOLANT;
int RUNTIME;
int VOLTAGE;
int AMBIENT_TEMP;


TinyGPSPlus gps;
HardwareSerial SerialGPS(2);

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
    vga.println("DTC ERR:NO");
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
  if (value1 >= 0 && value1 < threshold1)
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
    // white
  }
  if (value1 >= threshold1 && value1 < threshold2)
  {
    vga.setTextColor(vga.RGB(255, 255, 0), vga.RGB(0, 0, 0));
    // yellow
  }
  if (value1 >= threshold2 && value1 < threshold3)
  {
    vga.setTextColor(vga.RGB(0, 255, 0), vga.RGB(0, 0, 0));
    // green
  }
  if (value1 >= threshold3)
  {
    vga.setTextColor(vga.RGB(255, 0, 0), vga.RGB(0, 0, 0));
    // red
  }
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

void dashInfo()
{

  // SPEED
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

  // RPM
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(60, 4);
  vga.print("RPM");
  vga.setCursor(60, 28);
  setColorByValue(RPM, 3000, 3300, 3600);
  vga.print(RPM, 4);

  // COOLANT
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(110, 4);
  vga.print("WATER");
  vga.setCursor(110, 28);
  vga.print(COOLANT);

  // L/100KM

  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(160, 4);
  vga.print("L/KM");

  if (speedTMP <= 0) speedTMP = 0.01;
  if (fuelTMP > 30.00) fuelTMP = 30.00;
  fuelTMP = (fuelRate / speedTMP) / 0.036;

  if (fuelTMP < 10) vga.print("  ");
  if (fuelTMP > 1)
  {
    vga.setCursor(160, 28);
    setColorByValue(fuelTMP, 5, 7, 9);
    vga.print(fuelTMP, 2);
  }

  if (fuelRate <= 0) fuelRate = 0.01;

  // 2NR ROW -----------

  // RUNTIME

  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(10, 52);
  vga.print("RUN");
  vga.setCursor(10, 76);
  vga.print(RUNTIME / 60, 3);

  // distance
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(60, 52);
  vga.print("DIST");
  vga.setCursor(60, 76);
  vga.print(distance / 36000, 1);

  // VOLTS
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(110, 52);
  vga.print("V+/-");
  vga.setCursor(110, 76);
  vga.print(VOLTAGE, 2);

  // AMBIENT_TEMP
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(160, 52);
  vga.print("C *");
  vga.setCursor(160, 76);
  vga.print(AMBIENT_TEMP, 2);

  vga.rect(0, 0, 200, 96, 6);

}

void bottom()
{
  vga.rect(0, 96, 200, 54, 6);
  if (gps.satellites.isValid())
  {
    vga.setCursor(10, 98);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.print("GPS FIX:");
    setColorByValue(gps.satellites.value(), 5, 7, 15);
    vga.print(gps.satellites.value());
    if (gps.satellites.value() < 10) vga.print(" ");
  }

  if (!gps.satellites.isValid() || gps.satellites.value() < 1 )
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 98);
    vga.print("GPS FIX:NO");
  }

  if (gps.location.isValid())
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 114);
    vga.print(gps.location.lat(), 6);
    vga.setCursor(10, 130);
    vga.print(gps.location.lng(), 6);
  }

  if (!gps.location.isValid())
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 114);
    vga.print("NOFIX");
  }

  if (gps.date.isValid())
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

  if (!gps.date.isValid())
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 98);
    vga.print("NOFIX");
  }

  if (gps.time.isValid())
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

  if (!gps.time.isValid())
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 114);
    vga.print("NOFIX");
  }
}

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600, SERIAL_8N1, 17, 16);
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE200x150, redPin, greenPin, bluePin, hsyncPin, vsyncPin);

  vga.setFont(CodePage437_8x19);
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font

  sprites.draw(vga, (millis() / 50) % 20, vga.xres / 2, vga.yres / 2);
  //combi.draw(vga, (millis() / 50) % 20, vga.xres / 2, vga.yres / 2);
  delay(5000);
  vga.clear();

  vga.setCursor(40, 65);
  vga.print("WELCOME TO OPEL");

  delay(1000);
  //obd.begin();

  vga.setCursor(55, 80);
  vga.print("Connecting...");
  delay(1000);
  //while (!obd.init());
  vga.clear();

  bottom();
  //circlesInfo();
  showDTC();

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
      if (value < 8000) RPM = value;
      break;

    case PID_COOLANT_TEMP:
      if (value < 130) COOLANT = value;
      break;

    case PID_ENGINE_FUEL_RATE:
      if (value < 200) fuelRate = (double)value;
      break;

    case PID_RUNTIME:
      RUNTIME = value;
      break;

    case PID_CONTROL_MODULE_VOLTAGE:
      VOLTAGE = value;
      break;

    case PID_AMBIENT_TEMP:
      AMBIENT_TEMP = value;
      break;
  }
}

void debugInfo()
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
  // This sketch displays information every time a new sentence is correctly encoded.
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      distance += gps.speed.kmph();
      debugInfo();
      //circlesInfo();
      dashInfo();
      bottom();

    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
      static byte pids[] = {PID_RPM, PID_COOLANT_TEMP, PID_ENGINE_FUEL_RATE, PID_RUNTIME, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP};
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
