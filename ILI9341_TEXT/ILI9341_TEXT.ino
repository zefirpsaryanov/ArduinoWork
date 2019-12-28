//Code By A.Tedds.
//Using Adafruit Library Files. Thank you.

//Usage of tft.drawRect(start position width 0-240,Start position Height 0-320,size 0-240,size 0-320, Color)

#include <Adafruit_GFX_AS.h>    // Core graphics library
#include <Adafruit_ILI9341_8bit_AS.h> // Hardware-specific library



#define LCD_CS 34 // Chip Select goes to Analog 3
#define LCD_CD 35 // Command/Data goes to Analog 2
#define LCD_WR 4 // LCD Write goes to Analog 1
#define LCD_RD 2 // LCD Read goes to Analog 0
#define LCD_RESET 36 // Can alternately just connect to Arduino's reset pin


// Assign human-readable names to some common 16-bit color values:

// With Changes for inversion !!

#define BLACK 0xFFFF
#define BLUE 0xFFE0
#define GREEN 0xF81F
#define CYAN 0xF800
#define GRAY1 0x8410
#define RED 0x07FF
#define GRAY2 0x4208
#define MAGENTA 0x07E0
#define YELLOW 0x001F
#define WHITE 0x0000


//Define TFT LCD Size
#define TFTWIDTH   240
#define TFTHEIGHT  320

Adafruit_ILI9341_8bit_AS tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);



void setup(void)
{
  tft.reset();
  delay(10);
  tft.begin(0x9341);
  tft.fillScreen(BLACK); // Clear Screen to BLACK.
  // tft.fillScreen(BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(2);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  delay (1500);
}

void loop(void)
{

}
