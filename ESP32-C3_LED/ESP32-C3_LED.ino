int ledRed = 3;
int ledGre = 4;
int ledBlu = 5;
int ledWhi = 18; // orange_led - 18 (12 13 14 15 16 17 - ne )
int ledVal;

void setup()
{
  pinMode(ledRed, OUTPUT);  // sets the pin as output
  pinMode(ledBlu, OUTPUT);  // sets the pin as output
  pinMode(ledGre, OUTPUT);  // sets the pin as output
  pinMode(ledWhi, OUTPUT);  // sets the pin as output
}

void loop()
{
  ledVal = random(0, 31);
  analogWrite(ledRed, ledVal);
  delay(250);
  analogWrite(ledGre, ledVal);
  delay(250);
  analogWrite(ledBlu, ledVal);
  delay(250);
  analogWrite(ledWhi, 63);
  delay(250);
  analogWrite(ledWhi, 0);
  delay(250);
}
