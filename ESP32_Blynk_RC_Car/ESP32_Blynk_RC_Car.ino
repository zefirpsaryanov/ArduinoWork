#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#include <Adafruit_NeoPixel.h>
#define PIN    2
#define N_LEDS 8
int R, G, B, brightness = 0;

char auth[] = "ebb3c553878a430080f4dfe8923ffb10";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Blynk RC Car");
  Blynk.begin(auth);

  pinMode(LED_BUILTIN, OUTPUT);

  brightness = 3;
  strip.begin();
  strip.setBrightness(brightness);
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
  //  strip.setBrightness(250);
  delay(200);
  strip.show();
}

static void pixels2(int p)
{
  strip.setPixelColor(0  , p); // Draw new pixel
  strip.setPixelColor(7  , p); // Draw new pixel

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

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i  , (0, 0, 0)); // Draw new pixel
    strip.show();
    delay(100);
  }
}
BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if (pinValue == 1)
  {
    strip.setBrightness(brightness);
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
    strip.setBrightness(brightness);
    pixels2(strip.Color(R, G, B)); // White
  }
  else
  {
    pixels2(strip.Color(0, 0, 0)); // Black
  }
}


BLYNK_WRITE(V9) //zebra
{
  int R = param[0].asInt();
  int G = param[1].asInt();
  int B = param[2].asInt();
  for (int i = 0; i < N_LEDS; i++)
  {
    strip.setPixelColor(i, strip.Color(R, G, B));
    strip.show();
  }
}

BLYNK_WRITE(V10)  //R , G , B , brightness
{
  R = param.asInt(); // Assigning incoming value from pin V0 to a variable
}
BLYNK_WRITE(V11)
{
  G = param.asInt(); // Assigning incoming value from pin V0 to a variable
}
BLYNK_WRITE(V12)
{
  B = param.asInt(); // Assigning incoming value from pin V0 to a variable
}
BLYNK_WRITE(V13)
{
  brightness = param.asInt(); // Assigning incoming value from pin V0 to a variable
}
