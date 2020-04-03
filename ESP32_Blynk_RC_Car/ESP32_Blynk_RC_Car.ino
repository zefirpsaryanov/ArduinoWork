#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BT.h>


char auth[] = "ebb3c553878a430080f4dfe8923ffb10";
int rssi = 0;
void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Blynk RC Car");
  Blynk.begin(auth);

  pinMode(LED_BUILTIN, OUTPUT);

}

void loop()
{
  Blynk.run();
  Blynk.virtualWrite(V4, rssi);
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



BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V2 to a variable
}

BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V3 to a variable
}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
}
