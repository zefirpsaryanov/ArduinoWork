#include "Keyboard.h"
#define key2 2  //connect wire 2 to pin  2 --> 2
#define key3 3  //connect wire 3 to pin  3 --> 3
#define key4 4  //connect wire 4 to pin  4 --> 4
#define key5 5  //connect wire 5 to pin  5 --> 5
#define key6 6  //connect wire 6 to pin  6 --> 6
#define key7 7  //connect wire 7 to pin  7 --> 7
#define key8 8  //connect wire 8 to pin  8 --> 8

void setup()
{
  Serial.begin(9600);

  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key3, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input
  pinMode(key5, INPUT_PULLUP);// set pin as input
  pinMode(key6, INPUT_PULLUP);// set pin as input
  pinMode(key7, INPUT_PULLUP);// set pin as input
  pinMode(key8, INPUT_PULLUP);// set pin as input
}

void loop()
{
  int key2S = digitalRead(key2); // read if key2 is pressed
  int key3S = digitalRead(key3); // read if key3 is pressed
  int key4S = digitalRead(key4); // read if key4 is pressed
  int key5S = digitalRead(key5); // read if key5 is pressed
  int key6S = digitalRead(key6); // read if key6 is pressed
  int key7S = digitalRead(key7); // read if key7 is pressed
  int key8S = digitalRead(key8); // read if key8 is pressed

  if (!key2S)
  {
    Keyboard.press('2');
    Keyboard.release('2');
  }
  if (!key3S)
  {
    Keyboard.press('3');
    Keyboard.release('3');
  }
  if (!key4S)
  {
    Keyboard.press('4');
    Keyboard.release('4');
  }
  if (!key5S)
  {
    Keyboard.press('5');
    Keyboard.release('5');
  }
  if (!key6S)
  {
    Keyboard.press('6');
    Keyboard.release('6');
  }
  if (!key7S)
  {
    Keyboard.press('7');
    Keyboard.release('7');
  }
  if (!key8S)
  {
    Keyboard.press('8');
    Keyboard.release('8');
  }

  delay(100);
}
