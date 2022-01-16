/*
  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######       TO SELECT YOUR DISPLAY TYPE AND ENABLE FONTS          ######
  #########################################################################
*/
#include <Wire.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include <Adafruit_AHTX0.h>
#include <Adafruit_BME280.h>
#include "Adafruit_SHT31.h"
#include "Adafruit_HTU21DF.h"

TFT_eSPI tft = TFT_eSPI();

Adafruit_AHTX0 aht20;
Adafruit_BME280 bme280;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_HTU21DF htu21 = Adafruit_HTU21DF();

float aht20_Temperature;
float aht20_Humidity;
float bme280_Temperature;
float bme280_Humidity;
float sht31_Temperature;
float sht31_Humidity;
float htu21_Temperature;
float htu21_Humidity;

float avrg_Temperature;
float avrg_Humidity;

sensors_event_t aht_humidity, aht_temp;

void setup(void)
{
  Serial.begin(115200);
  aht20.begin();
  bme280.begin(0x76);
  sht31.begin(0x44);
  htu21.begin();

  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.setTextFont(4);
  Serial.println("Initialized");
}

void loop()
{
  aht20.getEvent(&aht_humidity, &aht_temp);
  aht20_Temperature = aht_temp.temperature;
  aht20_Humidity = aht_humidity.relative_humidity;

  bme280_Temperature = bme280.readTemperature();
  bme280_Humidity = bme280.readHumidity();

  sht31_Temperature = sht31.readTemperature();
  sht31_Humidity = sht31.readHumidity();

  htu21_Temperature = htu21.readTemperature();
  htu21_Humidity = htu21.readHumidity();

  avrg_Temperature = (bme280_Temperature + aht20_Temperature + sht31_Temperature + htu21_Temperature) / 4.0;
  avrg_Humidity = (bme280_Humidity + aht20_Humidity + sht31_Humidity + htu21_Humidity) / 4.0;

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("AHT", 0, 0);
  tft.drawFloat(aht20_Temperature, 1, 0, 30);
  tft.drawFloat(aht20_Humidity, 1, 0, 60);

  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  tft.drawString("BME", 60, 0);
  tft.drawFloat(bme280_Temperature, 1, 60, 30);
  tft.drawFloat(bme280_Humidity, 1, 60, 60);

  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  tft.drawString("SHT", 120, 0);
  tft.drawFloat(sht31_Temperature, 1, 120, 30);
  tft.drawFloat(sht31_Humidity - 6.5, 1, 120, 60);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("HTU", 180, 0);
  tft.drawFloat(htu21_Temperature, 1, 180, 30);
  tft.drawFloat(htu21_Humidity, 1, 180, 60);


  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawFloat(avrg_Temperature, 1, 50, 100);
  tft.drawFloat(avrg_Humidity, 1, 150, 100);

  //delay(1500);
}