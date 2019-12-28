// Simple Test Sketch for Arduino UNO & 2.4" LCD TFT 320x240 Shield from Banggood.com
// SKU156315 Using the ILITEK ILI9341 or Samsung S6D04H0X Driver Chip(Almost identical chips!)
// 240x320 Resolution RGB and 262K Colors.
// Modified Sketch for UNO by KB1UIF(A.Tedds)
// Based on the new v2 simple UNO test sketch from Banggood.com.
// Various errors were in the Banggood release, which I have corrected.
// I have also added more functions and lots of comments.
// Use at your own risk!!

// Arduino UNO pin usage:
// LCD Data Bits :   7   6   5   4   3   2   1   0
// Uno dig. pins :   7   6   5   4   3   2   9   8
// Uno port/pins :  PD7 PD6 PD5 PD4 PD3 PD2 PB1 PB0

// Color maping HIGH 5Bits 00000 BLUE, middle 5Bits 00000 GREEN, Low 6 Bits 000000 RED.
// 0000 0,00 0 00,00 0000

// With MADCTL set to 0x40, Original Value 48, For RGB-BGR Flip D3.
#define WHITE 0x0000
#define RED 0xFFC0
#define GREEN 0xF81F
#define BLUE 0x07FF
#define CYAN 0x003F
#define MAGENTA 0x07C0
#define YELLOW 0xF800
#define BLACK 0xFFFF

//Define TFT LCD Size
#define TFTWIDTH   240
#define TFTHEIGHT  320

#define LCD_RD   A0 //Define A0 as LCD_RD (Read)
#define LCD_WR   A1 //Define A1 as LCD_WR (Write)    
#define LCD_RS   A2 //Define A2 as LCD_RS (0=Command/1=Data)       
#define LCD_CS   A3 //Define A3 as LCD_CS (Chip Select)     
#define LCD_RST  A4 //Define A4 as LCD_RST (Shield Reset, NOT to the Reset Button)

//-------------------------------------------------------------------------------------------
void Lcd_Writ_Bus(unsigned char d)
{
  //Clear port D relevent bits then AND input (d) with Mask (bits 0-1) for port D out (6 data bits 2-7).
  PORTD = (PORTD & B00000011) | ((d) & B11111100);
  //Clear port B relevent bits then AND input (d) with Mask (bits 2-7) for port B out (2 data bits 0-1).
  PORTB = (PORTB & B11111100) | ((d) & B00000011);
  //Now strobe the LCD shield write control line LCD_WR (A1).
  *(portOutputRegister(digitalPinToPort(LCD_WR))) &=  ~digitalPinToBitMask(LCD_WR);//LCD_WR=0
  *(portOutputRegister(digitalPinToPort(LCD_WR))) |=  digitalPinToBitMask(LCD_WR); //LCD_WR=1
}

//-------------------------------------------------------------------------------------------
void Lcd_Write_Com(unsigned char VH)
{
  *(portOutputRegister(digitalPinToPort(LCD_RS))) &=  ~digitalPinToBitMask(LCD_RS);//LCD_RS=0 (Set Control line for Command)
  Lcd_Writ_Bus(VH);//Write Hex Value of COM Register passed to Lcd_Write_Com.
}

//-------------------------------------------------------------------------------------------
void Lcd_Write_Data(unsigned char VH)
{
  *(portOutputRegister(digitalPinToPort(LCD_RS))) |=  digitalPinToBitMask(LCD_RS); //LCD_RS=1 (Set control line for Data)
  Lcd_Writ_Bus(VH);//Write Hex Value of Data passed to Lcd_Write_Data.
}
//-------------------------------------------------------------------------------------------
void Lcd_Write_Com_Data(unsigned char com, unsigned char dat)
{
  Lcd_Write_Com(com);//Write Hex Value of COM Register passed to Lcd_Write_Com_Data.
  Lcd_Write_Data(dat);//Write Hex Value of Data passed to Lcd_Write_Com_Data.
}
//-------------------------------------------------------------------------------------------
void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  Lcd_Write_Com(0x2a);//REG 2Ah=COLADDRSET Command 4 Values.
  Lcd_Write_Data(x1 >> 8); //Send Value of x1 with right bit shift x8.
  Lcd_Write_Data(x1);//Send Value of x1.
  Lcd_Write_Data(x2 >> 8); //Send Value of x2 with right bit shift x8.
  Lcd_Write_Data(x2);//Send Value of x2.

  Lcd_Write_Com(0x2b);//REG 2Bh=PAGEADDRSET Command 4 Values.
  Lcd_Write_Data(y1 >> 8); //Send Value of y1 with right bit shift x8.
  Lcd_Write_Data(y1);//Send Value of y1.
  Lcd_Write_Data(y2 >> 8); //Send Value of y2 with right bit shift x8.
  Lcd_Write_Data(y2);//Send Value of y2.

  Lcd_Write_Com(0x2c); // REG 2Ch = Memory Write
}

//-------------------------------------------------------------------------------------------
//Init commands for Shield.
void Lcd_Init(void)
{
  digitalWrite(LCD_RST, HIGH); // Take line HIGH.
  delay(5); //delay of 5mS.
  digitalWrite(LCD_RST, LOW); //Take line LOW to Reset Shield, Active LOW.
  delay(15);//delay of 15mS.
  digitalWrite(LCD_RST, HIGH); //Take Line HIGH allow Shield to function.
  delay(15);//delay of 15mS.

  digitalWrite(LCD_CS, HIGH); //Take Chip Select HIGH, Disable Shield Chip, Active LOW.
  digitalWrite(LCD_WR, HIGH); //Take Write line HIGH.
  digitalWrite(LCD_CS, LOW); //Take Chip Select LOW, Enable Shield Chip, Active LOW.

  Lcd_Write_Com(0xC0);    //Power control 1
  Lcd_Write_Data(0x23);   //VRH[5:0] 0010 0011 4.60 Volts, default is 4.50 Volts.

  Lcd_Write_Com(0xC1);    //Power control 2, Sets the factor used in the step-up circuits
  Lcd_Write_Data(0x10);   //BT[2:0] 0001 0000 default is 10

  Lcd_Write_Com(0xC5);    //C5h VCOM control 1 (Contrast?)
  Lcd_Write_Data(0x3e);   //VMH 0011 1110 +4.250 Volts
  Lcd_Write_Data(0x28);   //VML 0010 1000 -1.500 Volts

  Lcd_Write_Com(0xC7);    //C7h VCOM control2 Set the VCOM offset voltage.
  Lcd_Write_Data(0x86);   //1000 0110 VML=58 VMH=58

  Lcd_Write_Com(0x36);    //MADCTL, Memory Access Control.
  Lcd_Write_Data(0x40);   //Original Value 48, For RGB-BGR Flip D3 40h

  Lcd_Write_Com(0x3A);    //COLMOD (Interface Pixel Format)
  Lcd_Write_Data(0x55);   //RGB Interface, Color Format 16Bit,Control Interface Color Format 16bit.

  Lcd_Write_Com(0xB1);    //B1h, Frame Rate Control.
  Lcd_Write_Data(0x00);   //[1:0] DIVA division ratio 00 = fosc
  Lcd_Write_Data(0x18);   //[4:0] RTNA Frame Rate 18h 0001 1000 = 79Hz

  Lcd_Write_Com(0xB6);    //B6h Display Function Control.
  Lcd_Write_Data(0x08);   //08h PTG [3:2] & PT [1:0] 0000 1000
  //PTG [3:2]: Set the scan mode in non-display area.
  //PT [1:0]: Determine source/VCOM output in a non-display area in the partial display mode.
  Lcd_Write_Data(0x82);   //1000 0010 REV[D7] GS[D6] SS[D5] SM[D4] ISC[3:0]=5Frames 85mS.
  Lcd_Write_Data(0x27);   //0010 0111 NL[5:0]

  Lcd_Write_Com(0x11);    //Wake, Exit Sleep.
  delay(120);             //120mS Delay to allow shield to settle.

  Lcd_Write_Com(0x29);    //Display on.

  Lcd_Write_Com(0x2c);    //Memory Write.
}
//-------------------------------------------------------------------------------------------
void invertColor()
{
  Lcd_Write_Com(0x21); //Invert Colors.
}
//-------------------------------------------------------------------------------------------
void normalColor()
{
  Lcd_Write_Com(0x20); //Normal Colors.
}
//-------------------------------------------------------------------------------------------
void blink(int Times, int DL) //invert colors #Times, DL delay between each.
{
  for (int t = 1; t <= Times; ++t)
  {
    Lcd_Write_Com(0x21); //Invert
    delay(DL);           //DL delay between inversion and normal.
    Lcd_Write_Com(0x20); //Normal
    delay(DL);           //DL delay between normal and inversion.
  }
}
//-------------------------------------------------------------------------------------------
void drawCircle(int x, int y, int radius, unsigned color)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x1 = 0;
  int y1 = radius;

  digitalWrite(LCD_CS, LOW);//Chip Select Active.
  Address_set(x, y + radius, x, y + radius);
  Lcd_Write_Com(0x2C);
  Lcd_Write_Data(color >> 8);
  Lcd_Write_Data(color);
  Address_set(x, y - radius, x, y - radius);
  Lcd_Write_Com(0x2C);
  Lcd_Write_Data(color >> 8);
  Lcd_Write_Data(color);
  Address_set(x + radius, y, x + radius, y);
  Lcd_Write_Com(0x2C);
  Lcd_Write_Data(color >> 8);
  Lcd_Write_Data(color);
  Address_set(x - radius, y, x - radius, y);
  Lcd_Write_Com(0x2C);
  Lcd_Write_Data(color >> 8);
  Lcd_Write_Data(color);

  while (x1 < y1)
  {
    if (f >= 0)
    {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;
    Address_set(x + x1, y + y1, x + x1, y + y1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
    Address_set(x - x1, y + y1, x - x1, y + y1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
    Address_set(x + x1, y - y1, x + x1, y - y1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
    Address_set(x - x1, y - y1, x - x1, y - y1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
    Address_set(x + y1, y + x1, x + y1, y + x1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
    Address_set(x - y1, y + x1, x - y1, y + x1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
    Address_set(x + y1, y - x1, x + y1, y - x1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
    Address_set(x - y1, y - x1, x - y1, y - x1);
    Lcd_Write_Com(0x2C);
    Lcd_Write_Data(color >> 8);
    Lcd_Write_Data(color);
  }
}
//-------------------------------------------------------------------------------------------
void drawPixel(unsigned int x, unsigned int y, unsigned int color)
{
  //Check for values falling in display area.
  if ((x < 0) || (y < 0) || (x >= TFTWIDTH) || (y >= TFTHEIGHT)) return;

  digitalWrite(LCD_CS, LOW);// Chip Select active
  Address_set(x, y, TFTWIDTH - 1, TFTHEIGHT - 1);
  digitalWrite(LCD_CS, LOW);// Chip Select active
  Lcd_Write_Com(0x2C);
  Lcd_Write_Data(color >> 8);
  Lcd_Write_Data(color);
}
//-------------------------------------------------------------------------------------------
void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
  unsigned int  dx = (x2 > x1 ? x2 - x1 : x1 - x2);
  short     xstep =  x2 > x1 ? 1 : -1;
  unsigned int  dy = (y2 > y1 ? y2 - y1 : y1 - y2);
  short     ystep =  y2 > y1 ? 1 : -1;
  int       col = x1, row = y1;

  digitalWrite(LCD_CS, LOW);
  if (dx < dy)
  {
    int t = - (dy >> 1);
    while (true)
    {
      Address_set (col, row, col, row);
      Lcd_Write_Com(0x2C);
      Lcd_Write_Data(color >> 8);
      Lcd_Write_Data(color);
      if (row == y2)
        return;
      row += ystep;
      t += dx;
      if (t >= 0)
      {
        col += xstep;
        t   -= dy;
      }
    }
  }
  else
  {
    int t = - (dx >> 1);
    while (true)
    {
      Address_set (col, row, col, row);
      Lcd_Write_Com(0x2C);
      Lcd_Write_Data(color >> 8);
      Lcd_Write_Data(color);
      if (col == x2)
        return;
      col += xstep;
      t += dy;
      if (t >= 0)
      {
        row += ystep;
        t   -= dx;
      }
    }
  }
}
//-------------------------------------------------------------------------------------------
void drawTriangle(unsigned x0, unsigned y0, unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

//-------------------------------------------------------------------------------------------
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i, j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  Address_set(x, y, l, y);
  j = l * 2;
  for (i = 1; i <= j; i++)
  {
    Lcd_Write_Data(c);
  }
}
//-------------------------------------------------------------------------------------------
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i, j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  Address_set(x, y, x, l);
  j = l * 2;
  for (i = 1; i <= j; i++)
  {
    Lcd_Write_Data(c);
  }
}
//-------------------------------------------------------------------------------------------
//Draw Rectangle.
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}
//-------------------------------------------------------------------------------------------
//Draw Filled Rectangle.
void Rectf(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
  unsigned int i;
  for (i = 0; i < h; i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y + i, w, c);
  }
}
//-------------------------------------------------------------------------------------------
int RGB(int r, int g, int b)
{
  return r << 16 | g << 8 | b;
}
//-------------------------------------------------------------------------------------------
//Fill Screen with color j
void LCD_Clear(unsigned int j)
{
  unsigned int i, m;
  Address_set(0, 0, 239, 319);
  digitalWrite(LCD_CS, LOW); //Chip Select Active.

  for (i = 0; i < 240; i++)
    for (m = 0; m < 320; m++)
    {
      Lcd_Write_Data(j >> 8);
      Lcd_Write_Data(j);
    }
}
//-------------------------------------------------------------------------------------------
void setup()
{
  for (int p = 2; p < 10; p++)
  {
    pinMode(p, OUTPUT); //Set Pins 2-9 as Output.
  }
  pinMode(A0, OUTPUT); //Set A0 as Output for LCD_RD (Read)
  pinMode(A1, OUTPUT); //Set A1 as Output for LCD_WR (Write)
  pinMode(A2, OUTPUT); //Set A2 as Output for LCD_RS (0=Command/1=Data)
  pinMode(A3, OUTPUT); //Set A3 as Output for LCD_CS (Chip Select, Active LOW)
  pinMode(A4, OUTPUT); //Set A4 as Output for LCD_RST (Soft Reset, Active LOW)
  digitalWrite(A0, HIGH);//Set idle State for LCD_RD.
  digitalWrite(A1, HIGH);//Set Idle State for LCD_WR.
  digitalWrite(A2, HIGH);//Set input as Data for LCD_RS (0=Command/1=Data)
  digitalWrite(A3, HIGH);//Set Idle State for LCD_CS Active LOW.
  digitalWrite(A4, HIGH);//Set Idle State for LCD_RST Soft Reset active LOW.

  Lcd_Init();// Run Init LCD Shield Routine.

  LCD_Clear(BLACK);//Clear Screen to Black.
}
//-------------------------------------------------------------------------------------------
void loop()
{
  //LCD_Clear(BLACK);
  LCD_Clear(WHITE);//Clear Screen to White
  delay(1000);
  invertColor();
  delay(1000);
  normalColor();
  LCD_Clear(WHITE);//Clear Screen to White
  blink(1, 200); //Blink 1 times with 200mS Delay between.
  LCD_Clear(RED);//Clear Screen to Red
  blink(1, 200);
  LCD_Clear(GREEN);//Clear Screen to Green
  blink(1, 200);
  LCD_Clear(BLUE);//Clear Screen to Blue
  blink(1, 200);
  LCD_Clear(YELLOW);//Clear Screen to Yellow
  blink(1, 200);
  LCD_Clear(MAGENTA);//Clear Screen to Magenta
  blink(1, 200);
  LCD_Clear(CYAN);//Clear Screen to Cyan
  blink(1, 200);
  LCD_Clear(BLACK);//Clear Screen to Black
  blink(1, 200);
  LCD_Clear(BLACK);//Clear Screen to Black
  //Draw 50 random size, random position, random color triangles.
  for (int i = 0; i < 50; i++)
  {
    drawTriangle(random(239), random(319), random(239), random(319), random(239), random(319), random(65535)); //drawTriangle(x0,y0,x1,y1,x2,y2,Color);
  }
  delay(2000);
  blink(3, 500);
  //Draw 200 random size, random position, random color Lines.
  LCD_Clear(BLACK);//Clear Screen to Black
  for (int i = 0; i < 200; i++)
  {
    drawLine(random(240), random(320), random(240), random(320), random(65535)); // Lines at x, y, x1, y1, color.
  }
  delay(2000);//Delay of 2 Sec.
  blink(3, 500); //Blink 5 times with 100mS Delay between.
  LCD_Clear(BLACK);//Clear Screen to Black
  //Draw 10000 random position and random color dots.
  for (int i = 0; i < 10000; i++)
  {
    drawPixel(random(239), random(319), random(65535)); // Pixels at x, y, color.
  }
  delay(2000);//Delay of 2 Sec.
  blink(3, 500); //Blink 5 times with 100mS Delay between.
  LCD_Clear(BLACK);//Clear Screen to Black
  //Draw 100 random size, random position and random color rectangles.
  for (int i = 0; i < 100; i++)
  {
    Rect(random(120), random(160), random(120), random(160), random(65535)); // rectangle at x, y, width, hight, color.
  }
  delay(2000);//Delay of 2 Sec.
  blink(3, 500); //Blink 5 times with 100mS Delay between.
  LCD_Clear(BLACK);//Clear Screen to Black
  //Draw 100 random size, random color circles about the x, y, position, Center of screen in this example.
  for (int i = 0; i < 100; i++)
  {
    drawCircle(120, 160, random(120), random(65535)); // circles at x, y, radius, color.
  }
  delay(2000);//Delay of 2 Sec.
  blink(3, 500); //Blink 5 times with 100mS Delay between.
  LCD_Clear(BLACK);//Clear Screen to Black
  //Draw 30 random size, random position and random color filled rectangles.
  for (int i = 0; i < 30; i++)
  {
    Rectf(random(120), random(160), random(120), random(160), random(65535)); // Filled Rectangle at x, y, width, hight, color.
  }
  delay(2000);//Delay of 2 Sec.
  blink(3, 500); //Blink 5 times with 100mS Delay between.
  LCD_Clear(BLACK);//Clear Screen to Black
  //while(true);//Stop Sketch Here if required.
}
//-------------------------------------------------------------------------------------------
//End of Sketch.
