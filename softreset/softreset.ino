unsigned long currentMillis;
const unsigned long period = 3000;
const byte ledPin = LED_BUILTIN;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void(* resetFunc)() = 0;

void loop()
{
  digitalWrite(ledPin, LOW);
  currentMillis = millis();
  if (currentMillis >= period)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("resetting");
    resetFunc();
  }
  Serial.println(currentMillis);
}
