#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

void draw1(void) {
  u8g.setFont(u8g_font_7x13);
  u8g.drawStr( 0, 10,    "RPM - 1505");
  //u8g.drawStr( 100, 9,  "7777");
  u8g.drawStr( 0, 22,   "Coolant - 85*C");
 // u8g.drawStr( 100, 20,  "8888");
  u8g.drawStr( 0, 34,   "OIL Temp - 75*C");
 // u8g.drawStr( 100, 31,  "9999");
  u8g.drawStr( 0, 45,   "Speed - 95 km/h");
//  u8g.drawStr( 100, 42,  "0000");
//  u8g.drawStr( 0, 53,   "Hello 5 World!");
//  u8g.drawStr( 100, 53,  "2222");
//  u8g.drawStr( 0, 64,   "Hello 6 World!");
//  u8g.drawStr( 100, 64,  "3333");
}

void draw2(void) 
{
  u8g.setFont(u8g_font_7x13);
  u8g.setPrintPos(0, 10);
  u8g.print("PAGE2-TEXT4");
  u8g.setPrintPos(0, 25);
  u8g.print("PAGE2-TEXT5");
  u8g.setPrintPos(0, 40);
  //u8g.setFont(u8g_font_profont10);
  u8g.print("PAGE2-TEXT6");
}

void setup(void) 
{


}

void loop(void) 
{
  u8g.firstPage();
  do 
  {
    draw1();
  }
  while (u8g.nextPage());
  delay(4000);
  u8g.firstPage();
  do 
  {
    draw2();
  } 
  while (u8g.nextPage());
  delay(4000);
}
