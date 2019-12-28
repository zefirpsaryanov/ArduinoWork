#include <Wire.h>
#include <OBD.h>
#include <MicroLCD.h>

LCD_SH1106 lcd;
COBDI2C obd;

void reconnect()
{
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.print("Reconnecting");
  //digitalWrite(SD_CS_PIN, LOW);
  for (uint16_t i = 0; !obd.init(); i++) {
    if (i == 5) {
      lcd.clear();
    }
    delay(3000);
  }
}

void showData(byte pid, int value)
{
  switch (pid) {

    case PID_SPEED:
      lcd.setCursor(0, 0);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt((unsigned int)value % 1000, 3);
      break;

    case PID_RPM:
      lcd.setCursor(30, 0);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt((unsigned int)value % 10000, 4);
      break;

    case PID_ENGINE_OIL_TEMP:
      lcd.setCursor(60, 0);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;

    case PID_COOLANT_TEMP:
      lcd.setCursor(90, 0);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;

    //---------------------------------------//

    case PID_RUNTIME:
      lcd.setCursor(0, 3);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 6000, 5); //60*100 min
      break;

    case PID_DISTANCE:
      lcd.setCursor(30, 3);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 1000, 4);
      break;

    case PID_AMBIENT_TEMP:
      lcd.setCursor(60, 3);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;

    case PID_CONTROL_MODULE_VOLTAGE:
      lcd.setCursor(90, 3);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;
    //---------------------------------------//
/*
    case PID_ENGINE_LOAD:
      lcd.setCursor(0, 5);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;

    case PID_ENGINE_TORQUE_PERCENTAGE:
      lcd.setCursor(30, 5);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;

    case PID_MAF_FLOW:
      lcd.setCursor(60, 5);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;

    case PID_INTAKE_TEMP:
      lcd.setCursor(90, 5);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.printInt(value % 100, 3);
      break;

      //---------------------------------------//
      /*
          case getVIN:
            lcd.setCursor(24, 7);
            lcd.setFontSize(FONT_SIZE_SMALL);
            lcd.printInt(value, 4);
            break; *
      */
  }
}


void initScreen()
{
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_SMALL);

  lcd.setCursor(0, 1);
  lcd.print("KM/H");
  lcd.setCursor(30, 1);
  lcd.print("RPM");
  lcd.setCursor(60, 1);
  lcd.print("OIL");
  lcd.setCursor(90, 1);
  lcd.print("WATER");

  lcd.setCursor(0, 3);
  lcd.print("RUN");
  lcd.setCursor(30, 3);
  lcd.print("DIST");
  lcd.setCursor(60, 3);
  lcd.print("AMBI");
  lcd.setCursor(90, 3);
  lcd.print("VOLTS");

  lcd.setCursor(0, 5);
  lcd.print("LOAD");
  lcd.setCursor(30, 5);
  lcd.print("TORQ");
  lcd.setCursor(60, 5);
  lcd.print("MAF");
  lcd.setCursor(90, 5);
  lcd.print("INTAK");

  lcd.setCursor(0, 6);
  lcd.print("---------------------");

  lcd.setCursor(0, 7);
  lcd.print("VIN:");
}
/*
void initScreen2()
{
  //lcd.clear();
  lcd.setFontSize(FONT_SIZE_SMALL);

  lcd.setCursor(0, 0);
  lcd.print("0");
  lcd.setCursor(30, 0);
  lcd.print(0);
  lcd.setCursor(60, 0);
  lcd.print("0");
  lcd.setCursor(90, 0);
  lcd.print("0");

  lcd.setCursor(0, 2);
  lcd.print("0");
  lcd.setCursor(30, 2);
  lcd.print("0");
  lcd.setCursor(60, 2);
  lcd.print("0");
  lcd.setCursor(90, 2);
  lcd.print("0");

  lcd.setCursor(0, 4);
  lcd.print("0");
  lcd.setCursor(30, 4);
  lcd.print("0");
  lcd.setCursor(60, 4);
  lcd.print("0");
  lcd.setCursor(90, 4);
  lcd.print("0");

  lcd.setCursor(24, 7);
  
  lcd.print("OBD.getVin");

}

*/

void setup()
{
  lcd.begin();
  //lcd.setFontSize(FONT_SIZE_SMALL);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.println("I LOVE MY CAR!");

  delay(500);
  obd.begin();

  lcd.println();
  lcd.println(" Connecting...");
  while (!obd.init());
  initScreen();
}

void loop()
{
  static byte pids[] = {PID_SPEED, PID_RPM, PID_ENGINE_OIL_TEMP, PID_COOLANT_TEMP, PID_RUNTIME, PID_DISTANCE, PID_AMBIENT_TEMP, PID_CONTROL_MODULE_VOLTAGE, /*PID_ENGINE_LOAD, PID_ENGINE_TORQUE_PERCENTAGE, PID_MAF_FLOW, PID_INTAKE_TEMP,/*getVIN, getVoltage,*/};
  static byte index = 0;
  byte pid = pids[index];
  int value;
  // send a query to OBD adapter for specified OBD-II pid
  if (obd.readPID(pid, value)) {
    showData(pid, value);
  }
  index = (index + 1) % sizeof(pids);

  if (obd.errors >= 2) {
    reconnect();
    setup();
  }
}
