#include <Wire.h>
#include <OBD.h>

COBDI2C obd;
static double speedTMP;
static double distance;
#include "Free_Fonts.h" // Include the header file attached to this sketch

#include "SPI.h"
#include "TFT_eSPI.h"

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

unsigned long drawTime = 0;






void reconnect()
{
  tft.fillScreen(TFT_BLACK);
  tft.print("Reconnecting");
  for (uint16_t i = 0; !obd.init(); i++)
  {
    if (i == 3)
    {
      tft.fillScreen(TFT_BLACK);
    }
    delay(500);
  }
}

void showData(byte pid, int value)

{

  switch (pid)
  {

    case PID_SPEED:
      tft.setCursor(0, 0);
      tft.print((unsigned int)value % 1000, 3);
      speedTMP = (unsigned int)value % 1000;
      break;

    case PID_RPM:
      tft.setCursor(33, 0);
      tft.print((unsigned int)value % 10000, 4);
      break;

    case PID_COOLANT_TEMP:
      tft.setCursor(75, 0);
      tft.print(value, 2);
      break;

    case PID_MAF_FLOW:
      tft.setCursor(105, 0);
      tft.print((double)value * 0.8, 2);
      break;

    /*---------------------------------------*/

    case PID_RUNTIME:
      tft.setCursor(0, 3);
      tft.print(value / 60, 2); // /60 mins
      break;

    case PID_DISTANCE_WITH_MIL:
      tft.setCursor(33, 3);
      tft.print((double)distance / 60000, 3);
      break;

    case PID_CONTROL_MODULE_VOLTAGE:
      tft.setCursor(75, 3);
      tft.print(value, 2);
      break;

    case PID_AMBIENT_TEMP:
      tft.setCursor(105, 3);
      tft.print(value, 2);
      break;
  }
}

void initScreen()
{
  tft.fillScreen(TFT_BLACK);

  tft.setCursor(2, 2);
  tft.print("KM/H");
  tft.setCursor(40, 2);
  tft.print("RPM");
  tft.setCursor(70, 2);
  tft.print("WATER");
  tft.setCursor(105, 2);
  tft.print("FUEL");

  tft.setCursor(5, 5);
  tft.print("RUN");
  tft.setCursor(38, 5);
  tft.print("DIST");
  tft.setCursor(70, 5);
  tft.print("VOLTS");
  tft.setCursor(107, 5);
  tft.print("AMB");
}

void setup()
{
  tft.begin();
  tft.setRotation(1);
  tft.setFreeFont(FF18);




  tft.println("I LOVE MY CAR!");

  delay(500);
  obd.begin();

  tft.println(" Connecting...");
  delay(300);
  //while (!obd.init());

  showDTC();
  delay(2000);

  initScreen();
  showVIN();
}

void showVIN()
{
  char buf[255];
  tft.setCursor(0, 7);
  tft.print("VIN:");
  if (obd.getVIN(buf, sizeof(buf)))
  {
    tft.setCursor(25, 7);
    tft.print(buf);
  }
  else
  {
    tft.setCursor(24, 7);
    tft.print(" Can`t get VIN");
  }
}

void showDTC()
{

  uint16_t codes[6];
  byte dtcCount = obd.readDTC(codes, 6);
  tft.fillScreen(TFT_BLACK);

  tft.setCursor(0, 0);
  tft.println("Reading DTC");
  tft.setCursor(0, 3);
  tft.println("WAIT ...");
  delay(2000);
  if (dtcCount == 0)
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 3);
    tft.println("No DTC :)");
  }
  else
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("DTC Found :(");
    tft.setCursor(0, 1);
    tft.println(dtcCount);
    tft.print(" DTC:");
    tft.println(" ");
    for (byte n = 0; n < dtcCount; n++)
    {
      tft.print(' ');
      tft.print(codes[n], HEX);
    }
  }
}

void loop()
{

  static byte pids[] = {PID_SPEED, PID_RPM, PID_COOLANT_TEMP, PID_MAF_FLOW, PID_RUNTIME, PID_DISTANCE_WITH_MIL, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP, };
  static byte index = 0;
  byte pid = pids[index];
  int value;
  // send a query to OBD adapter for specified OBD-II pid
  if (obd.readPID(pid, value))
  {
    showData(pid, value);
    distance += (float)(speedTMP * 0.9);
  }
  index = (index + 1) % sizeof(pids);

  if (obd.errors >= 3000)
  {
    reconnect();
    setup();
  }
}
