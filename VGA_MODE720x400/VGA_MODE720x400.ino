#include <ESP32Lib.h>
#include <Ressources/CodePage437_9x16.h>
#include <TinyGPS++.h> // com2 / uart2
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);


const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

int h = 400;
int w = 720;

int clockCenterX = h / 2;
int clockCenterY = h / 2;

VGA3BitI vga;

void drawMark(int h)
{
  float x1, y1, x2, y2;

  h = h * 30;
  h = h + 270;
  x1 = 45 * cos(h * 0.0174532925);
  y1 = 45 * sin(h * 0.0174532925);
  x2 = 45 * cos(h * 0.0174532925);
  y2 = 45 * sin(h * 0.0174532925);

  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 6);

  float sx = 0, sy = 1;
  uint16_t xx0 = 0, xx1 = 0, yy0 = 0, yy1 = 0;

  for (int i = 0; i < 360; i += 6)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    xx0 = sx * clockCenterX + clockCenterX; //??
    yy0 = sy * clockCenterY + clockCenterY; //??

    vga.fillCircle(xx0, yy0, 2, 6);
  }

}

void drawClockFrame()
{
  vga.fillCircle(clockCenterX, clockCenterY, clockCenterX - 1, 6); // lite blue
  vga.fillCircle(clockCenterX, clockCenterY, clockCenterX - 3, 0); // black
  vga.fillCircle(clockCenterX, clockCenterY, 4, 1);                // red

  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0)); // white
  vga.setCursor(clockCenterX + 170, clockCenterY - 5); vga.print("3");
  vga.setCursor(clockCenterX - 5, clockCenterY + 170); vga.print("6");
  vga.setCursor(clockCenterX - 170, clockCenterY - 5); vga.print("9");
  vga.setCursor(clockCenterX - 5, clockCenterY - 170); vga.print("12");

  for (int i = 0; i < 12; i++)
  {
    //if ((i % 3) != 0)
    drawMark(i);
  }
}

void drawSec(int s) //done
{
  float x1, y1, x2, y2;
  int ps = s - 1;
  if (ps == -1)
    ps = 59;
  ps = ps * 6;
  ps = ps + 270;
  x1 = 120 * cos(ps * 0.0174532925);
  y1 = 120 * sin(ps * 0.0174532925);
  x2 = 10 * cos(ps * 0.0174532925);
  y2 = 10 * sin(ps * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  s = s * 6;
  s = s + 270;
  x1 = 120 * cos(s * 0.0174532925);
  y1 = 120 * sin(s * 0.0174532925);
  x2 = 10 * cos(s * 0.0174532925);
  y2 = 10 * sin(s * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
}

void drawMin(int m) //done
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int pm = m - 1;
  int w = 5;
  if (pm == -1)
    pm = 59;
  pm = pm * 6;
  pm = pm + 270;
  x1 = 100 * cos(pm * 0.0174532925);
  y1 = 100 * sin(pm * 0.0174532925);
  x2 = 10 * cos(pm * 0.0174532925);
  y2 = 10 * sin(pm * 0.0174532925);
  x3 = 20 * cos((pm + w) * 0.0174532925);
  y3 = 20 * sin((pm + w) * 0.0174532925);
  x4 = 20 * cos((pm - w) * 0.0174532925);
  y4 = 20 * sin((pm - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 0);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 0);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 0);
  m = m * 6;
  m = m + 270;
  x1 = 100 * cos(pm * 0.0174532925);
  y1 = 100 * sin(pm * 0.0174532925);
  x2 = 10 * cos(pm * 0.0174532925);
  y2 = 10 * sin(pm * 0.0174532925);
  x3 = 20 * cos((pm + w) * 0.0174532925);
  y3 = 20 * sin((pm + w) * 0.0174532925);
  x4 = 20 * cos((pm - w) * 0.0174532925);
  y4 = 20 * sin((pm - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 7);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 7);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 7);
}

void drawHour(int h, int m) //done
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int ph = h;
  //int color = 6;
  int w = 7;
  if (m == 0) {
    ph = ((ph - 1) * 30) + ((m + 59) / 2);
  }
  else {
    ph = (ph * 30) + ((m - 1) / 2);
  }
  ph = ph + 270;
  x1 = 90 * cos(ph * 0.0174532925);
  y1 = 90 * sin(ph * 0.0174532925);
  x2 = 10  * cos(ph * 0.0174532925);
  y2 = 10  * sin(ph * 0.0174532925);
  x3 = 20 * cos((ph + w) * 0.0174532925);
  y3 = 20 * sin((ph + w) * 0.0174532925);
  x4 = 20 * cos((ph - w) * 0.0174532925);
  y4 = 20 * sin((ph - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 0);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 0);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 0);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 0);
  h = (h * 30) + (m / 2);
  h = h + 270;
  x1 = 90 * cos(ph * 0.0174532925);
  y1 = 90 * sin(ph * 0.0174532925);
  x2 = 10  * cos(ph * 0.0174532925);
  y2 = 10  * sin(ph * 0.0174532925);
  x3 = 20 * cos((ph + w) * 0.0174532925);
  y3 = 20 * sin((ph + w) * 0.0174532925);
  x4 = 20 * cos((ph - w) * 0.0174532925);
  y4 = 20 * sin((ph - w) * 0.0174532925);
  vga.line(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY, 7);
  vga.line(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY, 7);
  vga.line(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY, 7);
  vga.line(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY, 7);
}

void setup()
{
  vga.init(vga.MODE720x400, redPin, greenPin, bluePin, hsyncPin, vsyncPin); //MODE640x480, MODE400x300, MODE720x400
  vga.setFont(CodePage437_9x16);
  vga.clear();

  //vga.setTextColor(vga.RGB(0, 255, 0), vga.RGB(0, 0, 0)); // green
  //vga.fillRect(1, 15, 700, 380, 4);

  //  vga.fillCircle(1, 10, 2, 6);
  //  vga.fillCircle(1, 398, 2, 6);
  //  vga.fillCircle(714, 10, 2, 6);
  //  vga.fillCircle(714, 398, 2, 6);

  drawClockFrame();
}
/*
     0 -> black
     1 -> red
     2 -> green
     3 -> yellow
     4 -> blue
     5 -> pink
     6 -> lite blue
     7 -> white
*/
//mainloop
void loop()
{
  drawSec(gps.time.second());
  drawMin(gps.time.minute());
  drawHour(gps.time.hour() + 2, gps.time.minute());
}
