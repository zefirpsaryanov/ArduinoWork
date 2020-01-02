#define key1 15 //connect wire 4 to pin 15 --> 1
#define key2 2  //connect wire 1 to pin  2 --> 2
#define key3 0  //connect wire 3 to pin  0 --> 3
#define key4 4  //connect wire 2 to pin  4 --> 4

void setup()
{
  Serial.begin(9600);
  pinMode(key1, INPUT_PULLUP);// set pin as input
  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key3, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input
}

void loop()
{
  int key1S = digitalRead(key1); // read if key1 is pressed
  int key2S = digitalRead(key2); // read if key2 is pressed
  int key3S = digitalRead(key3); // read if key3 is pressed
  int key4S = digitalRead(key4); // read if key4 is pressed

  if (!key1S) Serial.println("key 1 is pressed");
  if (!key2S) Serial.println("key 2 is pressed");
  if (!key3S) Serial.println("key 3 is pressed");
  if (!key4S) Serial.println("key 4 is pressed");
  delay(100);
}
