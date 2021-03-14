/*
  This is an example of how to use the OLED 128x64 I2C with SSD1306 driver using the Adafruit library.
  It also applies to the 128x32 version, but not all components would fit the smaller screen.

  Pins:
   GND = GND
   VCC = 3.3 V
   SCL = 2
   SDA = 0

  You can connect VCC to 3.3V to reduce the amount of high pitched noise that the display produces.

  It's a good idea to put a resistor between A4-5V and A5-5V to help stabilize the connection.
  What that does is pull-up the I2C pins to make it more reliable and prevents lock-ups.

  Libraries needed:
  https://github.com/adafruit/Adafruit_SSD1306
  https://github.com/adafruit/Adafruit-GFX-Library

  Guide for the GFX Library: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-gfx-graphics-library.pdf

  List of fonts: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts

  List of fonts that support right alignment:
  FreeMono9pt7b.h
  FreeMono12pt7b.h
  FreeMono18pt7b.h
  FreeMono24pt7b.h
  FreeMonoBold9pt7b.h
  FreeMonoBold12pt7b.h
  FreeMonoBold18pt7b.h
  FreeMonoBold24pt7b.h
  FreeMonoBoldOblique9pt7b.h
  FreeMonoBoldOblique12pt7b.h
  FreeMonoBoldOblique18pt7b.h
  FreeMonoBoldOblique24pt7b.h
  FreeMonoOblique9pt7b.h
  FreeMonoOblique12pt7b.h
  FreeMonoOblique18pt7b.h
  FreeMonoOblique24pt7b.h

*/

#include <Wire.h>

#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display

Adafruit_SSD1306 display32(128, 32);  // Create display 128*32
Adafruit_SSD1306 display64(128, 64);  // Create display 128*64

#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font

#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;


void setup()  // Start of setup
{
  Wire.pins(0, 2);

  display32.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize display with the I2C address of 0x3C , Create display 128*32
  display64.begin(SSD1306_SWITCHCAPVCC, 0x3D); // Initialize display with the I2C address of 0x3D , Create display 128*64

  display32.clearDisplay();  // Clear the buffer display 128*32
  display64.clearDisplay();  // Clear the buffer display 128*64

  display32.setTextColor(WHITE);  // Set color of the text
  display64.setTextColor(WHITE);  // Set color of the text

  display32.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display64.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3

  display32.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
  display64.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.

  display32.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)
  display64.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)

  display32.setFont(&FreeMono9pt7b);  // Set a custom font
  display64.setFont(&FreeMono9pt7b);  // Set a custom font

  display32.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of
  display64.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

}

void loop()  // Start of loop
{

  // Convert Variable1 into a string, so we can change the text alignment to the right:
  // It can be also used to add or remove decimal numbers.
  // char string[10];  // Create a character array of 10 characters
  // Convert float to a string:
  // dtostrf(Variable1, 3, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)

  display32.clearDisplay();  // Clear the display so we can refresh
  display64.clearDisplay();  // Clear the display so we can refresh

  display32.setFont(&FreeMono9pt7b);  // Set a custom font
  display64.setFont(&FreeMono9pt7b);  // Set a custom font

  display32.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display64.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

  // Print text:
  display32.setCursor(3, 10);  // (x,y)
  display32.println(bmp.readTemperature());  // celsius

  display64.setCursor(3, 10);  // (x,y)
  display64.println(bmp.readTemperature())*9/5+32);  // fahrenheit 
  
  display32.display();  // Print everything we set previously
  display64.display();  // Print everything we set previously

}
