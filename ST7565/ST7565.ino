#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

//VCC - 3.3 , GND , CS - 15, RST - 4 , A0/DC - 16 , SDA - 23 , SCK - 18 , LED -0
//BK -> GND

byte CS_pin  = 15;
byte RES_pin = 4;
byte DC_pin  = 16; //A0
byte SCK_pin = 18;
byte SDA_pin = 23;
byte LED_pin = 0;

U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI u8g2(U8G2_R0, SCK_pin, SDA_pin, CS_pin, DC_pin, RES_pin);

void setup(void)
{
  u8g2.begin();
  pinMode( LED_pin, OUTPUT );
  analogWrite(LED_pin, 63); //0-254
}

void loop(void) 
{
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_7x13B_tf);	// choose a suitable font
  u8g2.drawStr(5, 10, "0123456789");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);
}
