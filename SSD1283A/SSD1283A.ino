#include <LCDWIKI_GUI.h> //Core graphics library
#include <SSD1283A.h> //Hardware-specific library

SSD1283A_GUI mylcd(/*CS=*/ 15, /*DC=*/ 16, /*RST=*/ 4, /*LED=*/ 0); //hardware spi,cs,cd,reset,led

//VCC - 3.3 , GND , CS - 15, RST - 4 , A0/DC - 16 , SDA - 23 , SCK - 18 , LED -0


#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup()
{
  mylcd.init();
  mylcd.Fill_Screen(BLACK);

  pinMode( 0, OUTPUT );
  analogWrite(0, 254); //0-254
}

void loop()
{
  mylcd.Fill_Screen(0x0000);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(1);
  mylcd.Print_String("Hello World!", 0, 0);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 8, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 16, 0, ' ', 16);

  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String("Hello", 0, 32);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 48, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 64, 0, ' ', 16);

  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Hello", 0, 86);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 110, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 134, 0, ' ', 16);

  delay(3000);
}
