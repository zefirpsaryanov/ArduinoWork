#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "ACS712.h"
// Arduino UNO has 5.0 volt with a max ADC value of 1023 steps
// ACS712 5A  uses 185 mV per A
// ACS712 20A uses 100 mV per A
// ACS712 30A uses  66 mV per A

ACS712  ACS(A0, 5.0, 1023, 66);
// ESP 32 example (requires resistors to step down the logic voltage)
//ACS712  ACS(25, 5.0, 4095, 66);

char auth[] = "1a589190cd0e42caa727ea338ed16790";
char ssid[] = "test";
char pass[] = "1q2w3e4r";

void setup()
{
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 4); // Set "cursor" at top left corner of display (0,0) and select font 4
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("metar");

  ACS.autoMidPoint();

  WiFi.hostname("ESP-ACS712");
  //Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);
  //Blynk.begin(auth, ssid, pass, IPAddress(217, 174, 61, 217), 18080);
  WiFi.hostname("ESP-ACS712");
  delay(1000);
}

void loop()
{
  tft.setCursor(0, 0, 4);
  
  float A = ACS.mA_AC() / 1000.0;
  tft.print("ACS Amps: ");
  tft.print(A, 3);

  Blynk.virtualWrite(V26, A);  //Blynk V16 is for Temperature

}
