/*
 * PCD8544 - Interface with Philips PCD8544 (or compatible) LCDs.
 *
 * Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * To use this sketch, connect the eight pins from your LCD like thus:
 *
 * Pin 1 -> +3.3V (rightmost, when facing the display head-on)
 * Pin 2 -> Arduino digital pin 3
 * Pin 3 -> Arduino digital pin 4
 * Pin 4 -> Arduino digital pin 5
 * Pin 5 -> Arduino digital pin 7
 * Pin 6 -> Ground
 * Pin 7 -> 10uF capacitor -> Ground
 * Pin 8 -> Arduino digital pin 6
 *
 * Since these LCDs are +3.3V devices, you have to add extra components to
 * connect it to the digital pins of the Arduino (not necessary if you are
 * using a 3.3V variant of the Arduino, such as Sparkfun's Arduino Pro).
 */

/* niq_ro ( http://nicuflorica.blogspot.ro ) case:
 For module from China, you must connect like this:
* Pin 1 (RST) -> Arduino digital 6 (D6)
* Pin 2 (CE) -> Arduino digital 7 (D7)
* Pin 3 (DC) -> Arduino digital 5 (D5)
* Pin 4 (DIN) -> Arduino digital 4 (D4)
* Pin 5 (CLK) - Arduino digital 3 (D3)
* Pin 6 (Vcc) -> +5V thru adaptor module (see http://nicuflorica.blogspot.ro/2013/06/afisajul-folosit-la-telefoanele-nokia.html )
* Pin 7 (LIGHT) -> +5V thru 56-100 ohms resistor (for permanent lights) or... other pin control
* Pin 8 (GND) -> GND1 or GND2 
*/


#include <PCD8544.h>


// A custom glyph (a smiley)...
static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };


static PCD8544 lcd;


void setup() {
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);
  
  // Add the smiley to position "0" of the ASCII table...
  lcd.createChar(0, glyph);
}


void loop() {
  // Just to show the program is alive...
  static int counter = 0;

  // Write a piece of text on the first line...
  lcd.setCursor(0, 0);
  lcd.print("Hi, all!");

  // Write a piece of text on the first line...
  lcd.setCursor(0, 2);
  lcd.print("niq_ro made");

  // Write a piece of text on the second line...
  lcd.setCursor(0, 3);
  lcd.print("tehnic.go.ro  ");
   lcd.setCursor(0, 4);
  lcd.print("              ");

  // Write the counter on the fifth line...
  lcd.setCursor(0, 5);
  lcd.print(counter, DEC);
  lcd.write(' ');
  lcd.write(0);  // write the smiley
  delay(2000);  

  // Write a piece of text on thirdh and fourth line...
  lcd.setCursor(0, 3);
  lcd.print("niqro.3x.ro    ");
  lcd.setCursor(0, 4);
  lcd.print("              ");
  delay(2000);  

  // Write a piece of text on thirdh and fourth line...
  lcd.setCursor(0, 3);
  lcd.print("nicuflorica.   ");
  lcd.setCursor(17, 4);
  lcd.print("blogspot.ro");
  delay(2000); 


  counter++;
}