#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define TFT_CS   15       // CS  ->D8
#define TFT_DC   12       // DC  ->D6
#define TFT_RST  -1       // RES ->RES
#define TFT_MOSI 14       // SDA ->D5
#define TFT_SCLK 13       // SCL ->D7 

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup(void) {
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
