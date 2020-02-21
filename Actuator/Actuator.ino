// https://surtrtech.com/2018/01/27/step-by-step-on-how-to-use-the-l298n-dual-h-bridge-driver-with-arduino/
// https://www.youtube.com/watch?v=ly5PToVtPfg&feature=emb_logo

int in1 = 9;
int in2 = 8;

void setup()
{
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

void loop()
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
