/*************************************************************************
  Simple OBD Data Display
  Works with any Arduino board connected with SH1106 128*64 I2C OLED and
  Freematics OBD-II UART Adapter - https://freematics.com/products
  Distributed under public domain
  Written by Stanley Huang <stanley@freematics.com.au>
*************************************************************************/

#include <Wire.h>
#include <MicroLCD.h>

LCD_SH1106 lcd;

int PID_RPM; //– Engine RPM (rpm)
int PID_ENGINE_LOAD; // – Calculated engine load (%)
int PID_COOLANT_TEMP; // – Engine coolant temperature (°C)
int PID_ABSOLUTE_ENGINE_LOAD; // – Absolute Engine load (%)
int PID_TIMING_ADVANCE; // – Ignition timing advance (°)
int PID_ENGINE_OIL_TEMP; // – Engine oil temperature (°C)
int PID_ENGINE_TORQUE_PERCENTAGE; // – Engine torque percentage (%)
int PID_ENGINE_REF_TORQUE; // – Engine reference torque (Nm)
int PID_INTAKE_TEMP; // – Intake temperature (°C)
int PID_INTAKE_PRESSURE; // – Intake manifold absolute pressure (kPa)
int PID_MAF_FLOW; // – MAF flow pressure (grams/s)
int PID_BAROMETRIC; // – Barometric pressure (kPa)
int PID_SPEED; // – Vehicle speed (km/h)
int PID_RUNTIME; // – Engine running time (second)
int PID_DISTANCE; // – Vehicle running distance (km)
int PID_THROTTLE; // – Throttle position (%)
int PID_AMBIENT_TEMP; // – Ambient temperature (°C)
int PID_CONTROL_MODULE_VOLTAGE ; //– vehicle control module voltage (V)
int PID_HYBRID_BATTERY_PERCENTAGE ; //– Hybrid battery pack remaining life (%)


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

void initScreen2()
{
  //lcd.clear();
  lcd.setFontSize(FONT_SIZE_SMALL);

  lcd.setCursor(0, 0);
  lcd.print("100");
  lcd.setCursor(30, 0);
  lcd.print(3200);
  lcd.setCursor(60, 0);
  lcd.print("90C");
  lcd.setCursor(90, 0);

  lcd.print("87C");
  lcd.setCursor(0, 2);
  lcd.print("56");
  lcd.setCursor(30, 2);
  lcd.print("48");
  lcd.setCursor(60, 2);
  lcd.print("32C");
  lcd.setCursor(90, 2);
  lcd.print("12.4V");

  lcd.setCursor(0, 4);
  lcd.print("48%");
  lcd.setCursor(30, 4);
  lcd.print("33%");
  lcd.setCursor(60, 4);
  lcd.print("50");
  lcd.setCursor(90, 4);
  lcd.print("25C");

  lcd.setCursor(23, 7);
  lcd.print("W0L0AHL3562136509");

}

void setup()
{
  lcd.begin();
  //lcd.setFontSize(FONT_SIZE_SMALL);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.println("I LOVE MY CAR!");

  delay(1500);

  lcd.println();
  lcd.println("Connecting...");

  delay(1500);
  initScreen();
  delay(1000);
  initScreen2();
}

void loop()
{

}
