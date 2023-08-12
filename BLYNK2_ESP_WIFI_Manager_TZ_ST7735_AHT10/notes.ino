/*

  Information notes only:
  ======================

  //These are the text plotting alignment (reference datum point)

  TL_DATUM = Top left (default)
  TC_DATUM = Top centre
  TR_DATUM = Top right

  ML_DATUM = Middle left
  MC_DATUM = Middle centre
  MR_DATUM = Middle right

  BL_DATUM = Bottom left
  BC_DATUM = Bottom centre
  BR_DATUM = Bottom right

  tft.setTextDatum(TL_DATUM); // Top Centre datum

  L_BASELINE = Left character baseline (Line the 'A' character would sit on)
  C_BASELINE = Centre character baseline
  R_BASELINE = Right character baseline

  // Basic colours already defined:

  TFT_BLACK       0x0000
  TFT_NAVY        0x000F
  TFT_DARKGREEN   0x03E0
  TFT_DARKCYAN    0x03EF
  TFT_MAROON      0x7800
  TFT_PURPLE      0x780F
  TFT_OLIVE       0x7BE0
  TFT_LIGHTGREY   0xC618
  TFT_DARKGREY    0x7BEF
  TFT_BLUE        0x001F
  TFT_GREEN       0x07E0
  TFT_CYAN        0x07FF
  TFT_RED         0xF800
  TFT_MAGENTA     0xF81F
  TFT_YELLOW      0xFFE0
  TFT_WHITE       0xFFFF
  TFT_ORANGE      0xFDA0
  TFT_GREENYELLOW 0xB7E0
  TFT_PINK        0xFC9F




  Less than -50dB (-40, -30 and -20) = 100% of signal strength
  From -51 to -55dB= 90%
  From -56 to -62dB=80%
  From -63 to -65dB=75%
  The below is not good enough for Apple devices

  From -66 to 68dB=70%
  From -69 to 74dB= 60%
  From -75 to 79dB= 50%
  From -80 to -83dB=30%
  Windows laptops can work fine on -80dB however with slower speeds


void DSTTime()
{
  if (month() <= 3 && month() <= 10 && day() > 26) hourDST = hour() + 1;
  else hourDST = hour();
}
*/
