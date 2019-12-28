#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 


void draw(void) 
{
 
  u8g.setFont(u8g_font_7x13);                       //dont change any thing
 
  u8g.drawStr( 0, 9,    "Hello 1 World!");
  u8g.drawStr( 100, 9,  "7777");
  u8g.drawStr( 0, 20,   "Hello 2 World!");
  u8g.drawStr( 100, 20,  "8888");
  u8g.drawStr( 0, 31,   "Hello 3 World!");
  u8g.drawStr( 100, 31,  "9999");
  u8g.drawStr( 0, 42,   "Hello 4 World!");
  u8g.drawStr( 100, 42,  "0000");
  u8g.drawStr( 0, 53,   "Hello 5 World!");
  u8g.drawStr( 100, 53,  "2222");
  u8g.drawStr( 0, 64,   "Hello 6 World!");
  u8g.drawStr( 100, 64,  "3333");
}

void setup(void) 
{
  // flip screen, if required
   //u8g.setRot270();
  
  
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do 
  {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  //delay(50);
}

