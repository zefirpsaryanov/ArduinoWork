#include <Arduino.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>


#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font

#include <BlynkSimpleEsp8266.h>

#include <Adafruit_BME280.h>


#include <IRremoteESP8266.h>
#include <IRsend.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define BLYNK_PRINT Serial

char auth[] = "1a589190cd0e42caa727ea338ed16790";

Adafruit_SSD1306 display32(128, 32);  // Create display 128*32
Adafruit_SSD1306 display64(128, 64);  // Create display 128*64

Adafruit_BME280 bme;

const uint16_t kIrLed = 3;
IRsend irsend(kIrLed);

const char ssid[] = "test";
const char pass[] = "1q2w3e4r";

static const char ntpServerName[] = "ntp.comnet.bg";
const int timeZone = 2;
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
char timeToString[20];
char dateToString[20];
String dayofweek;

float h , t , p, a = 0;

void setup()  // Start of setup
{
  Serial.begin(9600);
  Wire.pins(0, 2);
  bme.begin(0x76);
  pinMode(3, OUTPUT);

  irsend.begin();

  /*-------- display start--------*/
  display32.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize display with the I2C address of 0x3C , Create display 128*32
  display64.begin(SSD1306_SWITCHCAPVCC, 0x3D); // Initialize display with the I2C address of 0x3D , Create display 128*64

  display32.clearDisplay();  // Clear the buffer display 128*32
  display64.clearDisplay();  // Clear the buffer display 128*64

  display32.setTextColor(WHITE);  // Set color of the text
  display64.setTextColor(WHITE);  // Set color of the text

  display32.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display64.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3

  display32.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
  display64.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.

  display32.dim(1);  //Set brightness (0 is maximun and 1 is a little dim)
  display64.dim(1);  //Set brightness (0 is maximun and 1 is a little dim)

  display32.setFont(&FreeMono9pt7b);  // Set a custom font
  display64.setFont(&FreeMono9pt7b);  // Set a custom font

  display32.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of
  display64.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of

  display32.display();  // Print everything we set previously
  display64.display();  // Print everything we set previously

  /*-------- display end--------*/

  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
  }

  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()  // Start of loop
{

  // Convert Variable1 into a string, so we can change the text alignment to the right:
  // It can be also used to add or remove decimal numbers.
  // char string[10];  // Create a character array of 10 characters
  // Convert float to a string:
  // dtostrf(Variable1, 3, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)

  display32.clearDisplay();  // Clear the display so we can refresh
  display64.clearDisplay();  // Clear the display so we can refresh

  display32.setCursor(0, 10);  // (x,y)
  display32.println(t);  // Text or value to print

  display32.setCursor(0, 25);  // (x,y)
  display32.println(h);  // Text or value to

  display32.setCursor(60, 10);  // (x,y)
  display32.println(p);  // Text or value to print

  display64.setCursor(0, 10);  // (x,y)
  display64.println(timeToString); // Text or value to print

  display64.setCursor(0, 25);  // (x,y)
  display64.println(dateToString); // Text or value to print

  display64.setCursor(0, 40);  // (x,y)
  display64.println(dayofweek); // Text or value to print

  if (timeStatus() != timeNotSet)
  {
    if (now() != prevDisplay)
    { //update the display only if time has changed
      prevDisplay = now();
      sprintfData();
    }
  }

  Blynk.run();

  h = bme.readHumidity();
  t = bme.readTemperature() - 1;
  p = bme.readPressure() / 100.0F;

  Blynk.virtualWrite(V20, h);
  Blynk.virtualWrite(V21, t);
  Blynk.virtualWrite(V22, p);

  Blynk.virtualWrite(V23, timeToString);
  Blynk.virtualWrite(V24, dateToString);
  Blynk.virtualWrite(V25, dayofweek);

  display32.display();  // Print everything we set previously
  display64.display();  // Print everything we set previously
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void sprintfData()
{
  sprintf(timeToString, "%02d:%02d:%02d" , hour() + 1, minute(), second());
  sprintf(dateToString, "%02d.%02d.%d", day(), month(), year());
  if (weekday() == 1) dayofweek = "Sunday";
  if (weekday() == 2) dayofweek = "Monday";
  if (weekday() == 3) dayofweek = "Tuesday";
  if (weekday() == 4) dayofweek = "Wednesday";
  if (weekday() == 5) dayofweek = "Thursday";
  if (weekday() == 6) dayofweek = "Friday";
  if (weekday() == 7) dayofweek = "Saturday";
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendNEC(0x2FD48B7, 32); // NEC: 2FD48B7 - TOSHIBA POWER
  }
}
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x38D); // MAG HOME BUTTON
  }
}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x38F); // MAG BACK BUTTON
  }
}
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x39D); // MAG INFO BUTTON
  }
}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x38A); // MAG MENU BUTTON
  }
}
BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BF); // MAG LEFT BUTTON
  }
}
BLYNK_WRITE(V6)
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AB); // MAG RIGH BUTTON
  }
}
BLYNK_WRITE(V7)
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BD); // MAG UPUP BUTTON
  }
}
BLYNK_WRITE(V8)
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BE); // MAG DOWN BUTTON
  }
}
BLYNK_WRITE(V9)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AC); // MAG OKOK BUTTON
  }
}

BLYNK_WRITE(V10)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AC); // MAG OKOK BUTTON
  }
}

BLYNK_WRITE(V11)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3AC); // MAG OKOK BUTTON
  }
}

BLYNK_WRITE(V12)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x392); // MAG VOL+ BUTTON
  }
}

BLYNK_WRITE(V13)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x393); // MAG VOL- BUTTON
  }
}

BLYNK_WRITE(V14)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x3BC); // MAG CHA+ BUTTON
  }
}

BLYNK_WRITE(V15)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x391); // MAG CHA- BUTTON
  }
}

BLYNK_WRITE(V16)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendRC5(0x383); // MAG 3 BUTTON
  }
}


/*
  NEC: 2FD48B7 - TOSHIBA POWER
  RC5: B8C , 38C  - MAG POWE BUTTON

  RC5: BB0 , 3B0  - MAG MUTE BUTTON
  RC5: B8D , 38D  - MAG HOME BUTTON
  RC5: B8F , 38F  - MAG BACK BUTTON
  RC5: B9D , 39D  - MAG INFO BUTTON
  RC5: B8A , 38A  - MAG MENU BUTTON
  RC5: B8B , 38B  - MAG PLAY BUTTON
  RC5: B99 , 399  - MAG STOP BUTTON
  RC5: BBF , 3BF  - MAG LEFT BUTTON
  RC5: BAB , 3AB  - MAG RIGH BUTTON
  RC5: BBD , 3BD  - MAG UPUP BUTTON
  RC5: BBE , 3BE  - MAG DOWN BUTTON
  RC5: BAC , 3AC  - MAG OKOK BUTTON
  RC5: B92 , 392  - MAG VOL+ BUTTON
  RC5: B93 , 393  - MAG VOL- BUTTON
  RC5: BBC , 3BC  - MAG CHA+ BUTTON
  RC5: B91 , 391  - MAG CHA- BUTTON
  RC5: B80 , 380  - MAG 0 BUTTON
  RC5: B81 , 381  - MAG 1 BUTTON
  RC5: B82 , 382  - MAG 2 BUTTON
  RC5: B83 , 383  - MAG 3 BUTTON
  RC5: B84 , 384  - MAG 4 BUTTON
  RC5: B85 , 385  - MAG 5 BUTTON
  RC5: B86 , 386  - MAG 6 BUTTON
  RC5: B87 , 387  - MAG 7 BUTTON
  RC5: B88 , 388  - MAG 8 BUTTON
  RC5: B89 , 389  - MAG 9 BUTTON
*/



/*
  List of fonts that support right alignment:
  FreeMono9pt7b.h
  FreeMono12pt7b.h
  FreeMono18pt7b.h
  FreeMono24pt7b.h
  FreeMonoBold9pt7b.h
  FreeMonoBold12pt7b.h
  FreeMonoBold18pt7b.h
  FreeMonoBold24pt7b.h
  FreeMonoBoldOblique9pt7b.h
  FreeMonoBoldOblique12pt7b.h
  FreeMonoBoldOblique18pt7b.h
  FreeMonoBoldOblique24pt7b.h
  FreeMonoOblique9pt7b.h
  FreeMonoOblique12pt7b.h
  FreeMonoOblique18pt7b.h
  FreeMonoOblique24pt7b.h
*/
