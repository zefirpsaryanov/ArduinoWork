#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#include <Adafruit_NeoPixel.h>
#define PIN    2
#define N_LEDS 8

char auth[] = "ebb3c553878a430080f4dfe8923ffb10";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Blynk RC Car");
  Blynk.begin(auth);

  pinMode(LED_BUILTIN, OUTPUT);

  strip.begin();
  strip.setBrightness(3);
  strip.clear();
}

void loop()
{
  Blynk.run();
}

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

/*-- NeoPixel ART--*/
static void pixels4(int p)
{
  strip.setPixelColor(0  , p); // Draw new pixel
  strip.setPixelColor(1  , p); // Draw new pixel
  strip.setPixelColor(6  , p); // Draw new pixel
  strip.setPixelColor(7  , p); // Draw new pixel
  strip.setBrightness(250);
  delay(200);
  strip.show();
}

static void pixels2(int p)
{
  strip.setPixelColor(0  , p); // Draw new pixel
  strip.setPixelColor(7  , p); // Draw new pixel
  strip.setBrightness(5);
  delay(200);
  strip.show();
}

/*-- NeoPixel END --*/

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED on.
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW); // Turn LED off.
  }
}

BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if (pinValue == 1)
  {
    pixels4(strip.Color(255, 255, 255)); // White
  }
  else
  {
    pixels4(strip.Color(0, 0, 0)); // Black
  }
}

BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if (pinValue == 1)
  {
    pixels2(strip.Color(255, 255, 255)); // White
  }
  else
  {
    pixels2(strip.Color(0, 0, 0)); // Black
  }
}
