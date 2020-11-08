#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>


#include <SPI.h>

#define TFT_RST -1 //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_CS D4  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_DC D3  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0

//#define TFT_CS 14  //for D32 Pro
//#define TFT_DC 27  //for D32 Pro
//#define TFT_RST 33 //for D32 Pro
//#define TS_CS  12 //for D32 Pro touch screen :)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int w = 128;
int h = 128;

int speed11 = 0;
int rpm12 = 0;
double coolant13 = -88.8;
double temp14 = 88.88;


void setup(void)
{
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  //tft.setTextWrap(false); // Allow text to run off right edge

  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_WHITE , ST7735_BLACK);
  //tft.setTextSize(0);

  tft.drawRect(0, 0, w, h, ST7735_WHITE);

  tft.drawFastHLine(0, h / 4, h, ST7735_WHITE);
  tft.drawFastHLine(0, h / 2, h, ST7735_WHITE);
  tft.drawFastHLine(0, h / 2 + h / 4, h, ST7735_WHITE);
  tft.drawFastVLine(w / 2, 0, w, ST7735_WHITE);

  tft.setCursor(3, h / 4 - 8);
  tft.print(speed11);

  tft.setCursor(3, h / 2 - 8);
  tft.print(rpm12);

  tft.setCursor(3, h / 2 + h / 4 - 8);
  tft.print(coolant13, 1);

  tft.setCursor(3, h - 8);
  tft.print(temp14, 1);

  ////////////////////////////////////////////


  tft.setCursor(w / 2 + 3, h / 4 - 8);
  tft.print("21,21");

  tft.setCursor(w / 2 + 3, h / 2 - 8);
  tft.print("43,43");

  tft.setCursor(w / 2 + 3, h / 2 + h / 4 - 8);
  tft.print("65,65");

  tft.setCursor(w / 2 + 3, h - 8);
  tft.print("87,87");

}

void loop(void)
{

}
