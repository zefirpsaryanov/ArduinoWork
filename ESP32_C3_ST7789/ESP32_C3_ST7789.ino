/*
   arduino-esp32 exercise run on ESP32-C3-DevKitM-1,
   display on 1.8" 128x160 TFT with SPI ST7735, and
   2.0" IPS 240x320 RGB LCD with SPI ST7789,
   using 'Adafruit ST7735 and ST7789 Library'.

   ref:
   graphicstest example under 'Adafruit ST7735 and ST7789 Library'
*/


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789


#define TFT_CS   2       // CS  ->D8
#define TFT_RST  3       // RES ->RES
#define TFT_SCLK 4       // SCL ->D7 
#define TFT_MOSI 6       // SDA ->D5
#define TFT_DC   8       // DC  ->D6

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

/*
   ===================
    TFT_ST7789 ESP32-C3
   -------------------
   BLK        3V3
   CS         2     +
   DC         8
   RES        3     +
   SDA        6
   SCL        4
   VCC        3V3
   GND        GND
*/

void setup(void) 
{
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));

  tft.init(240, 240);           // Init ST7789 240x240

  //tft.init(240, 240, SPI_MODE2);

  Serial.println(F("Initialized"));
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(3);
  tft.setRotation(2);
}

void loop()
{
  tft.setCursor(30, 30);
  tft.print(random(0, 9999));
}
