#include <BlynkSimpleEsp32_BT.h>
#include <Adafruit_NeoPixel.h>
/*---------*/

#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT
#define LED_PIN     2
#define LED_COUNT   8
/*---------*/

char auth[] = "ebb3c553878a430080f4dfe8923ffb10";
/*---------*/

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
/*---------*/
void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Blynk RC Car");
  Blynk.begin(auth);

  pinMode(LED_BUILTIN, OUTPUT);

  strip.begin();
  strip.show();
  strip.setBrightness(3);
}

void loop()
{
  Blynk.run();
}

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

/*-- NeoPixel START--*/
void colorWipeRight(int color, int wait)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
  strip.clear();
}

void colorWipeLeft(int color, int wait)
{
  for (int i = LED_COUNT; i >= 0; i--)
  {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
  strip.clear();
}


void singleLed()
{
  strip.setPixelColor(0, 255, 255, 255); // Set pixel , R , G , B
  strip.show();
}

/*-- NeoPixel END --*/


/*-- BLYNK START--*/

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
  int pinValue = param.asInt(); // Assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V2 to a variable

  if (pinValue != 0)
  {
    while (pinValue > 0)
    {
      colorWipeLeft(strip.Color(255, 80, 0), 60);
      strip.clear();
    }
    while (pinValue < 0)
    {
      colorWipeRight(strip.Color(255, 80, 0), 60);
      strip.clear();
    }
  }
  else strip.clear();
}
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V3 to a variable
}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V4 to a variable
}

BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
}

BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
}

/*-- BLYNK END --*/
