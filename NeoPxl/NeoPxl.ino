#include <Adafruit_NeoPixel.h>
#define LED_PIN     5
#define LED_COUNT   8
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.show();
  strip.setBrightness(3);
}

void loop()
{
  colorWipeBack(strip.Color(255, 80, 0), 60);
  strip.clear();
  colorWipe(strip.Color(255, 80, 0), 60);
  strip.clear();
}

void colorWipe(int color, int wait)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void colorWipeBack(int color, int wait)
{
  for (int i = strip.numPixels(); i >= 0; i--)
  {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}


void singleLed()
{
  strip.setPixelColor(5, 0, 0, 255); // Set pixel , R , G , B
}
