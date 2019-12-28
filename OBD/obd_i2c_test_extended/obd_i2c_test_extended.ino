#include <Wire.h>
#include <OBD.h>
#include <MicroLCD.h>

LCD_SH1106 lcd;
COBDI2C obd;
static double speedTMP;
static double distance;

void reconnect()
{
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.print("Reconnecting");
  for (uint16_t i = 0; !obd.init(); i++)
  {
    if (i == 3)
    {
      lcd.clear();
    }
    delay(1000);
  }
}

void showData(byte pid, int value)

{
  lcd.setFontSize(FONT_SIZE_MEDIUM);

  switch (pid)
  {

    case PID_SPEED:
      lcd.setCursor(0, 0);
      lcd.printInt(value, 3);
      speedTMP = value;
      break;

    case PID_RPM:
      lcd.setCursor(40, 0);
      lcd.printInt(value, 4);
      break;

    case PID_COOLANT_TEMP:
      lcd.setCursor(75, 0);
      lcd.printInt(value, 2);
      break;

    case PID_MAF_FLOW:
      lcd.setCursor(105, 0);
      lcd.printInt((double)value * 0.8, 2);
      break;

    //---------------------------------------//

    case PID_RUNTIME:
      lcd.setCursor(0, 3);
      lcd.printInt(value / 60, 3); // /60 mins
      break;

    case PID_DISTANCE_WITH_MIL:
      lcd.setCursor(40, 3);
      lcd.printInt((double)distance / 60000, 3);
      break;

    case PID_CONTROL_MODULE_VOLTAGE:
      lcd.setCursor(75, 3);
      lcd.printInt(value, 2);
      break;

    case PID_AMBIENT_TEMP:
      lcd.setCursor(105, 3);
      lcd.printInt(value, 2);
      break;
  }
}

void initScreen()
{
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_SMALL);

  lcd.setCursor(0, 2);
  lcd.print("KM/H");
  lcd.setCursor(33, 2);
  lcd.print("RPM");
  lcd.setCursor(70, 2);
  lcd.print("WATER");
  lcd.setCursor(105, 2);
  lcd.print("FUEL");


  lcd.setCursor(2, 5);
  lcd.print("RUN");
  lcd.setCursor(30, 5);
  lcd.print("DIST");
  lcd.setCursor(70, 5);
  lcd.print("VOLTS");
  lcd.setCursor(107, 5);
  lcd.print("AMB");


}

void setup()
{

  lcd.begin();
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.println("I LOVE MY CAR!");

  delay(500);
  obd.begin();

  lcd.println(" Connecting...");
  delay(300);
  while (!obd.init());

  showDTC();
  delay(2000);

  initScreen();
  showVIN();
}

void showVIN()
{
  char buf[255];
  lcd.setFontSize(FONT_SIZE_SMALL);
  lcd.setCursor(0, 7);
  lcd.print("VIN:");
  if (obd.getVIN(buf, sizeof(buf)))
  {
    lcd.setCursor(25, 7);
    lcd.print(buf);
  }
  else
  {
    lcd.setCursor(24, 7);
    lcd.print(" Can`t read VIN");
  }
}

void showDTC()
{

  unsigned int codes[6];
  byte dtcCount = obd.readDTC(codes, 6);
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.setCursor(0, 0);
  lcd.println("Reading DTC");
  lcd.setCursor(0, 3);
  lcd.println("WAIT ...");
  delay(2000);
  if (dtcCount == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 3);
    lcd.println("No DTC :)");
  }
  else
  {
    lcd.clear();
    lcd.setFontSize(FONT_SIZE_SMALL);
    lcd.setCursor(0, 0);
    lcd.println("DTC Found :(");
    lcd.setCursor(0, 1);
    lcd.println(dtcCount);
    lcd.print(" DTC:");
    lcd.println(" ");
    for (byte n = 0; n < dtcCount; n++)
    {
      lcd.print(' ');
      lcd.print(codes[n], HEX);
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

  if (obd.errors >= 3)
  {
    reconnect();
    setup();
  }
}
