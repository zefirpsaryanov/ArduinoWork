#include "U8glib.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // I2C / TWI 
//U8G2_SSD1306_64X48_ER_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered


const uint8_t bm[] PROGMEM = {
  0b00011000,
  0b00111100,
  0b01111110,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000
};

static int WIDTH=128;
static int HEIGHT=64;

int x, y;

void setup(void) {
  x = 0;
  y = 0;
}

void loop(void) {

  u8g.firstPage();  
  do {
    u8g.drawBitmapP( x, y, 1, 8, bm);
  } while( u8g.nextPage() );
  
  delay(100);

  x += 8;
  if( x >= WIDTH){
    x = 0;
    y += 8;
    if( y >= HEIGHT){
      y = 0;
    }
  }
}
