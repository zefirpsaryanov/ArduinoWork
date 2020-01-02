#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_CS 14  //for D32 Pro
#define TFT_DC 27  //for D32 Pro
#define TFT_RST 33 //for D32 Pro

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const int button15 = 15;
const int button0 = 0;
const int button4 = 4;

int button15State , button0State , button4State , number = 0;

void setup()
{
  tft.begin();
  tft.setRotation(0);
  tft.setTextSize(2);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void loop()
{
  tft.setCursor(0, 0);
  number = random(1000);
  delay(150);
  tft.print(number);
  tft.println("");

  button15State = digitalRead(button15);
  button0State = digitalRead(button0);
  button4State = digitalRead(button4);

  tft.println(button15State);
  tft.println(button0State);
  tft.println(button4State);

  if (button15State == LOW)
  {
    tft.fillScreen(ILI9341_BLACK);
    tft.setRotation(1);
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  }
  if (button0State  == LOW)
  {
    tft.fillScreen(ILI9341_BLACK);
    tft.setRotation(2);
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  }
  if (button4State  == LOW)
  {
    tft.fillScreen(ILI9341_BLACK);
    tft.setRotation(3);
    tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  }
  // else tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
