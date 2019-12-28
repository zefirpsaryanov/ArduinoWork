#include <Wire.h>
#include <OBD.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <ESP32Lib.h>
#include <Ressources/CodePage437_8x19.h>
#include "opel.h"

//pin configuration
const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

//VGA Device
VGA3Bit vga;


COBDI2C obd;
static double speedTMP;
static double distance;

TinyGPSPlus gps;
HardwareSerial SerialGPS(2);

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

void setColorByValue(int value, int threshold1, int threshold2, int threshold3)
{
  if (value >= 0 && value < threshold1)
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
    // white
  }
  if (value >= threshold1 && value < threshold2)
  {
    vga.setTextColor(vga.RGB(255, 255, 0), vga.RGB(0, 0, 0));
    // yellow
  }
  if (value >= threshold2 && value < threshold3)
  {
    vga.setTextColor(vga.RGB(0, 255, 0), vga.RGB(0, 0, 0));
    // green
  }
  if (value >= threshold3)
  {
    vga.setTextColor(vga.RGB(255, 0, 0), vga.RGB(0, 0, 0));
    // red
  }
}


void showData(byte pid, int value)
{
  char buf[8];
  switch (pid)
  {
    case PID_RPM:

      if (value < 8000)
      {
        vga.setCursor(10, 28);
        setColorByValue(value, 3000, 3500, 3750);
        vga.print(value, 4);

      }
      break;

    //    case PID_SPEED:
    //      if (gps.speed.kmph() < 260)
    //      {
    //        vga.setCursor(60, 28);
    //        setColorByValue(gps.speed.isValid(), 55, 95, 135);
    //        vga.print(value, 3);
    //      }
    //      break;

    case PID_COOLANT_TEMP:
      if (value < 120)
      {
        vga.setCursor(110, 28);
        setColorByValue(value, 90, 95, 100);
        vga.print(value, 2);
      }
      break;

    case PID_MAF_FLOW:
      vga.setCursor(160, 28);
      vga.print((float)value * 0.8, 2);
      break;

    //---------------------------------------//

    case PID_RUNTIME:
      vga.setCursor(10, 76);
      vga.print(value / 60, 3); // /sec 60 -> min
      break;

    //    case PID_DISTANCE_WITH_MIL:
    //      lcd.setCursor(33, 3);
    //      lcd.printInt((double)distance / 60000, 3);
    //      break;

    case PID_CONTROL_MODULE_VOLTAGE:
      vga.setCursor(110, 76);
      vga.print(value, 2);
      break;

    case PID_AMBIENT_TEMP:
      vga.setCursor(160, 76);
      vga.print(value, 2);
      break;
  }
}

void initScreen()
{
  vga.clear();
  //vga.setFont(CodePage437_8x19);

  vga.setCursor(10, 4);
  vga.print("KM/H");
  //vga.setCursor(10, 28);
  //vga.print("---");

  vga.setCursor(60, 4);
  vga.print("RPM");
  vga.setCursor(60, 28);
  vga.print("----");

  vga.setCursor(110, 4);
  vga.print("WATER");
  vga.setCursor(110, 28);
  vga.print("--");

  vga.setCursor(160, 4);
  vga.print("FUEL");
  vga.setCursor(160, 28);
  vga.print("--");

  //------------------------

  vga.setCursor(10, 52);
  vga.print("RUN");
  vga.setCursor(10, 76);
  vga.print("---");

  vga.setCursor(60, 52);
  vga.print("DIST");
  //vga.setCursor(60, 76);
  //vga.print("---");

  vga.setCursor(110, 52);
  vga.print("VOLTS");
  vga.setCursor(110, 76);
  vga.print("--");

  vga.setCursor(160, 52);
  vga.print("C *");
  vga.setCursor(160, 76);
  vga.print("--");


  vga.line(0, 0, 198, 0, 4); // - up
  vga.line(0, 150, 0, 0, 4);  // - left
  vga.line(0, 149, 198, 149, 4); // - down
  vga.line(198, 0, 198, 150, 4); // - right
  vga.line(0, 96, 198, 96, 4); // - line

  /*
     0 -> black
     1 -> red
     2 -> green
     3 -> yellow
     4 -> blue
     5 -> pink
     6 -> lite blue
     7 -> white
  */

}

void showVIN()
{
  char buf[255];
  if (obd.getVIN(buf, sizeof(buf)))
  {
    vga.setCursor(46, 100);
    vga.print(buf);
  }
  else
  {
    vga.setCursor(46, 100);
    //vga.scroll(20 , 3);

    vga.print("Can`t read VIN");
  }
}

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
    //    for (byte n = 0; n < dtcCount; n++)
    //    {
    //      vga.print(' ');
    //      vga.print(codes[n], HEX);
    //    }
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

  initScreen();
  //showVIN();
  showDTC();
}


void displayInfo()
{
  Serial.print(F("Sat`s: "));
  if (gps.satellites.isValid())
  {
    Serial.print(gps.satellites.value());

    vga.setCursor(10, 98);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.print("GPS FIX:");
    setColorByValue(gps.satellites.value(), 5, 7, 15);
    vga.print(gps.satellites.value());
    if (gps.satellites.value() < 10)
    {
      vga.print(" ");
    }
  }

  if (!gps.satellites.isValid() || gps.satellites.value() < 1 )
  {
    Serial.print(F("NO FIX"));
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 98);
    vga.print("GPS FIX:NO");
  }

  Serial.print(F(" | "));

  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" / "));
    Serial.print(gps.location.lng(), 6);

    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 114);
    vga.print(gps.location.lat(), 6);
    vga.setCursor(10, 130);
    vga.print(gps.location.lng(), 6);
  }
  if (!gps.location.isValid())
  {
    Serial.print(F("NOFIX"));
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 114);
    vga.print("NOFIX");
  }

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
    Serial.print(F("NOFIX"));
    vga.setCursor(110, 98);
    vga.print("NOFIX");
  }

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
    Serial.print(F("NOFIX"));
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 114);
    vga.print("NOFIX");
  }


  Serial.print(F(" | "));

  Serial.print(F("Speed: "));

  if (gps.speed.isValid())
  {
    Serial.print(gps.speed.kmph());
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 28);
    int speedTMP = gps.speed.kmph();

    setColorByValue(speedTMP, 51, 91, 121);

    vga.print(gps.speed.kmph(), 1);

    if (speedTMP < 10)
    {
      vga.print("  ");
    }
  }

  if (!gps.speed.isValid())
  {
    Serial.print(F("NO FIX"));
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 28);
    vga.print("***");
  }
  Serial.print(F(" | "));


  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(60, 76);
  vga.print(distance / 60000000, 1);
  Serial.print(distance / 60000000 , 1);
  Serial.println(F(" | "));

}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }


  //    static byte pids[] = {PID_SPEED, PID_RPM, PID_COOLANT_TEMP, PID_MAF_FLOW, PID_RUNTIME, PID_DISTANCE_WITH_MIL, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP};
  //    static byte index = 0;
  //    byte pid = pids[index];
  //    int value;
  //    // send a query to OBD adapter for specified OBD-II pid
  //    if (obd.readPID(pid, value))
  //    {
  //      showData(pid, value);
  distance += gps.speed.kmph();
  //    }
  //    index = (index + 1) % sizeof(pids);
  //
  //
  //    if (obd.errors >= 3000)
  //    {
  //      reconnect();
  //      setup();
  //    }

}
