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
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);

  strip.begin();

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

BLYNK_WRITE(V1) // forward / back
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue >= 3)
  {
    digitalWrite(17, HIGH); // Turn LED on.
  }
  else if (pinValue <= -3)
  {
    digitalWrite(16, HIGH); // Turn LED on.
  }
  else
  {
    digitalWrite(17, LOW); // Turn LED off.
    digitalWrite(16, LOW); // Turn LED off.
  }
}

BLYNK_WRITE(V2) // left / right
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue <= -2)
  {
    digitalWrite(4, HIGH); // Turn LED on. // left
  }
  else if (pinValue >= 2)
  {
    digitalWrite(0, HIGH); // Turn LED on. // right
  }
  else
  {
    digitalWrite(4, LOW); // Turn LED off.
    digitalWrite(0, LOW); // Turn LED off.
  }
}
