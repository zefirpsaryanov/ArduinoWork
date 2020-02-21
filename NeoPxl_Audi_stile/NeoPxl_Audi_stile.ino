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
  colorWipeRight(strip.Color(255, 80, 0), 60);
  strip.clear();
  colorWipeLeft(strip.Color(255, 80, 0), 60);
  strip.clear();
  color2ways(strip.Color(255, 80, 0), 120);
  strip.clear();

}

void colorWipeRight(int color, int wait)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void colorWipeLeft(int color, int wait)
{
  for (int i = LED_COUNT; i >= 0; i--)
  {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void color2ways(int color, int wait)
{
  int i, j;
  for (i = (LED_COUNT / 2 - 1), j = LED_COUNT / 2; i >= 0  && j <= LED_COUNT - 1 ; i--, j++)
  {
    strip.setPixelColor(i, color);
    strip.setPixelColor(j, color);
    strip.show();
    delay(wait);
  }
}

void singleLed()
{
  strip.setPixelColor(0, 255, 255, 255); // Set pixel , R , G , B
  strip.show();
}
