#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
int analogMin = 0;
int analogMax = 999;
int preasure , preasureMap;
char preasurePrint[4];

TFT_eSPI tft = TFT_eSPI();
void setup(void)

{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop()
{
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  preasure = constrain(analogRead(A0), analogMin, analogMax);
  preasureMap = map(preasure, analogMin, analogMax, 0, 99); //grean to %
  sprintf(preasurePrint, "%03d", preasureMap);
  tft.drawString(preasurePrint, 0, 0, 8);
  Serial.println(analogRead(A0));
  delay(100);
}
