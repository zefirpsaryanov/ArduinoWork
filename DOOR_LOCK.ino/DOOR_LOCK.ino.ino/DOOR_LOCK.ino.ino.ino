/* PN532 NFC RFID module V3 (PN532) [S192] : http://rdiot.tistory.com/173 [RDIoT Demo] */

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
  
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

const int output3 = 3;							// the number of the relay1 pin
const int wifiButton = 5;						// the number of the pushbutton pin
const int button1 = 7;                     		// the number of the pushbutton pin


// variables will change:
int buttonState1;                       		// variable for reading the pushbutton1 status
int buttonState2;                       		// variable for reading the pushbutton1 status

void setup(void) {
	Serial.begin(115200);
	Serial.println("Hello!");
  
	pinMode(output3, OUTPUT);               	// initialize the relay pin as an output:
	pinMode(button1, INPUT_PULLUP);         	// initialize the pushbutton pin as an input:
	pinMode(wifiButton, INPUT_PULLUP);         	// initialize the pushbutton pin as an input:

	nfc.begin();

	uint32_t versiondata = nfc.getFirmwareVersion();
	if (! versiondata) 
	{
		Serial.print("Didn't find PN53x board");
		while (1); // halt
	}
  
	// Got ok data, print it out!
	
	Serial.print("Found chip PN5"); 
	Serial.println((versiondata>>24) & 0xFF, HEX); 	Serial.print("Firmware ver. "); 	Serial.print((versiondata>>16) & 0xFF, DEC); 	Serial.print('.'); 
	Serial.println((versiondata>>8) & 0xFF, DEC);
  
	// Set the max number of retry attempts to read from a card
	// This prevents us from waiting forever for a card, which is
	// the default behaviour of the PN532.
	
	nfc.setPassiveActivationRetries(0xFF);
  
	// configure board to read RFID tags
	
	nfc.SAMConfig();
    
	Serial.println("Waiting for an ISO14443A card");
}

void loop(void) 
{
	buttonState1 = digitalRead(button1);    		// read the state of the pushbutton value:
													// check if the pushbutton is pressed.
													// if it is, the buttonState is LOW:
		if (buttonState1 == LOW) 
			{                                     	// pin 2 is pressed and connected to GND so it will be LOW
				digitalWrite(output3, LOW);         // remove 5v from pin 11 so relay in1 will be 0v and this make relay on
				delay (2000);                       // wait 5 second
				Serial.println("buttonState1");
			} 
		else										//
			{                                        
				digitalWrite(output3, HIGH);        // add 5v to arduino pin 11 so relay in1 will be 5v and this make relay off
			}
	buttonState2 = digitalRead(wifiButton);    		// read the state of the pushbutton value:
													// check if the pushbutton is pressed.
													// if it is, the buttonState is LOW:
		if (buttonState2 == LOW) 
			{                                     	// pin 2 is pressed and connected to GND so it will be LOW
				digitalWrite(output3, LOW);         // remove 5v from pin 11 so relay in1 will be 0v and this make relay on
				delay (2000);                       // wait 5 second
				Serial.println("buttonState2");
			} 
		else
			{                                        
				digitalWrite(output3, HIGH);        // add 5v to arduino pin 11 so relay in1 will be 5v and this make relay off
			}
  
	boolean success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  		// Buffer to store the returned UID
	uint8_t uidLength;                        		// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
	// Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
	// 'uid' will be populated with the UID, and uidLength will indicate
	// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
	
	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
	if (success) 
		{
			Serial.println("Found a card!");
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
					Serial.println("This is Key Tag. - Gasprom");
					digitalWrite(output3, LOW);           	// remove 5v from pin 11 so relay in1 will be 0v and this make relay on
					delay (2000);                          	// wait 5 second
					digitalWrite(output3, HIGH);          	// add 5v to arduino pin 11 so relay in1 will be 5v and this make relay off
				}
			else if(hex_value == "1535839131") 
				{
					Serial.println("This is Key Tag. - Chip-1");
					digitalWrite(output3, LOW);          	// remove 5v from pin 11 so relay in1 will be 0v and this make relay on
					delay (2000);                          	// wait 5 second
					digitalWrite(output3, HIGH);          	// add 5v to arduino pin 11 so relay in1 will be 5v and this make relay off
				}
			else if(hex_value == "12324281115")
				{
					Serial.println("This is Key Tag. - Chip-2");
					digitalWrite(output3, LOW);           	// remove 5v from pin 11 so relay in1 will be 0v and this make relay on
					delay (2000);                          	// wait 5 second
					digitalWrite(output3, HIGH);          	// add 5v to arduino pin 11 so relay in1 will be 5v and this make relay off
				}
			else if(hex_value == "2112178928") 
				{
					Serial.println("This is Card");
					digitalWrite(output3, LOW);           	// remove 5v from pin 11 so relay in1 will be 0v and this make relay on
					delay (2000);                          	// wait 5 second
					digitalWrite(output3, HIGH);          	// add 5v to arduino pin 11 so relay in1 will be 5v and this make relay off
				}
				
			//**********************************************//	
				
			else
			Serial.println("I don't know.");
			Serial.println("");
			// Wait 1 second before continuing
			delay(500);
		}
	else
		{
			Serial.println("Waiting for a card...");		// PN532 probably timed out waiting for a card
		}
}
