#include <Adafruit_NeoPixel.h>

#define PIN    2
#define N_LEDS 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.setBrightness(3);
}

void loop()
{
  pixels(strip.Color(255, 0, 0)); // Red
  pixels(strip.Color(0, 255, 0)); // Green
  pixels(strip.Color(0, 0, 255)); // Blue
  pixels(strip.Color(255, 255, 255)); // White
  pixels(strip.Color(255, 255, 0)); // ??
  pixels(strip.Color(255, 0, 255)); // ??
  pixels(strip.Color(0, 255, 255)); // ??
}

static void pixels(int p)
{
  strip.setPixelColor(0  , p); // Draw new pixel
  strip.setPixelColor(7  , p); // Draw new pixel
  delay(200);
  strip.show();
}
