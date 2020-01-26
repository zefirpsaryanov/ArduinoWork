#include <Wire.h>
#include <ESP8266WiFi.h>
#include <NfcAdapter.h>
#include <PN532_I2C.h>
#include <PN532.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

// Replace with your network credentials
const char* ssid     = "test";
const char* password = "1q2w3e4r";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output4State = "off";
String output5State = "off";

// Assign output variables to GPIO pins
const int output4 = D4;
const int output5 = D5;

void setup(void) 
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello!");
  
  // Initialize the output variables as outputs
  pinMode(output4, OUTPUT);
  pinMode(output5, OUTPUT);
  
  
  // Set outputs to LOW
  digitalWrite(output4, LOW);
  digitalWrite(output5, LOW);
  
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) 
    {
      Serial.print("Didn't find PN53x board");
      while (1); // halt
    }
  
  else
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

      // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    nfc.setPassiveActivationRetries(0xFF);
    
    // configure board to read RFID tags
    nfc.SAMConfig();
    
    Serial.println("Waiting for an ISO14443A card");
      
      
void loop(void) 
{
	WiFiClient client = server.available();   			// Listen for incoming clients

      if (client) 
        {                                             	// If a new client connects,
          Serial.println("New Client.");                // print a message out in the serial port
          String currentLine = "";                      // make a String to hold incoming data from the client
          while (client.connected()) 
            {                                 			// loop while the client's connected
              if (client.available()) 
                {                               		// if there's bytes to read from the client,
                  char c = client.read();               // read a byte, then
                  Serial.write(c);                      // print it out the serial monitor
                  header += c;
                  
                  if (c == '\n')              			// if the byte is a newline character 
                    {                    
														// if the current line is blank, you got two newline characters in a row.
														// that's the end of the client HTTP request, so send a response:
                      if (currentLine.length() == 0) 
                        {
														// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
														// and a content-type so the client knows what's coming, then a blank line:
							client.println("HTTP/1.1 200 OK");
							client.println("Content-type:text/html");
							client.println("Connection: close");
							client.println();
            
														// turns the GPIOs on and off
							if (header.indexOf("GET /D5/on") >= 0) 
								{
								Serial.println("GPIO D5 on");
								output5State = "on";
								digitalWrite(output5, HIGH);
								} 
							else if (header.indexOf("GET /D5/off") >= 0) 
								{
								Serial.println("GPIO D5 off");
								output5State = "off";
								digitalWrite(output5, LOW);
								} 
							else if (header.indexOf("GET /D4/on") >= 0) 
								{
								Serial.println("GPIO D4 on");
								output4State = "on";
								digitalWrite(output4, HIGH);
								} 
							else if (header.indexOf("GET /D4/off") >= 0) 
								{
								Serial.println("GPIO D4 off");
								output4State = "off";
								digitalWrite(output4, LOW);
								}
            
															// Display the HTML web page
															
							client.println("<!DOCTYPE html><html>");
							client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
							client.println("<link rel=\"icon\" href=\"data:,\">");
							
															// CSS to style the on/off buttons 
															// Feel free to change the background-color and font-size attributes to fit your preferences
															
							client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
							client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
							client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
							client.println(".button2 {background-color: #77878A;}</style></head>");
 
 
															// Web Page Heading
															
							client.println("<body><h1>ESP8266 Web Server</h1>");
															
															
															
															// Display current state, and ON/OFF buttons for GPIO 4 
															
							client.println("<p>GPIO 4 - State " + output4State + "</p>");
							
															// If the output4State is off, it displays the ON button       
							if (output4State=="off") 
								{
									client.println("<p><a href=\"/D4/on\"><button class=\"button\">ON</button></a></p>");
								}
							else 
								{
									client.println("<p><a href=\"/D4/off\"><button class=\"button button2\">OFF</button></a></p>");
								}
								
															// Display current state, and ON/OFF buttons for GPIO 5
															
							client.println("<p>GPIO 5 - State " + output5State + "</p>");
							
															// If the output5State is off, it displays the ON button 
															
								if (output5State=="off") 
								{
									client.println("<p><a href=\"/D5/on\"><button class=\"button\">ON</button></a></p>");
								} 
								else 
								{
									client.println("<p><a href=\"/D5/off\"><button class=\"button button2\">OFF</button></a></p>");
								}
								
							client.println("</body></html>");
            
															// The HTTP response ends with another blank line
							client.println();
															// Break out of the while loop
							break;
                        } 
                        else        						// if you got a newline, then clear currentLine
                        { 
                          currentLine = ""; 
                        }
                
                    } 
                    else if (c != '\r') 					// if you got anything else but a carriage return character,
                      {  
                        currentLine += c;      				// add it to the end of the currentLine
                      }
                }
            }
															// Clear the header variable
                  header = "";
															// Close the connection
                  client.stop();
                  Serial.println("Client disconnected.");
                  Serial.println("");
        }
		
	
	boolean success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };			// Buffer to store the returned UID
	uint8_t uidLength;              					// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
	digitalWrite(LED_BUILTIN, HIGH);      				// turn the LED off by making the voltage LOW
 
														// Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
														// 'uid' will be populated with the UID, and uidLength will indicate
														// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
	if (success) 
		{
			digitalWrite(LED_BUILTIN, LOW);   			// turn the LED on (HIGH is the voltage level)
			delay(1000);            					// wait for a 1 second
			Serial.println("Found a card!");
			Serial.print("UID Length: ");
			Serial.print(uidLength, DEC);
			Serial.println(" bytes");
			Serial.print("UID Value: ");
			for (uint8_t i=0; i < uidLength; i++) 
				{
					Serial.print(" 0x");Serial.print(uid[i], HEX); 
				}
			Serial.println("");						
			delay(100);									// Wait 100ms second before continuing
			
			if(hex_value == "16517722582") 
			{
				digitalWrite(output4, HIGH);
				digitalWrite(output5, HIGH);
				Serial.println("This is Key Tag. ");
				delay(3000);
			}
			else if(hex_value == "230522426") 
			{
				digitalWrite(output4, HIGH);
				digitalWrite(output5, HIGH);
				Serial.println("This is Key Tag. ");
				delay(3000);
			}
			else if(hex_value == "63156295") 
			{
				digitalWrite(output4, HIGH);
				digitalWrite(output5, HIGH);
				Serial.println("This is Key Tag. ");
				delay(3000);
			}
			else if(hex_value == "63156295") 
			{
				digitalWrite(output4, HIGH);
				digitalWrite(output5, HIGH);
				Serial.println("This is Key Tag. ");
				delay(3000);
			}
			else
			{
				Serial.println("I don't know.");
				Serial.println("");				
				delay(1000);							// Wait 1 second before continuing
      		}
		}
	else
		{
			// PN532 probably timed out waiting for a card
			Serial.println("Timed out waiting for a card");
		}

    
}