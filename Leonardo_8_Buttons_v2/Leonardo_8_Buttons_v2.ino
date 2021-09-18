#include "Keyboard.h"
#define KEY_LEFT_ARROW    0xD8
#define KEY_RIGHT_ARROW   0xD7

#define key2 2  //connect wire 2 to pin  2 --> KEY_LEFT_ARROW
#define key4 4  //connect wire 4 to pin  4 --> KEY_RIGHT_ARROW
#define key7 7  //connect wire 7 to pin  7 --> 1
#define key8 8  //connect wire 8 to pin  8 --> 5
#define key15 15  //connect wire 15 to pin  3 --> z
#define key14 14  //connect wire 14 to pin  5 --> 7
#define key16 16  //connect wire 16 to pin  6 --> 8

void setup()
{
  Serial.begin(9600);

  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input
  pinMode(key7, INPUT_PULLUP);// set pin as input
  pinMode(key8, INPUT_PULLUP);// set pin as input
  pinMode(key15, INPUT_PULLUP);// set pin as input
  pinMode(key14, INPUT_PULLUP);// set pin as input
  pinMode(key16, INPUT_PULLUP);// set pin as input
}

void loop()
{
  int key2S = digitalRead(key2); // read if key2 is pressed
  int key4S = digitalRead(key4); // read if key4 is pressed
  int key7S = digitalRead(key7); // read if key7 is pressed
  int key8S = digitalRead(key8); // read if key8 is pressed
  int key15S = digitalRead(key15); // read if key15 is pressed
  int key14S = digitalRead(key14); // read if key14 is pressed
  int key16S = digitalRead(key16); // read if key16 is pressed

  if (!key2S)
  {
    Keyboard.press(KEY_LEFT_ARROW);
    Keyboard.release(KEY_LEFT_ARROW);
  }
  if (!key4S)
  {
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_RIGHT_ARROW);
  }
  if (!key7S)
  {
    Keyboard.press('1');
    Keyboard.release('1');
  }
  if (!key8S)
  {
    Keyboard.press('5');
    Keyboard.release('5');
  }
  if (!key15S)
  {
    Keyboard.press('z');
    Keyboard.release('z');
  }
  if (!key14S)
  {
    Keyboard.press('7');
    Keyboard.release('7');
  }
  if (!key16S)
  {
    Keyboard.press('8');
    Keyboard.release('8');
  }

  delay(100);
}
