/*
  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######       TO SELECT YOUR DISPLAY TYPE AND ENABLE FONTS          ######
  #########################################################################
*/
#include <Wire.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include <DFRobot_MAX30102.h>

DFRobot_MAX30102 particleSensor;
TFT_eSPI tft = TFT_eSPI();

int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid

void setup(void)
{
  Serial.begin(115200);
  particleSensor.begin();
  particleSensor.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4, \
      /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100, \
      /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);

  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.setTextFont(4);
  Serial.println("Initialized");
}

void loop()
{
  particleSensor.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("HR", 0, 0);
  tft.drawNumber(heartRate, 0, 30);

  tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
  tft.drawString("SPO2", 60, 0);
  tft.drawNumber(SPO2, 60, 30);

}
