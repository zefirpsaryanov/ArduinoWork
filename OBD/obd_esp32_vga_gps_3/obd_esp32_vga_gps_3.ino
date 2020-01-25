#include <Wire.h>
#include <OBD.h> // PIN 21 22
#include <TinyGPS++.h> // com2 / uart2
#include <HardwareSerial.h> // PIN 16 17
#include <ESP32Lib.h>
#include <Ressources/CodePage437_8x19.h>
#include <OLED_I2C.h>

const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;
const int w = 200;
const int h = 150;


VGA3Bit vga;
COBDI2C obd;
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);


OLED  myOLED(21, 22);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

double speedTMP;
double distance;
double fuelTMP;

int RPM;
int COOLANT;
double ENGINE_FUEL_RATE;
int RUNTIME;
double VOLTAGE;
int AMBIENT_TEMP;
int FUEL_LEVEL;
int ENGINE_OIL_TEMP;


char timeToStringOLED[20];
char timeToStringVGA[20];
char dateToString[20];

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

void speedDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(5, 5);
  vga.print("KM/H");
  if (gps.speed.isValid())
  {
    vga.setCursor(9, 21);
    speedTMP = gps.speed.kmph();
    setColorByValue(speedTMP, 51, 91, 121);
    if (speedTMP >= 0 && speedTMP < 10) vga.print("00");
    if (speedTMP >= 10 && speedTMP < 100) vga.print("0");
    vga.print(speedTMP, 0);
    vga.print(" ");
  }

  if (!gps.speed.isValid())
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(9, 21);
    vga.print("000");
  }
}

void rpmDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(48, 5);
  vga.print("RPM");
  vga.setCursor(45, 21);
  setColorByValue(RPM, 2500, 3000, 3500);
  if (RPM >= 0 && RPM < 10) vga.print("000");
  if (RPM >= 10 && RPM < 100) vga.print("00");
  if (RPM >= 100 && RPM < 1000) vga.print("0");
  vga.print(RPM);
}

void coolantDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(82, 5);
  vga.print("Water");
  vga.setCursor(90, 21);
  if (COOLANT >= 0 && COOLANT < 10) vga.print("00");
  if (COOLANT >= 10 && COOLANT < 100) vga.print("0");
  vga.print(COOLANT);
}

void fuelDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(130, 5);
  vga.print("L/KM");
  vga.setCursor(130, 21);
  setColorByValue(fuelTMP, 5, 7, 9);
  if (fuelTMP >= 0 && fuelTMP < 10) vga.print("0");
  vga.print(fuelTMP, 1);
  vga.print(" ");
}

void ambientDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(174, 5);
  vga.print("C*");
  vga.setCursor(174, 21);
  //  if (AMBIENT_TEMP < 0 && AMBIENT_TEMP < 10) vga.print("-"); /// TO DO
  if (AMBIENT_TEMP >= 0 && AMBIENT_TEMP < 10) vga.print("0");
  vga.print(AMBIENT_TEMP);
}

void runtimeDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(9, 50);
  vga.print("RUN");
  vga.setCursor(25, 66);
  vga.print(RUNTIME / 60, 3);
}

void distanceDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(45, 50);
  vga.print("Dist");
  vga.setCursor(61, 66);
  vga.print(distance / 36000, 0);
}

void voltsDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(86, 50);
  vga.print("V+/-");
  vga.setCursor(86, 66);
  if (VOLTAGE >= 0 && VOLTAGE < 10) vga.print("0");
  vga.print(VOLTAGE, 1);
}

void fuellvlDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(125, 50);
  vga.print("F.%");
  vga.setCursor(130, 66);
  if (FUEL_LEVEL >= 0 && FUEL_LEVEL < 10) vga.print("0");
  vga.print(FUEL_LEVEL);
}

void oilTempDraw()
{
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.setCursor(158, 50);
  vga.print("OIL.C");
  vga.setCursor(166, 66);
  if (ENGINE_OIL_TEMP >= 0 && ENGINE_OIL_TEMP < 10) vga.print("00");
  if (ENGINE_OIL_TEMP >= 10 && ENGINE_OIL_TEMP < 100) vga.print("0");
  vga.print(ENGINE_OIL_TEMP);
}

void bottom()
{
  if (gps.satellites.isValid()) //print valid GPS Sat Count
  {
    vga.setCursor(10, 95);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.print("GPS FIX:");
    setColorByValue(gps.satellites.value(), 4, 8, 15);
    vga.print(gps.satellites.value());
    if (gps.satellites.value() < 10) vga.print(" ");
  }

  //  if (!gps.satellites.isValid() || gps.satellites.value() < 1) //print NOFIX GPS Sat Count
  //  {
  //    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  //    vga.setCursor(10, 95);
  //    vga.print("GPS FIX:NO");
  //  }

  if (gps.location.isValid()) //print valid GPS Location
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 112);
    vga.print(gps.location.lat(), 6);
    vga.setCursor(10, 130);
    vga.print(gps.location.lng(), 6);
  }

  if (!gps.location.isValid()) //print NOFIX GPS Location
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(10, 112);
    vga.print("NOFIX");
  }

  if (gps.date.isValid()) //print valid GPS Date
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 112);
    vga.print(dateToString);
  }

  if (!gps.date.isValid()) //print NOFIX GPS Date
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 112);
    vga.print("NOFIX");
  }

  if (gps.time.isValid()) //print valid GPS Time
  {
    vga.setCursor(110, 95);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.print(timeToStringVGA);
  }

  if (!gps.time.isValid()) //print NOFIX GPS Time
  {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
    vga.setCursor(110, 95);
    vga.print("NOFIX");
  }
}

void oledDisp()
{
  //  //myOLED.clrScr();
  //  myOLED.setBrightness(1);
  //  myOLED.setFont(MediumNumbers);
  //
  //  int HH = gps.time.hour() + 2;
  //  int MM = gps.time.minute();
  //  int SS = gps.time.second();
  //  int DDD = gps.date.day();
  //  int MMM = gps.date.month();
  //  int YYY = gps.date.year();
  //
  //  String strHH;
  //  String strMM;
  //  String strSS;
  //
  //  String strDDD;
  //  String strMMM;
  //
  //  String timeToString;
  //  String dateToString;
  //  String secondRow;
  //
  //  HH < 10 ? strHH = "0" + String(HH) : strHH = String(HH);
  //  MM < 10 ? strMM = "0" + String(MM) : strMM = String(MM);
  //  SS < 10 ? strSS = "0" + String(SS) : strSS = String(SS);
  //
  //  DDD < 10 ? strDDD = "0" + String(DDD) : strDDD = String(DDD);
  //  MMM < 10 ? strMMM = "0" + String(MMM) : strMMM = String(MMM);
  //
  //  timeToString = strHH + "." + strMM + "." + strSS;
  //  dateToString = strDDD + "." + strMMM + "." + String(YYY);
  //
  //  myOLED.print(timeToString, CENTER, 0);
  //  myOLED.printNumF(fuelTMP, 1 , LEFT , 16);
  //  myOLED.printNumI(COOLANT, RIGHT, 16);
  //  myOLED.update();

  myOLED.clrScr();
  myOLED.setBrightness(1);
  myOLED.setFont(MediumNumbers);

  myOLED.print(timeToStringOLED, CENTER, 0);
  //myOLED.print(dateToString, CENTER, 16);
  myOLED.printNumF(fuelTMP, 1 , LEFT , 16);
  myOLED.printNumI(COOLANT, RIGHT, 16);
  myOLED.update();
}

void calculator()
{
  // FUEL
  if (speedTMP <= 0) speedTMP = 0.01;
  if (fuelTMP >= 30.00) fuelTMP = 30.00;
  if (ENGINE_FUEL_RATE <= 0) ENGINE_FUEL_RATE = 0.01;
  fuelTMP = (ENGINE_FUEL_RATE / speedTMP) / 0.036;

  // TIME and DATE
  sprintf(timeToStringVGA, "%02d:%02d:%02d", gps.time.hour() + 2, gps.time.minute(), gps.time.second());
  sprintf(timeToStringOLED, "%02d.%02d.%02d", gps.time.hour() + 2, gps.time.minute(), gps.time.second());
  sprintf(dateToString, "%02d.%02d.%d", gps.date.day(), gps.date.month(), gps.date.year());

}

void mainDisplay()
{
  speedDraw();
  rpmDraw();
  coolantDraw();
  fuelDraw();
  runtimeDraw();
  distanceDraw();
  voltsDraw();
  ambientDraw();
  fuellvlDraw();
  oilTempDraw();
  bottom();
  oledDisp();
}

void debugInfoGPS() // GPS Debug Info
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

  Serial.print(distance / 36000, 2);
  Serial.print(F(" | "));

  Serial.print(ENGINE_FUEL_RATE);
  Serial.print(F(" | "));
  Serial.print(fuelTMP);
  Serial.println(F(" | "));
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
      if (value < 16000) RPM = value;
      break;

    case PID_COOLANT_TEMP:
      if (value < 240) COOLANT = value;
      break;

    case PID_ENGINE_FUEL_RATE:
      if (value < 3200) ENGINE_FUEL_RATE = (double)value;
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

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600, SERIAL_8N1, 17, 16);

  myOLED.begin(SSD1306_128X32);

  vga.setFrameBufferCount(2);
  //vga.init(vga.MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.init(vga.MODE200x150, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(CodePage437_8x19);

  //obd.begin();
  //while (!obd.init());

  vga.clear();
  //showDTC();
}

void loop()
{
  vga.rect(0, 0, w, h, 6);
  vga.line(0, 44 , 198, 44, 6); // - line
  vga.line(0, 91 , 198, 91, 6); // - line 2

  mainDisplay();

  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      calculator();
      debugInfoGPS();
      distance += gps.speed.kmph();
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }

  //  static byte pids[] = {PID_RPM, PID_RUNTIME, PID_FUEL_LEVEL, PID_ENGINE_FUEL_RATE, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP, PID_COOLANT_TEMP, PID_ENGINE_OIL_TEMP};
  //  static byte index = 0;
  //  byte pid = pids[index];
  //  int value;
  //
  //  // send a query to OBD adapter for specified OBD-II pid
  //
  //  if (obd.readPID(pid, value))
  //  {
  //    readData(pid, value);
  //  }
  //
  //  index = (index + 1) % sizeof(pids);
  //
  //  if (obd.errors >= 100)
  //  {
  //    reconnect();
  //    setup();
  //  }
}