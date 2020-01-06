#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#include <GfxWrapper.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

/***fonts***
  #include<Fonts/FreeMono9pt7b.h  >
  #include<Fonts/FreeMono12pt7b.h>
  #include<Fonts/FreeMono18pt7b.h>
  #include<Fonts/FreeMono24pt7b.h>
  #include<Fonts/FreeMonoBold9pt7b.h>
  #include<Fonts/FreeMonoBold12pt7b.h>
  #include<Fonts/FreeMonoBold18pt7b.h>
  #include<Fonts/FreeMonoBold24pt7b.h>
  #include<Fonts/FreeMonoBoldOblique9pt7b.h>
  #include<Fonts/FreeMonoBoldOblique12pt7b.h>
  #include<Fonts/FreeMonoBoldOblique18pt7b.h>
  #include<Fonts/FreeMonoBoldOblique24pt7b.h>
  #include<Fonts/FreeMonoOblique9pt7b.h>
  #include<Fonts/FreeMonoOblique12pt7b.h>
  #include<Fonts/FreeMonoOblique18pt7b.h>
  #include<Fonts/FreeMonoOblique24pt7b.h>
  #include<Fonts/FreeSans9pt7b.h>
  #include<Fonts/FreeSans12pt7b.h>
  #include<Fonts/FreeSans18pt7b.h>
  #include<Fonts/FreeSans24pt7b.h>
  #include<Fonts/FreeSansBold9pt7b.h>
  #include<Fonts/FreeSansBold12pt7b.h>
  #include<Fonts/FreeSansBold18pt7b.h>
  #include<Fonts/FreeSansBold24pt7b.h>
  #include<Fonts/FreeSansBoldOblique9pt7b.h>
  #include<Fonts/FreeSansBoldOblique12pt7b.h>
  #include<Fonts/FreeSansBoldOblique18pt7b.h>
  #include<Fonts/FreeSansBoldOblique24pt7b.h>
  #include<Fonts/FreeSansOblique9pt7b.h>
  #include<Fonts/FreeSansOblique12pt7b.h>
  #include<Fonts/FreeSansOblique18pt7b.h>
  #include<Fonts/FreeSansOblique24pt7b.h>
  #include<Fonts/FreeSerif9pt7b.h>
  #include<Fonts/FreeSerif12pt7b.h>
  #include<Fonts/FreeSerif18pt7b.h>
  #include<Fonts/FreeSerif24pt7b.h>
  #include<Fonts/FreeSerifBold9pt7b.h>
  #include<Fonts/FreeSerifBold12pt7b.h>
  #include<Fonts/FreeSerifBold18pt7b.h>
  #include<Fonts/FreeSerifBold24pt7b.h>
  #include<Fonts/FreeSerifBoldItalic9pt7b.h>
  #include<Fonts/FreeSerifBoldItalic12pt7b.h>
  #include<Fonts/FreeSerifBoldItalic18pt7b.h>
  #include<Fonts/FreeSerifBoldItalic24pt7b.h>
  #include<Fonts/FreeSerifItalic9pt7b.h>
  #include<Fonts/FreeSerifItalic12pt7b.h>
  #include<Fonts/FreeSerifItalic18pt7b.h>
  #include<Fonts/FreeSerifItalic24pt7b.h>

*/

VGA6Bit vga;
GfxWrapper<VGA6Bit> gfx(vga, 640, 400);

TinyGPSPlus gps;
HardwareSerial SerialGPS(2);

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600, SERIAL_8N1, 17, 16);

  vga.init(vga.MODE640x400, vga.VGABlackEdition);

  gfx.setFont(&FreeMonoBoldOblique24pt7b);
}


// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (SerialGPS.available())
      gps.encode(SerialGPS.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      gfx.print('*');
    gfx.print(' ');
  }
  else
  {
    gfx.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      gfx.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  gfx.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    gfx.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());

    gfx.print(sz);
    //Serial.print(sz);
  }

  if (!t.isValid())
  {
    gfx.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour() + 2, t.minute(), t.second());
    gfx.print(sz);
    //Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    gfx.print(i < slen ? str[i] : ' ');
  smartDelay(0);
}


void loop()
{
  gfx.fillScreen(BLACK);
  gfx.setCursor(30, 30);
  static const double HOME_LAT = 42.505626, HOME_LON = 27.473606;

  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

  unsigned long distanceKmToHOME =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      HOME_LAT,
      HOME_LON) / 1000;
  printInt(distanceKmToHOME, gps.location.isValid(), 9);

  double courseToHOME =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      HOME_LAT,
      HOME_LON);

  printFloat(courseToHOME, gps.location.isValid(), 7, 2);

  const char *cardinalToHOME = TinyGPSPlus::cardinal(courseToHOME);

  printStr(gps.location.isValid() ? cardinalToHOME : "*** ", 6);

  printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  gfx.println();

  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}
