#include <Wire.h>
#include <OBD.h>

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
  else if (value >= threshold1 && value < threshold2)
  {
    vga.setTextColor(vga.RGB(255, 255, 0), vga.RGB(0, 0, 0));
    // yellow
  }
  else if (value >= threshold2 && value < threshold3)
  {
    vga.setTextColor(vga.RGB(0, 255, 0), vga.RGB(0, 0, 0));
    // green
  }
  else if (value >= threshold3)
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
        setColorByValue(value, 3000, 3500, 4000);
        vga.print(value, 4);

      }
      break;

    case PID_SPEED:
      if (value < 260)
      {
        vga.setCursor(60, 28);
        setColorByValue(value, 55, 95, 135);
        vga.print(value, 3);
      }
      break;

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

    case PID_DISTANCE_WITH_MIL:
      vga.setCursor(60, 76);
      vga.print(distance / 60000, 3);
      break;

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
  vga.setCursor(10, 28);
  vga.print("---");

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
  vga.setCursor(60, 76);
  vga.print("---");

  vga.setCursor(110, 52);
  vga.print("VOLTS");
  vga.setCursor(110, 76);
  vga.print("--");

  vga.setCursor(160, 52);
  vga.print("C *");
  vga.setCursor(160, 76);
  vga.print("--");

  vga.setCursor(10, 100);
  vga.print("VIN:");

  vga.setCursor(10, 125);
  vga.println("Err:");

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
    vga.setCursor(46, 125);
    vga.println("NO");
  }
  else
  {
    vga.setCursor(46, 125);
    //vga.print(dtcCount);
    vga.print(' ');
    for (byte n = 0; n < dtcCount; n++)
    {
      vga.print(' ');
      vga.print(codes[n], HEX);
    }
  }
}

void setup()
{
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE200x150, redPin, greenPin, bluePin, hsyncPin, vsyncPin);

  vga.setFont(CodePage437_8x19);
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font

  sprites.draw(vga, (millis() / 50) % 20, vga.xres / 2, vga.yres / 2);
  delay(5000);
  vga.clear();


  vga.setCursor(40, 65);
  vga.print("WELLCOME TO OPEL");

  delay(1000);
  //obd.begin();

  vga.setCursor(55, 80);
  vga.print("Connecting...");
  delay(1000);
  //while (!obd.init());

  initScreen();
  showVIN();
  showDTC();
}

void loop()
{
  static byte pids[] = {PID_SPEED, PID_RPM, PID_COOLANT_TEMP, PID_MAF_FLOW, PID_RUNTIME, PID_DISTANCE_WITH_MIL, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP};
  static byte index = 0;
  byte pid = pids[index];
  int value;
  // send a query to OBD adapter for specified OBD-II pid
  if (obd.readPID(pid, value))
  {
    showData(pid, value);
    distance += (speedTMP * 0.66);
  }
  index = (index + 1) % sizeof(pids);

  if (obd.errors >= 3000)
  {
    reconnect();
    setup();
  }
}
