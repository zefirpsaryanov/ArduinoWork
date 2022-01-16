#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define key8 8  //connect wire 8 to pin  8 --> 8

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  pinMode(key8, INPUT_PULLUP);  // set pin as input

  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(10); //Set volume value. From 0 to 30
}

void loop()
{
  int key8S = digitalRead(key8); // read if key2 is pressed

  if (!key8S)
  {
    Serial.println(F("Key Presed"));
    myDFPlayer.play(1);   //Play the first mp3
  }
}
