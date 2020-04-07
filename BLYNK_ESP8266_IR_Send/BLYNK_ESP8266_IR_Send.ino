#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 3;

IRsend irsend(kIrLed);

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

void setup()
{
  Serial.begin(115200);
  irsend.begin();
  //pinMode(1, OUTPUT);

  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
}

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendNEC(0x2FD48B7, 32); // NEC: 2FD48B7 - TOSHIBA POWER
  }
}
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x38D); // MAG HOME BUTTON
  }
}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x38F); // MAG BACK BUTTON
  }
}
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x39D); // MAG INFO BUTTON
  }
}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x38A); // MAG MENU BUTTON
  }
}
BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BF); // MAG LEFT BUTTON
  }
}
BLYNK_WRITE(V6)
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AB); // MAG RIGH BUTTON
  }
}
BLYNK_WRITE(V7)
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BD); // MAG UPUP BUTTON
  }
}
BLYNK_WRITE(V8)
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BE); // MAG DOWN BUTTON
  }
}
BLYNK_WRITE(V9)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AC); // MAG OKOK BUTTON
  }
}

BLYNK_WRITE(V10)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AC); // MAG OKOK BUTTON
  }
}

BLYNK_WRITE(V11)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AC); // MAG OKOK BUTTON
  }
}

BLYNK_WRITE(V12)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x392); // MAG VOL+ BUTTON
  }
}

BLYNK_WRITE(V13)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x393); // MAG VOL- BUTTON
  }
}

BLYNK_WRITE(V14)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BC); // MAG CHA+ BUTTON
  }
}

BLYNK_WRITE(V15)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x391); // MAG CHA- BUTTON
  }
}

BLYNK_WRITE(V16)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x383); // MAG 3 BUTTON
  }
}


/*
NEC: 2FD48B7 - TOSHIBA POWER
RC5: B8C , 38C  - MAG POWE BUTTON

RC5: BB0 , 3B0  - MAG MUTE BUTTON
RC5: B8D , 38D  - MAG HOME BUTTON
RC5: B8F , 38F  - MAG BACK BUTTON
RC5: B9D , 39D  - MAG INFO BUTTON
RC5: B8A , 38A  - MAG MENU BUTTON
RC5: B8B , 38B  - MAG PLAY BUTTON
RC5: B99 , 399  - MAG STOP BUTTON
RC5: BBF , 3BF  - MAG LEFT BUTTON
RC5: BAB , 3AB  - MAG RIGH BUTTON
RC5: BBD , 3BD  - MAG UPUP BUTTON
RC5: BBE , 3BE  - MAG DOWN BUTTON
RC5: BAC , 3AC  - MAG OKOK BUTTON
RC5: B92 , 392  - MAG VOL+ BUTTON
RC5: B93 , 393  - MAG VOL- BUTTON
RC5: BBC , 3BC  - MAG CHA+ BUTTON
RC5: B91 , 391  - MAG CHA- BUTTON
RC5: B80 , 380  - MAG 0 BUTTON
RC5: B81 , 381  - MAG 1 BUTTON
RC5: B82 , 382  - MAG 2 BUTTON
RC5: B83 , 383  - MAG 3 BUTTON
RC5: B84 , 384  - MAG 4 BUTTON
RC5: B85 , 385  - MAG 5 BUTTON
RC5: B86 , 386  - MAG 6 BUTTON
RC5: B87 , 387  - MAG 7 BUTTON
RC5: B88 , 388  - MAG 8 BUTTON
RC5: B89 , 389  - MAG 9 BUTTON
*/
