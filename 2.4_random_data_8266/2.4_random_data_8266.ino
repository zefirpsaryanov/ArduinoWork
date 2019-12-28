#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC D8
#define TFT_CS D0
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


#define GLCD_CL_BLACK 0x0000
#define GLCD_CL_WHITE 0xFFFF
#define GLCD_CL_GRAY 0x7BEF
#define GLCD_CL_LIGHT_GRAY 0xC618
#define GLCD_CL_GREEN 0x07E0
#define GLCD_CL_LIME 0x87E0
#define GLCD_CL_BLUE 0x001F
#define GLCD_CL_RED 0xF800
#define GLCD_CL_AQUA 0x5D1C
#define GLCD_CL_YELLOW 0xFFE0
#define GLCD_CL_MAGENTA 0xF81F
#define GLCD_CL_CYAN 0x07FF
#define GLCD_CL_DARK_CYAN 0x03EF
#define GLCD_CL_ORANGE 0xFCA0
#define GLCD_CL_PINK 0xF97F
#define GLCD_CL_BROWN 0x8200
#define GLCD_CL_VIOLET 0x9199
#define GLCD_CL_SILVER 0xA510
#define GLCD_CL_GOLD 0xA508
#define GLCD_CL_NAVY 0x000F
#define GLCD_CL_MAROON 0x7800
#define GLCD_CL_PURPLE 0x780F
#define GLCD_CL_OLIVE 0x7BE0


char BX[20];
int lectV;

void setup() {
  tft.begin();
  tft.setRotation(1); tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(GLCD_CL_SILVER, ILI9341_BLACK); tft.setTextSize(2);
  tft.setCursor(0, 5); tft.print("Number");

  sprintf(BX,"Volume = %4d ml", lectV);
  tft.setCursor(10, 50);  tft.print(BX);
}

void loop() {
  lectV = random(9000); 
  sprintf(BX,"Volume = %4d ml", lectV);
  tft.setCursor(10, 50);  tft.print(BX);
  delay(500);
}
