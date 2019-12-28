#include <TinyGPS++.h>
#include <HardwareSerial.h>


#include <Adafruit_ssd1306syp.h>                        // Adafruit oled library for display
Adafruit_ssd1306syp display(4, 5);                      // OLED display (SDA to Pin 4), (SCL to Pin 5)

TinyGPSPlus gps;
HardwareSerial SerialGPS(2);

const double Home_LAT = 42.505626;                      // Your Home Latitude
const double Home_LNG = 27.473606;                     // Your Home Longitude


void setup()
{
  display.initialize();                                 // Initialize OLED display
  display.clear();                                      // Clear OLED display
  display.setTextSize(1);                               // Set OLED text size to small
  display.setTextColor(WHITE);                          // Set OLED color to White
  display.setCursor(0, 0);                              // Set cursor to 0,0
  display.println("GPS example");
  display.println(TinyGPSPlus::libraryVersion());
  display.update();                                     // Update display
  delay(1500);                                          // Pause 1.5 seconds
  SerialGPS.begin(9600, SERIAL_8N1, 17, 16);
}

void loop()
{
  display.clear();
  display.setCursor(0, 0);
  display.print("Latitude  : ");
  display.println(gps.location.lat(), 5);
  display.print("Longitude : ");
  display.println(gps.location.lng(), 4);
  display.print("Satellites: ");
  display.println(gps.satellites.value());
  display.print("Elevation : ");
  display.print(gps.altitude.feet());
  display.println("ft");
  display.print("Time UTC  : ");
  display.print(gps.time.hour());                       // GPS time UTC
  display.print(":");
  display.print(gps.time.minute());                     // Minutes
  display.print(":");
  display.println(gps.time.second());                   // Seconds
  display.print("Heading   : ");
  display.println(gps.course.deg());
  display.print("Speed     : ");
  display.println(gps.speed.mph());

  unsigned long Distance_To_Home = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), Home_LAT, Home_LNG);
  display.print("KM to Home: ");                        // Have TinyGPS Calculate distance to home and display it
  display.print(Distance_To_Home);
  display.update();                                     // Update display
  delay(200);

  smartDelay(500);                                      // Run Procedure smartDelay

  if (millis() > 5000 && gps.charsProcessed() < 10)
    display.println(F("No GPS data received: check wiring"));
}

static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do
  {
    while (SerialGPS.available())
      gps.encode(SerialGPS.read());
  } while (millis() - start < ms);
}
