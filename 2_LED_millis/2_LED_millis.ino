// constants to set pin numbers
const int greenLedPin = D5;						// the number of the green LED pin
const int redLedPin = D7;						// the number of the green LED pin

// Variables will change
int greenLedState = HIGH; 						//ledState for green LED
int redLedState = HIGH;   						//ledState for red LED
long previousMillisRed = 0;    					//last time Red LED was updated
long previousMillisGreen = 0;  					//last time Green LED was updated

// must be long to prevent overflow
long greenLedInterval = 200; 					//interval to blink greed LED (milliseconds)
long redLedInterval = 300;    					//interval to blink red LED (milliseconds)
unsigned long currentMillis = 0;

void setup() 
{
	Serial.begin(115200);
	Serial.println("Hello!");
	
	// set the pins to output mode
	pinMode(greenLedPin, OUTPUT);
	pinMode(redLedPin, OUTPUT);
	digitalWrite(redLedPin, redLedState);
	digitalWrite(greenLedPin, greenLedState);
	currentMillis = millis();
}

void loop()
{
	// capture the current time
	currentMillis = millis();
	manageRedLed();
	manageGreenLed();
}

void manageRedLed() 
{  																	//check if it's time to change the Red LED yet
	if(currentMillis - previousMillisRed > redLedInterval) 
		{															//store the time of this change
			previousMillisRed = currentMillis;
			redLedState = (redLedState == HIGH) ? LOW : HIGH;
			digitalWrite(redLedPin, redLedState);
			Serial.println(currentMillis);
		}
}

void manageGreenLed() 
{
	if(currentMillis - previousMillisGreen > greenLedInterval) 		//check if it's time to change the green LED yet 
		{															//store the time of this change
			previousMillisGreen = currentMillis;
			greenLedState = (greenLedState == HIGH) ? LOW : HIGH;
			digitalWrite(greenLedPin, greenLedState);
			Serial.println(currentMillis);
		}
}
