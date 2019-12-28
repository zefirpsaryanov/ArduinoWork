#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <OLED_I2C.h>


TinyGPSPlus gps;
HardwareSerial SerialGPS(2);
OLED  myOLED(21, 22);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600, SERIAL_8N1, 17, 16);

  if (!myOLED.begin(SSD1306_128X32))
    while (1);  // In case the library failed to allocate enough RAM for the display buffer...

  myOLED.setFont(SmallFont);
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
}

void displayInfo()
{
  Serial.print(F("Sat`s: "));
  if (gps.satellites.isValid())
  {
    Serial.print(gps.satellites.value());
    myOLED.printNumI(gps.satellites.value(), 70, 0); // value , X ,Y
  }
  else
  {
    Serial.print(F("********"));
    myOLED.print(" ", 70, 10);
  }
  Serial.print(F(" | "));

// myOLED.setFont(SmallFont);

  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    myOLED.printNumF(float(gps.location.lat()), 6, 0, 0); // value , X ,Y

    Serial.print(F(" / "));
    
    Serial.print(gps.location.lng(), 6);
    myOLED.printNumF(float(gps.location.lng()), 6, 0, 10); // value , X ,Y
  }
  else
  {
    Serial.print(F("INVALID"));
    myOLED.print(" ", 0, 0);
    myOLED.print(" ", 0, 10);
  }
  Serial.print(F(" | "));


  if (gps.date.isValid())
  {
    Serial.print(gps.date.day());
    Serial.print(F("."));
    Serial.print(gps.date.month());
    Serial.print(F("."));
    Serial.print(gps.date.year());


    if (gps.date.day() < 10) myOLED.print("0", 0, 20);
    myOLED.printNumI(gps.date.day(), 0, 20); // value , X ,Y

    myOLED.print(".", 14, 20);

    if (gps.date.month() < 10) myOLED.print("0", 0, 20);
    myOLED.printNumI(gps.date.month(), 20, 20); // value , X ,Y

    myOLED.print(".", 32, 20);

    myOLED.printNumI(gps.date.year(), 38, 20); // value , X ,Y
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" | "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());


    if (gps.time.hour() + 2 < 10)
    {
      myOLED.print("0", 70, 20);
      myOLED.printNumI(gps.time.hour() + 2, 76, 20); // value , X ,Y
    }
    else
    {
      myOLED.printNumI(gps.time.hour() + 2, 70, 20); // value , X ,Y
    }

    myOLED.print(":", 82, 20);

    if (gps.time.minute() < 10)
    {
      myOLED.print("0", 88, 20);
      myOLED.printNumI(gps.time.minute(), 94, 20); // value , X ,Y
    }
    else
    {
      myOLED.printNumI(gps.time.minute(), 88, 20); // value , X ,Y
    }

    myOLED.print(":", 102, 20);


    if (gps.time.second() < 10)
    {
      myOLED.print("0", 108, 20);
      myOLED.printNumI(gps.time.second(), 114, 20); // value , X ,Y
    }
    else
    {
      myOLED.printNumI(gps.time.second(), 108, 20); // value , X ,Y
    }
  }
  else
  {
    Serial.print(F("INVALID"));
    myOLED.print(" ", 0, 20);
  }


  Serial.print(F(" | "));

  Serial.print(F("Speed: "));
  if (gps.speed.isValid())
  {
    Serial.print(gps.speed.kmph());
    //myOLED.printNumI(gps.speed.kmph(), 96, 0); // value , X ,Y
    myOLED.printNumF(float(gps.speed.kmph()), 2, 70, 10);
  }
  else
  {
    Serial.print(F("INVALID"));
    myOLED.print(" ", 70, 0);
  }
  Serial.println(F(" | "));

  myOLED.update();

}



/*
  myOLED.print("0", 0, 20);
  myOLED.print("8", 6, 20);
  myOLED.print(":", 12, 20);
  myOLED.print("0", 17, 20);
  myOLED.print("8", 23, 20);
  myOLED.print(":", 29, 20);
  myOLED.print("0", 34, 20);
  myOLED.print("8", 40, 20);

  myOLED.print("|", 46, 20);

  myOLED.print("8", 52, 20);
  myOLED.print("8", 58, 20);
  myOLED.print("8", 64, 20);
  myOLED.print("8", 70, 20);
*/
