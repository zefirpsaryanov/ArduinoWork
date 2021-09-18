#include <FastLED.h>
#define LED_PIN     PD2
#define NUM_LEDS    113

CRGB leds[NUM_LEDS];

int brightLevel = 0;
int brightVal = 0;

int redLevel = 0;
int redVal = 0;

int greenLevel = 0;
int greenVal = 0;

int blueLevel = 0;
int blueVal = 0;

int analogMin = 50;
int analogMax = 950;

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop()
{

  brightVal = constrain(analogRead(A0), analogMin, analogMax);  // limits range of sensor values to between analogMin and analogMax
  brightLevel  = map(brightVal, analogMin, analogMax, 0, 8);  //bright to %

  redVal = constrain(analogRead(A1), analogMin, analogMax);
  redLevel = map(redVal, analogMin, analogMax, 0, 250);   //red to %

  greenVal = constrain(analogRead(A2), analogMin, analogMax);
  greenLevel = map(greenVal, analogMin, analogMax, 0, 250); //grean to %

  blueVal = constrain(analogRead(A3), analogMin, analogMax);
  blueLevel = map(blueVal, analogMin, analogMax, 0, 250);  //blue to %

  FastLED.setBrightness(brightLevel);


  for (int i = 0; i <= 113; i++)
  {
    leds[i] = CRGB(redLevel, greenLevel, blueLevel);
    delay(1);
  }
//  leds[0] = CRGB(redLevel, greenLevel, blueLevel);

  FastLED.show();

  Serial.println("");

  /*-------Bright Level-------*/
  Serial.print("1 = ");
  Serial.print(analogRead(A0));
  Serial.print(" ,Bright= ");
  Serial.print(brightLevel);
  Serial.print(" ; ");

  /*-------Red Level-------*/
  Serial.print("2 = ");
  Serial.print(analogRead(A1));
  Serial.print(" ,Red= ");
  Serial.print(redLevel);
  Serial.print(" ; ");

  /*-------Green Level-------*/
  Serial.print("3 = ");
  Serial.print(analogRead(A2));
  Serial.print(" ,Green= ");
  Serial.print(greenLevel);
  Serial.print(" ; ");

  /*-------Blue Level-------*/
  Serial.print("4 = ");
  Serial.print(analogRead(A3));
  Serial.print(" ,Blue= ");
  Serial.print(blueLevel);


}
