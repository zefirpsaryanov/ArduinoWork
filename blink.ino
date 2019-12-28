
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  delay(100);                         // wait for ..
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED ON by making the voltage HIGH
  delay(2000);                        // wait for ..
  digitalWrite(LED_BUILTIN, LOW);     // turn the LED OFF 
}