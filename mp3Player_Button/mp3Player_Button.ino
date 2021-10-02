#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define key2 2  //connect wire 2 to pin  2 --> 2

SoftwareSerial mySoftwareSerial(12, 14); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  pinMode(key2, INPUT_PULLUP);  // set pin as input

  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(10); //Set volume value. From 0 to 30
}

void loop()
{
  int key2S = digitalRead(key2); // read if key2 is pressed

  if (!key2S)
  {
    Serial.println(F("Key Presed"));
    myDFPlayer.play(1);   //Play the first mp3
  }
}