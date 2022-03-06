#include <Wire.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include <Adafruit_AHTX0.h>

TFT_eSPI tft = TFT_eSPI();

Adafruit_AHTX0 aht20;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 10000;  //the value is a number of milliseconds

float aht20_Temperature;
float aht20_Humidity;
int analogMin = 0;
int analogMax = 4095;

int relay1 = 13;
int relay2 = 14;

int relayState1 = LOW;
int relayState2 = LOW;
int A26Val, A27Val = 0;
char Val1[8];
char Val2[8];
char AVal1[8];
char AVal2[8];


sensors_event_t aht_humidity, aht_temp;

void setup(void)
{
  Serial.begin(115200);
  aht20.begin();

  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.setTextFont(4);
  Serial.println("Initialized");

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  startMillis = millis();  //initial start time
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    if (aht20_Temperature >= A26Val ) //&& aht20_Temperature < A26Val)
    {
      digitalWrite(relay1, LOW);
      relayState1 = LOW;
    }
    if
    {
      digitalWrite(relay1, HIGH);
      relayState1 = HIGH;
    }

    if (aht20_Humidity >= A27Val ) //&& aht20_Humidity < A27Val)
    {
      digitalWrite(relay2, LOW);
      relayState2 = LOW;
    }
    if
    {
      digitalWrite(relay2, HIGH);
      relayState2 = HIGH;
    }
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }

  A26Val = constrain(analogRead(26), analogMin, analogMax);
  A26Val = map(A26Val, analogMin, analogMax, -40, 85);   //temperature

  A27Val = constrain(analogRead(27), analogMin, analogMax);
  A27Val = map(A27Val, analogMin, analogMax, 0, 100);   //humidity

  aht20.getEvent(&aht_humidity, &aht_temp);
  aht20_Temperature = aht_temp.temperature;
  aht20_Humidity = aht_humidity.relative_humidity;

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("AHT", 0, 0);
  tft.drawFloat(aht20_Temperature, 1, 0, 30);
  tft.drawFloat(aht20_Humidity, 1, 0, 60);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Val", 60, 0);

  sprintf(Val1, "%03d ", A26Val);
  sprintf(Val2, "%03d ", A27Val);

  tft.drawString(String(Val1), 60, 30);
  tft.drawString(String(Val2), 60, 60);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Analog", 120, 0);

  sprintf(AVal1, "%04d ", analogRead(26));
  sprintf(AVal2, "%04d ", analogRead(27));

  tft.drawString(String(AVal1), 120, 30);
  tft.drawString(String(AVal2), 120, 60);

  //  tft.setTextColor(TFT_RED, TFT_BLACK);
  //  tft.drawString("HTU", 180, 0);
  //  tft.drawFloat(htu21_Temperature, 1, 180, 30);
  //  tft.drawFloat(htu21_Humidity, 1, 180, 60);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawNumber(relayState1, 60, 100);
  tft.drawNumber(relayState2, 120, 100);
}
