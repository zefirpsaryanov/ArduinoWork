// https://surtrtech.com/2018/01/27/step-by-step-on-how-to-use-the-l298n-dual-h-bridge-driver-with-arduino/
// https://www.youtube.com/watch?v=ly5PToVtPfg&feature=emb_logo


#define key4 4  // DOWN
#define key5 5  // UP
#define key12 12  // UP

int in1 = 7;
int in2 = 8;

void setup()
{
  pinMode(key4, INPUT_PULLUP);
  pinMode(key5, INPUT_PULLUP);
  pinMode(key12, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  movement();
}

void TurnMotorA1()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void TurnMotorA2()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void TurnOFFA()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void movement()
{
  TurnMotorA1();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(30000);
  TurnOFFA();
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);
  TurnMotorA2();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(30000);
  TurnOFFA();
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}

void loop()
{
  int key4S = digitalRead(key4); // read if key1 is pressed
  int key5S = digitalRead(key5); // read if key2 is pressed
  int key12S = digitalRead(key12); // read if key2 is pressed

  if (!key4S) TurnMotorA1();
  if (!key5S) TurnMotorA2();
  if (!key12S) TurnOFFA();
  if (key4S && key5S) TurnOFFA();

  delay(100);
}
