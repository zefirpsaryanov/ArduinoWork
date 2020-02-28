// https://surtrtech.com/2018/01/27/step-by-step-on-how-to-use-the-l298n-dual-h-bridge-driver-with-arduino/
// https://www.youtube.com/watch?v=ly5PToVtPfg&feature=emb_logo


#define key1 5
#define key2 6

int in1 = 7;
int in2 = 8;

void setup()
{
  pinMode(key1, INPUT_PULLUP);// set pin as input
  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void TurnMotorA1()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void TurnOFFA()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void TurnMotorA2()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void movement()
{
  TurnMotorA1();
  delay(3000);
  TurnOFFA();
  delay(2000);
  TurnMotorA2();
  delay(3000);
  TurnOFFA();
  delay(2000);
}

void loop()
{
  int key1S = digitalRead(key1); // read if key1 is pressed
  int key2S = digitalRead(key2); // read if key2 is pressed

  if (!key1S) TurnMotorA1();
  if (!key2S) TurnMotorA2();

  delay(100);
}
