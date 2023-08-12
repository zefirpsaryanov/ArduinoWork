#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define key6 6  //connect wire 8 to pin  9 --> 9

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned long period = 1000;  //the value is a number of milliseconds

SoftwareSerial mySoftwareSerial(11, 10); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  pinMode(key6, INPUT_PULLUP);  // set pin as input
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(10); //Set volume value. From 0 to 30
  Serial.print("Go!");
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    int key6S = digitalRead(key6); // read if key2 is pressed
    if (!key6S)
    {
      Serial.println(F("Key Presed"));
      myDFPlayer.play(1);   //Play the first mp3
      startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    }
  }
}