#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
/*
#define MOTOR1_IN1 9
#define MOTOR1_IN2 10
#define MOTOR2_IN1 9
#define MOTOR2_IN2 10
#define MOTOR3_IN1 9
#define MOTOR3_IN2 10
#define MOTOR4_IN1 9
#define MOTOR4_IN2 10
*/


char auth[] = "ebb3c553878a430080f4dfe8923ffb10";

void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Blynk RC Car");
  Blynk.begin(auth);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  int FR , FL, RR, RL = 0;
  motorOneForvard
  motorOneBack
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
  int pinValue = param.asInt(); // Assigning incoming value from pin V1 to a variable
  if (pinValue >= 2)
  {
    digitalWrite(17, HIGH); // Turn LED forward.
    digitalWrite(16, LOW);
  }
  else if (pinValue <= -2)
  {
    digitalWrite(16, HIGH); // Turn LED back.
    digitalWrite(17, LOW); // Turn forward LED off.
  }
  else
  {
    digitalWrite(17, LOW); // Turn forward LED off.
    digitalWrite(16, LOW); // Turn back LED off.
  }
}

BLYNK_WRITE(V2) // left / right
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V2 to a variable
  if (pinValue <= -2)
  {
    digitalWrite(4, HIGH); // Turn left LED on.
    digitalWrite(0, LOW); // Turn right LED off.
  }
  else if (pinValue >= 2)
  {
    digitalWrite(0, HIGH); // Turn right LED on.
    digitalWrite(4, LOW); // Turn left LED off.
  }
  else
  {
    digitalWrite(4, LOW); // Turn left LED off.
    digitalWrite(0, LOW); // Turn right LED off.
  }
}
