#include <OLED_I2C.h>
OLED  myOLED(21, 22); // Remember to add the RESET pin if your display requires it...
extern uint8_t MediumNumbers[];

void setup()
{
  myOLED.begin(SSD1306_128X32);
}

void loop()
{
  int HH = 23; //gps.time.hour();
  int MM = 59; //gps.time.minute();
  int SS = 59; //gps.time.second();

  int COOLANT = 90;
  int AMBIENT_TEMP = 25;
  double fuelTMP = 7.5;

  String strHH;
  String strMM;
  String strSS;
  String timeToString;

  HH < 10 ? strHH = "0" + String(HH) : strHH = String(HH);
  MM < 10 ? strMM = "0" + String(MM) : strMM = String(MM);
  SS < 10 ? strSS = "0" + String(SS) : strSS = String(SS);

  timeToString =  strHH + "." + strMM + "." + strSS;

  myOLED.update();
  myOLED.setBrightness(0);
  myOLED.setFont(MediumNumbers);

  myOLED.print(timeToString, CENTER, 0);  // time

  myOLED.printNumF(fuelTMP, 1, LEFT, 16);
  myOLED.printNumI(AMBIENT_TEMP, CENTER, 16);
  myOLED.printNumI(COOLANT, RIGHT, 16);

}
