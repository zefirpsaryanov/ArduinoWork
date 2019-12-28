/* PN532 NFC RFID module V3 (PN532) [S192] : http://rdiot.tistory.com/173 [RDIoT Demo] */

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

const int ralayPin4 = 4;										// the number4 of the "relay1" pin
const int wifiButton = 5;										// the number5 of the "wifiButton" pin
const int homeButton = 6;										// the number6 of the "homeButton" pin
const int _433Button = 7;										// the number7 of the "_433Button" pin
const int GREEN_LED_PIN = 8;									// the number8 of the "Green LED" pin
const int BUTTON_LED_PIN = 9;									// the number9 of the "Button LED" pin
const int buzzer = 10;											// the number10 of the "buzzer" pin

// variables will change:
int buttonState1;												// variable for reading the wifiButton status
int buttonState2;												// variable for reading the homeButton status
int buttonState3;												// variable for reading the _433Button status

void setup() 
{
	Serial.begin(115200);
	Serial.println("Hello!");

	pinMode(ralayPin4, OUTPUT);									// initialize the relay pin as output
	pinMode(GREEN_LED_PIN, OUTPUT);								// initialize the LED as output
	pinMode(BUTTON_LED_PIN, OUTPUT);							// initialize the BUTTON LED as output
	pinMode(buzzer, OUTPUT); 									// initialize the buzzer as output
	pinMode(wifiButton, INPUT_PULLUP);							// initialize the WIFI button pin as input
	pinMode(homeButton, INPUT_PULLUP);							// initialize the home push button pin as input
	pinMode(_433Button, INPUT_PULLUP);							// initialize the 433 relay button pin as input
	digitalWrite(GREEN_LED_PIN, HIGH);							// turn GREEN LED ON
	digitalWrite(BUTTON_LED_PIN, HIGH);							// turn GREEN BUTTON LED ON

	nfc.begin();

	uint32_t versiondata = nfc.getFirmwareVersion();
	if (! versiondata) 
	{
		Serial.print("Didn't find PN53x board");
		while (1); // halt
	}
																// Got OK data, print it out!

	Serial.print("Found chip PN5"); 
	Serial.println((versiondata>>24) & 0xFF, HEX);Serial.print("Firmware ver. ");Serial.print((versiondata>>16) & 0xFF, DEC);Serial.print('.'); 
	Serial.println((versiondata>>8) & 0xFF, DEC);

																// Set the max number of retry attempts to read from a card
																// This prevents us from waiting forever for a card, which is
																// the default behavior of the PN532.
	nfc.setPassiveActivationRetries(0xFF);
																// configure board to read NFC tags
	nfc.SAMConfig();

	Serial.println("Waiting for an ISO14443A card");
}

void loop() 
{
	
	digitalWrite(GREEN_LED_PIN, LOW);							// turn GREEN LED OFF
	digitalWrite(BUTTON_LED_PIN, LOW);							// turn GREEN BUTTON LED OFF
	digitalWrite(ralayPin4, LOW);								// relay ON

	buttonState1 = digitalRead(wifiButton);						// read the state of the pushbutton value:
																// check if the pushbutton is pressed.
																// if it is, the buttonState is LOW:
			if (buttonState1 == LOW) 
				{												// pin 5 is pressed and connected to GND so it will be LOW
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else
				{
					digitalWrite(buttonState1, HIGH);			// add 5v to arduino pin 4 so relay in1 will be 5v and this make relay off
				}

	buttonState2 = digitalRead(homeButton);						// read the state of the pushbutton value:
																// check if the pushbutton is pressed.
																// if it is, the buttonState is LOW:
			if (buttonState2 ==LOW) 
				{												// pin 6 is pressed and connected to GND so it will be LOW
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(BUTTON_LED_PIN, HIGH);			// turn GREEN BUTTON LED ON
					tone(buzzer, 1250);							// Send 1.250 KHz sound signal
					delay(1500);									// delay for 0.5 sec 
					noTone(buzzer);								// buzzer off
					digitalWrite(BUTTON_LED_PIN, LOW);			// turn GREEN BUTTON LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else
				{
					digitalWrite(buttonState1, HIGH);			// add 5v to arduino pin 4 so relay in1 will be 5v and this make relay off
				}

	buttonState3 = digitalRead(_433Button);						// read the state of the pushbutton value:
																// check if the pushbutton is pressed.
																// if it is, the buttonState is LOW:
			if (buttonState3 == LOW) 
				{												// pin 7 is pressed and connected to GND so it will be LOW
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else
				{
					digitalWrite(buttonState3, HIGH);			// add 5v to arduino pin 4 so relay in1 will be 5v and this make relay off
				}

 //*********************** ADD MORE HERE ***********************//

	boolean success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };					// Buffer to store the returned UID
	uint8_t uidLength;											// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
																// Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
																// 'uid' will be populated with the UID, and uidLength will indicate
																// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)

	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

	if (success) 
		{
			Serial.println("Found something ...");
			Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
			Serial.print("UID Value: ");
			String hex_value = "";
			for (uint8_t i=0; i < uidLength; i++) 
				{
					Serial.print(" 0x");Serial.print(uid[i], HEX);
					//Serial.print(" ");Serial.print(uid[i], HEX);
					hex_value += (String)uid[i];
				}

			Serial.println(", value="+hex_value);

			if(hex_value == "132120147215") 
				{
					Serial.println("This is Key Tag. - G-Chip");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else if(hex_value == "1535839131") 
				{
					Serial.println("This is Key Tag. - Chip-1");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else if(hex_value == "12324281115")
				{
					Serial.println("This is Key Tag. - Chip-2");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else if(hex_value == "22113381211")
				{
					Serial.println("This is Key Tag. - Chip-3");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
				
			else if(hex_value == "2112178928") 
				{
					Serial.println("This is Card-1");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else if(hex_value == "21225445") 
				{
					Serial.println("This is Card-2");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
				
			else if(hex_value == "5197124137") 
				{
					Serial.println("This is Card-3");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					delay (2000);								// wait 2 second
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}
			else if(hex_value == "49219324211576129") 
				{
					Serial.println("This is paper chip");
					digitalWrite(ralayPin4, HIGH);				// relay OFF
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					tone(buzzer, 1250);							// Send 1.250 KHz sound signal
					delay(400);									// delay for 0.5 sec 
					noTone(buzzer);								// buzzer off
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					delay(400);									// delay for 0.5 sec 
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					tone(buzzer, 1250);							// Send 1.250 KHz sound signal
					delay(400);									// delay for 0.5 sec 
					noTone(buzzer);								// buzzer off
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					delay(400);									// delay for 0.5 sec 
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					tone(buzzer, 1250);							// Send 1.250 KHz sound signal
					delay(400);									// delay for 0.5 sec 
					noTone(buzzer);								// buzzer off
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					delay(400);									// delay for 0.5 sec 
					digitalWrite(GREEN_LED_PIN, HIGH);			// turn GREEN LED ON
					tone(buzzer, 1250);							// Send 1.250 KHz sound signal
					delay(400);									// delay for 0.5 sec 
					noTone(buzzer);								// buzzer off
					digitalWrite(GREEN_LED_PIN, LOW);			// turn GREEN LED OFF
					digitalWrite(ralayPin4, LOW);				// relay ON
				}

 //*********************** ADD MORE HERE ***********************//

			else
			{
				Serial.println("I don't know.");
				Serial.println("");
				tone(buzzer, 1000);								// Send 1KHz sound signal
				delay(1000);									// ... for 1 sec
				noTone(buzzer);									// Stop sound.
				delay(500);										// Wait 0.5 second before continuing
			}
		}
	else
		{
			Serial.println("Waiting for a card ...");			// PN532 probably timed out waiting for a card
		}
}
