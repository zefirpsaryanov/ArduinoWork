/*
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)
*/
#include <Arduino.h>
#include <U8x8lib.h>

#define keyPin    12  //D6 START
#define buzzerPin 14  //D5

int timer;
int tempTimer;
char bffr[3];
int counter = 0;

U8X8_SSD1306_64X48_ER_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(keyPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  u8x8.begin();
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off
}

void loop(void)
{
  int timerValue = analogRead(0);
  timer = map(timerValue, 0, 1024, 0, 120);

  int keyStatus = digitalRead(keyPin);
  if (keyStatus == 0) // 0 - pressed
    magic(); 

  u8x8.setCursor(0, 0);
  u8x8.setFont(u8x8_font_courR24_3x4_n);
  sprintf(bffr, "%03d", timer);
  u8x8.printf(bffr);

  u8x8.setFont(u8x8_font_8x13B_1x2_n);
  u8x8.drawString(0, 4, String(counter).c_str());

  if (counter > 0)  tone(buzzerPin , 1000); //digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)

  delay(100);
}

void magic()
{
  tempTimer = timer;
  noTone(buzzerPin);
  for (int i = tempTimer -1; i > 0; i--)
  {
    tempTimer --;
    u8x8.setCursor(0, 0);
    u8x8.setFont(u8x8_font_courR24_3x4_n );
    sprintf(bffr, "%03d", tempTimer);
    u8x8.printf(bffr);
    delay(1000);
  }
  counter++;
}
