
// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	Serial.println("turn the LED ON");
	delay(1000);                       // wait for a second
	Serial.println("delay(1000)");
	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
	Serial.println("turn the LED OFF");
	delay(100);                       // wait for a second
	Serial.println("delay(100)");
}