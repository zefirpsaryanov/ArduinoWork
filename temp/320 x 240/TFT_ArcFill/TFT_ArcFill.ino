#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
//#include <Adafruit_SPITFT.h>

#define TFT_CS 14  //for D32 Pro
#define TFT_DC 27  //for D32 Pro
#define TFT_RST 33 //for D32 Pro

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

#define TFT_BLACK       0x0000  ///<   0,   0,   0
#define TFT_NAVY        0x000F  ///<   0,   0, 123
#define TFT_DARKGREEN   0x03E0  ///<   0, 125,   0
#define TFT_DARKCYAN    0x03EF  ///<   0, 125, 123
#define TFT_MAROON      0x7800  ///< 123,   0,   0
#define TFT_PURPLE      0x780F  ///< 123,   0, 123
#define TFT_OLIVE       0x7BE0  ///< 123, 125,   0
#define TFT_LIGHTGREY   0xC618  ///< 198, 195, 198
#define TFT_DARKGREY    0x7BEF  ///< 123, 125, 123
#define TFT_BLUE        0x001F  ///<   0,   0, 255
#define TFT_GREEN       0x07E0  ///<   0, 255,   0
#define TFT_CYAN        0x07FF  ///<   0, 255, 255
#define TFT_RED         0xF800  ///< 255,   0,   0
#define TFT_MAGENTA     0xF81F  ///< 255,   0, 255
#define TFT_YELLOW      0xFFE0  ///< 255, 255,   0
#define TFT_WHITE       0xFFFF  ///< 255, 255, 255
#define TFT_ORANGE      0xFD20  ///< 255, 165,   0
#define TFT_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define TFT_PINK        0xFC18  ///< 255, 130, 198

#define DEG2RAD 0.0174532925

#define LOOP_DELAY 10 // Loop delay to slow things down

byte inc = 0;
unsigned int col = 0;

byte red = 31; // Red is the top 5 bits of a 16 bit colour value
byte green = 0;// Green is the middle 6 bits
byte blue = 0; // Blue is the bottom 5 bits
byte state = 0;

void setup(void) {
  tft.begin();

  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);

}


void loop() {

  // Continuous elliptical arc drawing
  fillArc(160, 120, inc * 6, 1, 140, 100, 10, rainbow(col));

  // Continuous segmented (inc*2) elliptical arc drawing
  fillArc(160, 120, ((inc * 2) % 60) * 6, 1, 120, 80, 30, rainbow(col));

  // Circle drawing using arc with arc width = radius
  fillArc(160, 120, inc * 6, 1, 42, 42, 42, rainbow(col));

  inc++;
  col += 1;
  if (col > 191) col = 0;
  if (inc > 59) inc = 0;

  delay(LOOP_DELAY);
}


// #########################################################################
// Draw a circular or elliptical arc with a defined thickness
// #########################################################################

// x,y == coords of centre of arc
// start_angle = 0 - 359
// seg_count = number of 6 degree segments to draw (60 => 360 degree arc)
// rx = x axis outer radius
// ry = y axis outer radius
// w  = width (thickness) of arc in pixels
// colour = 16 bit colour value
// Note if rx and ry are the same then an arc of a circle is drawn

int fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

  byte seg = 6; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  // Draw colour blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);

    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
}

// #########################################################################
// Return the 16 bit colour with brightness 0-100%
// #########################################################################
unsigned int brightness(unsigned int colour, int brightness)
{
  byte red   = colour >> 11;
  byte green = (colour & 0x7E0) >> 5;
  byte blue  = colour & 0x1F;

  blue =  (blue * brightness) / 100;
  green = (green * brightness) / 100;
  red =   (red * brightness) / 100;

  return (red << 11) + (green << 5) + blue;
}

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  switch (state) {
    case 0:
      green ++;
      if (green == 64) {
        green = 63;
        state = 1;
      }
      break;
    case 1:
      red--;
      if (red == 255) {
        red = 0;
        state = 2;
      }
      break;
    case 2:
      blue ++;
      if (blue == 32) {
        blue = 31;
        state = 3;
      }
      break;
    case 3:
      green --;
      if (green == 255) {
        green = 0;
        state = 4;
      }
      break;
    case 4:
      red ++;
      if (red == 32) {
        red = 31;
        state = 5;
      }
      break;
    case 5:
      blue --;
      if (blue == 255) {
        blue = 0;
        state = 0;
      }
      break;
  }
  return red << 11 | green << 5 | blue;
}