#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <OBD.h>
#include <MicroLCD.h>

LCD_SH1106 lcd;
COBDI2C obd;
static double speedTMP;
static double distance;
/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


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
    delay(500);
  }
}

void showData(byte pid, int value)

{
  lcd.setFontSize(FONT_SIZE_MEDIUM);

  switch (pid)
  {

    case PID_SPEED:
      lcd.setCursor(0, 0);
      lcd.printInt(gps.speed.kmph());
      speedTMP = (unsigned int)value / 1000;
      break;

    case PID_RPM:
      lcd.setCursor(33, 0);
      lcd.printInt((unsigned int)value % 10000, 4);
      break;

    case PID_COOLANT_TEMP:
      lcd.setCursor(75, 0);
      lcd.printInt(value, 2);
      break;

    case PID_MAF_FLOW:
      lcd.setCursor(105, 0);
      lcd.printInt((double)value * 0.8, 2);
      break;

    /*---------------------------------------*/

    case PID_RUNTIME:
      lcd.setCursor(0, 3);
      lcd.printInt(value / 60, 2); // /60 mins
      break;

    case PID_DISTANCE_WITH_MIL:
      lcd.setCursor(33, 3);
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

  lcd.setCursor(2, 2);
  lcd.print("KM/H");
  lcd.setCursor(40, 2);
  lcd.print("RPM");
  lcd.setCursor(70, 2);
  lcd.print("WATER");
  lcd.setCursor(105, 2);
  lcd.print("FUEL");

  lcd.setCursor(5, 5);
  lcd.print("RUN");
  lcd.setCursor(38, 5);
  lcd.print("DIST");
  lcd.setCursor(70, 5);
  lcd.print("VOLTS");
  lcd.setCursor(107, 5);
  lcd.print("AMB");
}

void setup()
{
  ss.begin(GPSBaud);
  lcd.begin();
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.println("I LOVE MY CAR!");

  delay(500);
  //obd.begin();

  lcd.println(" Connecting...");
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
    lcd.print(" Can`t get VIN");
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



/*
  void setup()
  {
  Serial.begin(115200);


  Serial.println(F("FullExample.ino"));
  Serial.println(F("An extensive example of many interesting TinyGPS++ features"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
  Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to Burgas  ----  RX    RX        Fail"));
  Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));
  }
*/


void loop()
{
  static const double LONDON_LAT = 42.505626, LONDON_LON = 27.473606;


  /*
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
  */



  unsigned long distanceKmToLondon = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), LONDON_LAT, LONDON_LON) / 1000;
  // printInt(distanceKmToLondon, gps.location.isValid(), 9);

  double courseToLondon = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), LONDON_LAT, LONDON_LON);

  // printFloat(courseToLondon, gps.location.isValid(), 7, 2);

  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

  //printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

  //printInt(gps.charsProcessed(), true, 6);
  //printInt(gps.sentencesWithFix(), true, 10);
  //printInt(gps.failedChecksum(), true, 9);
  //Serial.println();

  //smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));




static byte pids[] = {PID_SPEED, PID_RPM, PID_COOLANT_TEMP, PID_MAF_FLOW, PID_RUNTIME, PID_DISTANCE_WITH_MIL, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP, };
static byte index = 0;
byte pid = pids[index];
int value;
// send a query to OBD adapter for specified OBD-II pid
if (obd.readPID(pid, value))
{
  showData(pid, value);
  // distance += (float)(speedTMP * 0.9);
}
index = (index + 1) % sizeof(pids);

if (obd.errors >= 30000)
{
  reconnect();
  setup();
}
}
// This custom version of delay() ensures that the gps object
// is being "fed".


static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
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
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }

  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour() + 2, t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    Serial.print(i < slen ? str[i] : ' ');
  smartDelay(0);
}
