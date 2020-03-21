#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <OLED_I2C.h>

OLED  myOLED(0, 2); // (2, 0);

extern uint8_t SmallFont[];

const char* ssid     = "test";
const char* password = "1q2w3e4r";

Adafruit_BMP085 bmp;

void setup()
{
  Wire.pins(0, 2);  //I2C declare pins
  Wire.begin(0, 2); //I2C begin

  if (!bmp.begin())
    while (1)
    {
      return;
    }

  if (!myOLED.begin(SSD1306_128X32))
    while (1)
    {
      return;
    }

  myOLED.setFont(SmallFont);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
    delay(500);
}

void loop()
{
  String t = String(bmp.readTemperature());
  String p = String(bmp.readPressure());

  myOLED.print(t, LEFT, 0);
  myOLED.print(p, RIGHT, 0);
  myOLED.update();
  delay (500);
}
