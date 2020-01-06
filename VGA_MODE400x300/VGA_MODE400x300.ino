#include <ESP32Lib.h>
#include <Ressources/CodePage437_8x19.h>


//pin configuration
const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;
int w = 400;
int h = 300;


//VGA Device
VGA3Bit vga;
void setup()
{
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(CodePage437_8x19);
 
  vga.clear();
  vga.rect(0, 0, w, h, 6);


  vga.setCursor(2, 2);
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // font color , background color font
  vga.print("WELLCOME TO OPEL");

}
void loop()
{

}
