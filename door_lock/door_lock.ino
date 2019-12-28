/* PN532 NFC RFID module V3 (PN532) [S192] : http://rdiot.tistory.com/173 [RDIoT Demo] */

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

const int output4 = 4;											// the number4 of the "relay1" pin
const int wifiButton1 = 5;										// the number5 of the "wifiButton1" pin
const int homeButton2 = 6;                     					// the number6 of the "homeButton2" pin
const int _433Button3 = 7;                     					// the number7 of the "_433Button3" pin
const int GREEN_LED_PIN = 8;              						// the number8 of the "Green LED"  pin

// variables will change:
int buttonState1;                       						// variable for reading the wifiButton1 status
int buttonState2;                       						// variable for reading the homeButton2 status
int buttonState3;                       						// variable for reading the _433Button3 status

void setup()
{
	Serial.begin(115200);
	Serial.println("Hello!");

	pinMode(output4, OUTPUT);               					// initialize the relay pin as an output
	pinMode(GREEN_LED_PIN, OUTPUT);       						// declare LED as output:
	pinMode(wifiButton1, INPUT_PULLUP);         				// initialize the wifi button pin as an input
	pinMode(homeButton2, INPUT_PULLUP);         				// initialize the home push button pin as an input
	pinMode(_433Button3, INPUT_PULLUP);         				// initialize the 433 relay button pin as an input
	digitalWrite(GREEN_LED_PIN, HIGH);   						// turn GREEN LED ON

	nfc.begin();

	uint32_t versiondata = nfc.getFirmwareVersion();
	if (!versiondata)
	{
		Serial.print("Didn't find PN53x board");
		while (1); // halt
	}
	// Got OK data, print it out!

	Serial.print("Found chip PN5");
	Serial.println((versiondata >> 24) & 0xFF, HEX); Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC); Serial.print('.');
	Serial.println((versiondata >> 8) & 0xFF, DEC);

	// Set the max number of retry attempts to read from a card
	// This prevents us from waiting forever for a card, which is
	// the default behavior of the PN532.
	nfc.setPassiveActivationRetries(0xFF);
	// configure board to read RFID tags
	nfc.SAMConfig();

	Serial.println("Waiting for an ISO14443A card");
}

void loop()
{

	digitalWrite(GREEN_LED_PIN, LOW);   						// turn GREEN LED OFF
	digitalWrite(output4, LOW);									// relay ON

	buttonState1 = digitalRead(wifiButton1);    				// read the state of the pushbutton value:
																// check if the pushbutton is pressed.
																// if it is, the buttonState is LOW:
	if (buttonState1 == LOW)
	{                                     			// pin 5 is pressed and connected to GND so it will be LOW
		Serial.println("buttonState1");
		digitalWrite(output4, HIGH);               	// relay OFF
		digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
		delay(2000);                             	// wait 2 second
		digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
		digitalWrite(output4, LOW);             	// relay ON
		Serial.println(buttonState1);
	}

	buttonState2 = digitalRead(homeButton2);    				// read the state of the pushbutton value:
																// check if the pushbutton is pressed.
																// if it is, the buttonState is LOW:
	if (buttonState2 == LOW)
	{                                     			// pin 6 is pressed and connected to GND so it will be LOW
		Serial.println("buttonState2");
		digitalWrite(output4, HIGH);               	// relay OFF
		digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
		delay(2000);                             	// wait 2 second
		digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
		digitalWrite(output4, LOW);             	// relay ON
		Serial.println(buttonState2);
	}
	buttonState3 = digitalRead(_433Button3);    				// read the state of the pushbutton value:
																// check if the pushbutton is pressed.
																// if it is, the buttonState is LOW:
	if (buttonState3 == LOW)
	{                                     			// pin 7 is pressed and connected to GND so it will be LOW
		Serial.println("buttonState3");
		digitalWrite(output4, HIGH);               	// relay OFF
		digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
		delay(2000);                             	// wait 2 second
		digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
		digitalWrite(output4, LOW);             	// relay ON
		Serial.println(buttonState3);
	}

	//********************* ADD MORE HERE **********************//

	boolean success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  					// Buffer to store the returned UID
	uint8_t uidLength;                        					// Length of the UID (4 or 7 bytes depending on ISO14443A card type)

																// Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
																// 'uid' will be populated with the UID, and uidLength will indicate
																// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)

	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

	if (success)
	{
		Serial.println("Found something ...");
		Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
		Serial.print("UID Value: ");
		String hex_value = "";
		for (uint8_t i = 0; i < uidLength; i++)
		{
			Serial.print(" 0x"); Serial.print(uid[i], HEX);
			//Serial.print(" ");Serial.print(uid[i], HEX);       
			hex_value += (String)uid[i];
		}

		Serial.println(", value=" + hex_value);

		if (hex_value == "132120147215")
		{
			Serial.println("This is Key Tag. - G-Chip");
			digitalWrite(output4, HIGH);               	// relay OFF
			digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
			delay(2000);                             	// wait 2 second
			digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
			digitalWrite(output4, LOW);             	// relay ON
		}
		else if (hex_value == "1535839131")
		{
			Serial.println("This is Key Tag. - Chip-1");
			digitalWrite(output4, HIGH);               	// relay OFF
			digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
			delay(2000);                             	// wait 2 second
			digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
			digitalWrite(output4, LOW);             	// relay ON
		}
		else if (hex_value == "12324281115")
		{
			Serial.println("This is Key Tag. - Chip-2");
			digitalWrite(output4, HIGH);               	// relay OFF
			digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
			delay(2000);                             	// wait 2 second
			digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
			digitalWrite(output4, LOW);             	// relay ON
		}
		else if (hex_value == "2112178928")
		{
			Serial.println("This is Card");
			digitalWrite(output4, HIGH);               	// relay OFF
			digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
			delay(2000);                             	// wait 2 second
			digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
			digitalWrite(output4, LOW);             	// relay ON
		}
		else if (hex_value == "49219324211576129")
		{
			Serial.println("This is paper chip");
			digitalWrite(output4, HIGH);               	// relay OFF
			digitalWrite(GREEN_LED_PIN, HIGH);      	// turn GREEN LED ON
			delay(2000);                             	// wait 2 second
			digitalWrite(GREEN_LED_PIN, LOW);       	// turn GREEN LED OFF
			digitalWrite(output4, LOW);             	// relay ON
		}

		//********************* ADD MORE HERE **************//

		else
			Serial.println("I don't know.");
		Serial.println("");
		// Wait 1 second before continuing
		delay(500);
	}
	else
	{
		Serial.println("Waiting for a card...");			// PN532 probably timed out waiting for a card
	}
}
