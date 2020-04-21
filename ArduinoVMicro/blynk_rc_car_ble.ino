#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

#include <Arduino.h>
#include <analogWrite.h>


#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#include <FastLED.h>

#define LED_PIN_UP      17
#define LED_PIN_DOWN    16
#define LED_PIN_RIGHT   4
#define LED_PIN_LEFT    0

#define MOTOR1_IN1      27 // right 
#define MOTOR1_IN2      14 // left
#define MOTOR2_IN1      12 // up
#define MOTOR2_IN2      13 // down

#define NUM_LEDS		8
//#define BRIGHTNESS		10
#define DATA_PIN		15
#define wait            100

#define debugInfo       0

char auth[] = "ebb3c553878a430080f4dfe8923ffb10";

bool goUp = false;
bool goDown = false;
bool goLeft = false;
bool goRight = false;

int MOTOR1_PWR = 0; // left and right
int MOTOR2_PWR = 0; // up and down
int BRIGHTNESS;

CRGB leds[NUM_LEDS];


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


void setup()
{
	Serial.begin(9600);
	
	Blynk.setDeviceName("Blynk RC Car");
	Blynk.begin(auth);

	pinMode(LED_BUILTIN, OUTPUT);

	digitalWrite(LED_BUILTIN, HIGH); // Turn LED OFF.

	pinMode(LED_PIN_UP, OUTPUT);
	pinMode(LED_PIN_DOWN, OUTPUT);
	pinMode(LED_PIN_RIGHT, OUTPUT);
	pinMode(LED_PIN_LEFT, OUTPUT);

	pinMode(MOTOR1_IN1, OUTPUT);
	pinMode(MOTOR1_IN2, OUTPUT);
	pinMode(MOTOR2_IN1, OUTPUT);
	pinMode(MOTOR2_IN2, OUTPUT);

	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
	BRIGHTNESS = 10;

	inputString.reserve(200);
}

void loop()
{
	Blynk.run();
	ledStrip();
	readyOrNot();



#if debugInfo

	Serial.print("goUp: ");
	Serial.print(goUp);
	Serial.print(":");
	Serial.print(MOTOR2_PWR);
	Serial.print(" ");

	Serial.print("goDown: ");
	Serial.print(goDown);
	Serial.print(":");
	Serial.print(MOTOR2_PWR);
	Serial.print(" ");

	Serial.print("goLeft: ");
	Serial.print(goLeft);
	Serial.print(":");
	Serial.print(MOTOR1_PWR);
	Serial.print(" ");

	Serial.print("goRight: ");
	Serial.print(goRight);
	Serial.print(":");
	Serial.print(MOTOR1_PWR);
	Serial.print(" ");

	Serial.print("BRIGHTNESS: ");
	Serial.print(BRIGHTNESS);
	Serial.println(" ");
	delay(wait);

#endif
}

void readyOrNot()
{
	if (stringComplete)
	{
		if (inputString.startsWith("Ready"))
		{
			digitalWrite(LED_BUILTIN, HIGH); // Turn LED ON.
		}
		if (inputString.startsWith("Disconnected"))
		{
			digitalWrite(LED_BUILTIN, LOW); // Turn LED OFF.
		}
		inputString = "";
		stringComplete = false;
	}
	Serial.print(inputString);
}


BLYNK_CONNECTED()
{
	Blynk.syncAll();
}

BLYNK_APP_CONNECTED()
{
	digitalWrite(LED_BUILTIN, LOW); // Turn LED on.
}

BLYNK_APP_DISCONNECTED()
{
	digitalWrite(LED_BUILTIN, HIGH); // Turn LED OFF.
}

BLYNK_WRITE(V1)                                 // LEFT / RIGHT
{
	int pinValue1 = param.asInt();

	if (pinValue1 != 0)
	{
		if (pinValue1 >= 2)                         // Turn right.
		{
			digitalWrite(LED_PIN_RIGHT, HIGH);        // Turn right LED ON.
			digitalWrite(LED_PIN_LEFT, LOW);          // Turn left LED OFF.
			MOTOR1_PWR = pinValue1 * 64 - 1;
			analogWrite(MOTOR1_IN1, MOTOR1_PWR);
			goRight = true;
			goLeft = false;
		}

		if (pinValue1 <= -2)                        // Turn left.
		{
			digitalWrite(LED_PIN_RIGHT, LOW);         // Turn right LED OFF.
			digitalWrite(LED_PIN_LEFT, HIGH);         // Turn left LED ON.
			MOTOR1_PWR = (pinValue1 * -1) * 64 - 1;
			analogWrite(MOTOR1_IN2, MOTOR1_PWR);
			goLeft = true;
			goRight = false;
		}
	}
	else
	{
		MOTOR1_PWR = 0;
		digitalWrite(LED_PIN_RIGHT, LOW);           // Turn right LED off.
		digitalWrite(LED_PIN_LEFT, LOW);            // Turn left LED off.
		goLeft = false;
		goRight = false;
	}
}

BLYNK_WRITE(V2)                                 // UP / DOWN
{
	int pinValue2 = param.asInt();

	if (pinValue2 != 0)
	{
		if (pinValue2 >= 1)             // go UP
		{
			digitalWrite(LED_PIN_UP, HIGH);       // Turn ON UP LED.
			digitalWrite(LED_PIN_DOWN, LOW);        // Turn OFF DOWN LED.
			MOTOR2_PWR = pinValue2 * 32 - 1;
			analogWrite(MOTOR2_IN1, MOTOR2_PWR);
			goUp = true;
			goDown = false;

		}

		if (pinValue2 <= -1)            // go DOWN
		{
			digitalWrite(LED_PIN_UP, LOW);        // Turn OFF UP LED.
			digitalWrite(LED_PIN_DOWN, HIGH);       // Turn ON BACK LED.
			MOTOR2_PWR = (pinValue2 * -1) * 32 - 1;
			analogWrite(MOTOR2_IN2, MOTOR2_PWR);
			digitalWrite(LED_BUILTIN, LOW); // Turn LED OFF.
			goDown = true;
			goUp = false;
		}
	}

	else
	{
		MOTOR2_PWR = 0;
		digitalWrite(LED_PIN_UP, LOW);          // Turn OFF UP LED.
		digitalWrite(LED_PIN_DOWN, LOW);          // Turn OFF DOWN LED.
		goUp = false;
		goDown = false;
	}
}

BLYNK_WRITE(V3)
{
	BRIGHTNESS = param.asInt();
}

void ledStrip()
{
	FastLED.setBrightness(BRIGHTNESS);

	if (goUp == 1)
	{
		leds[0] = CRGB::White;
		leds[7] = CRGB::White;
	}
	if (goUp == 0)
	{
		leds[0] = CRGB::Black;
		leds[7] = CRGB::Black;
	}
	if (goDown == 1)
	{
		leds[1] = CRGB::White;
		leds[6] = CRGB::White;
	}
	if (goDown == 0)
	{
		leds[1] = CRGB::Black;
		leds[6] = CRGB::Black;
	}
	if (goLeft == 1)
	{
		leds[2] = CRGB::DarkOrange;
		delay(wait);
		FastLED.show();
		leds[2] = CRGB::Black;
		delay(wait);
		FastLED.show();
	}
	if (goLeft == 0)
	{
		leds[2] = CRGB::Black;
	}
	if (goRight == 1)
	{
		leds[5] = CRGB::DarkOrange;
		delay(wait);
		FastLED.show();
		leds[5] = CRGB::Black;
		delay(wait);
		FastLED.show();
	}
	if (goRight == 0)
	{
		leds[5] = CRGB::Black;;
	}
	leds[3] = CRGB::Red;
	leds[4] = CRGB::Red;

	FastLED.show();
}