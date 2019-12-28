
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SSD1306_64X48_ER_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered





// End of constructor list

void setup(void) {

  u8g2.begin();  
}

uint8_t m = 0;
uint8_t h = 0;

void loop(void) {
  char m_str[3];
  char h_str[3];
  
  strcpy(m_str, u8x8_u8toa(m, 2));		/* convert m to a string with two digits */
  strcpy(h_str, u8x8_u8toa(h, 2));    /* convert m to a string with two digits */
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_7x13_mr);
    
    u8g2.drawStr(0,11,h_str);
    u8g2.drawStr(14,11,m_str);
    u8g2.drawStr()
  } while ( u8g2.nextPage() );
  delay(50);
  m++;
  if ( m == 100 ){
    h++;
    m = 0;
  }
}

