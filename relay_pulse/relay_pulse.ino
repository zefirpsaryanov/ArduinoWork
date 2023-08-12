#define keyPin    3  // START button
#define relayPin  0  // relayPin


void setup()
{
  pinMode(keyPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
}

void loop()
{
  int keyStatus = digitalRead(keyPin);
  if (keyStatus == 0) // 0 - pressed
  {
    delay(60000); //60000
    digitalWrite(relayPin, LOW);
    delay(100);
    digitalWrite(relayPin, HIGH);
  }
}
