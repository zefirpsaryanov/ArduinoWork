#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>  // PIN 16 17

U8G2_ST7567_ENH_DG128064I_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/23, /* data=*/18, /* cs=*/15, /* dc=*/5, /* reset=*/4);
TinyGPSPlus gps;
HardwareSerial HS(2);

char tempSpeed[8];
char tempAlt[8];
char tempSats[8];
char time_date[32];

int LCD_PIN = 19;
int DUTY = 64;
int altM;
int sats;

int EET = 2;  // !!!!!

int gpsSpeed;

void setup() {
  Serial.begin(9600);
  HS.begin(9600, SERIAL_8N1, 17, 16);
  pinMode(LCD_PIN, OUTPUT);
  analogWrite(LCD_PIN, DUTY);
  pinMode(12, OUTPUT);
  u8g2.begin();
  u8g2.setContrast(160);
}

void loop() {
  u8g2.clearBuffer();
  smartDelay(0);
  calcs();
  sprints();
  drawScrean();
  delay(200);
  u8g2.sendBuffer();
}

void sprints() {
  sprintf(tempSpeed, "%03d", int(gpsSpeed));
  sprintf(tempAlt, "%04d", int(altM));
  sprintf(tempSats, "%02d", int(sats));

  sprintf(time_date, "%02d:%02d:%02d %02d/%02d/%02d", gps.time.hour() + EET, gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
}
void calcs() {
  !gps.speed.isValid() ? gpsSpeed = 0 : gpsSpeed = gps.speed.kmph();
  !gps.altitude.isValid() ? altM = 0 : altM = gps.altitude.meters();
  !gps.satellites.isValid() ? sats = 0 : sats = gps.satellites.value();
}

void drawScrean() {
  u8g2.setFont(u8g2_font_6x13_t_cyrillic);
  u8g2.setCursor(0, 10);
  u8g2.print(time_date);

  u8g2.drawUTF8(0, 20, "Скорост");
  u8g2.setCursor(54, 20);
  u8g2.print(gpsSpeed);

  u8g2.drawUTF8(0, 30, "Височина");
  u8g2.setCursor(54, 30);
  u8g2.print(altM);

  u8g2.drawUTF8(0, 40, "Точност");
  u8g2.setCursor(54, 40);
  u8g2.print(sats);

  u8g2.drawUTF8(0, 50, "Скорост");
  u8g2.setCursor(54, 50);
  u8g2.print(gpsSpeed);

  u8g2.drawUTF8(0, 60, "Височина");
  u8g2.setCursor(54, 60);
  u8g2.print(altM);
}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (HS.available())
      gps.encode(HS.read());
  } while (millis() - start < ms);
}