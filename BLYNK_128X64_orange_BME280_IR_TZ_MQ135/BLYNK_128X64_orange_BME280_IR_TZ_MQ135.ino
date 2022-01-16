#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <ezTime.h>

#include <U8g2lib.h>
U8G2_ST7567_ENH_DG128064I_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* cs=*/ 2, /* dc=*/ 0, /* reset=*/ -1);

#include <BlynkSimpleEsp8266.h>

#include <Adafruit_BME280.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define BLYNK_PRINT Serial
Adafruit_BME280 bme;
char auth[] = "1a589190cd0e42caa727ea338ed16790";

const uint16_t kIrLed = 12; //D6 IR Pin
IRsend irsend(kIrLed);

const char ssid[] = "test";
const char pass[] = "1q2w3e4r";

Timezone myTZ;
char timeToString[20];
char dateToString[20];
String dayofweek;

float h , t , p = 0;

int LCD_PIN = 15;
int DUTY = 48;

#include <MQUnifiedsensor.h>
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  

MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

float CO , Alcohol , CO2 , Tolueno , NH4 , Acetona;
int gasLevel = 0;
String quality = "";

void setup()
{
  Serial.begin(9600);
  pinMode( LCD_PIN, OUTPUT );
  analogWrite( LCD_PIN, DUTY );

  bme.begin(0x76);

  pinMode(12, OUTPUT);
  irsend.begin();

  u8g2.begin();
  u8g2.setContrast(160);

  WiFi.hostname("WemosMini-indor");
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 10, 10), 8080);

  waitForSync();
  myTZ.setLocation(F("Europe/Sofia"));

  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.init();
  float calcR0 = 0;
  for (int i = 1; i <= 10; i ++)
  {
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(". ");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("done!");
}

void loop()  // Start of loop
{
  t = bme.readTemperature() - 1;
  h = bme.readHumidity() - 5;
  p = bme.readPressure() / 100.0F;

  MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
  CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(77.255); MQ135.setB(-3.18); // Configurate the ecuation values to get Alcohol concentration
  Alcohol = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
  CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(44.947); MQ135.setB(-3.445); // Configurate the ecuation values to get Tolueno concentration
  Tolueno = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
  NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(34.668); MQ135.setB(-3.369); // Configurate the ecuation values to get Acetona concentration
  Acetona = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  sprintfData();
  air_sensor();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x13B_tf);

  u8g2.setCursor(0, 9);  // (x,y)
  u8g2.println(timeToString); // Text or value to print

  u8g2.setCursor(0, 20);  // (x,y)
  u8g2.println(dateToString); // Text or value to print

  u8g2.setCursor(0, 31);  // (x,y)
  u8g2.println(dayofweek); // Text or value to print

  u8g2.setCursor(0, 42);  // (x,y)
  u8g2.println("CO  "); // Text or value to print
  u8g2.println(CO); // Text or value to print

  u8g2.setCursor(0, 53);  // (x,y)
  u8g2.println("Alc "); // Text or value to print
  u8g2.println(Alcohol); // Text or value to print

  u8g2.setCursor(0, 64);  // (x,y)
  u8g2.println("CO2 "); // Text or value to print
  u8g2.println(CO2); // Text or value to print


  u8g2.setCursor(80, 9);  // (x,y)
  u8g2.println(t);  // Text or value to print

  u8g2.setCursor(80, 20);  // (x,y)
  u8g2.println(h);  // Text or value to

  u8g2.setCursor(80, 31);  // (x,y)
  u8g2.println(p);  // Text or value to print

  u8g2.setCursor(73, 42);  // (x,y)
  u8g2.println("Tol ");
  u8g2.println(Tolueno);

  u8g2.setCursor(73, 53);  // (x,y)
  u8g2.println("NH4 ");  // Text or value to
  u8g2.println(NH4);  // Text or value to

  u8g2.setCursor(73, 64);  // (x,y)
  u8g2.println("Ace ");  // Text or value to print
  u8g2.println(Acetona);  // Text or value to print
  u8g2.sendBuffer();


  Blynk.run();

  Blynk.virtualWrite(V20, h);
  Blynk.virtualWrite(V21, t);
  Blynk.virtualWrite(V22, p);

  Blynk.virtualWrite(V23, timeToString);
  Blynk.virtualWrite(V24, dateToString);
  Blynk.virtualWrite(V25, dayofweek);

  Blynk.virtualWrite(V28, CO);
  Blynk.virtualWrite(V29, Alcohol);
  Blynk.virtualWrite(V30, CO2);
  Blynk.virtualWrite(V31, NH4);
  Blynk.virtualWrite(V32, Acetona);
  Blynk.virtualWrite(V33, Tolueno);
  Blynk.virtualWrite(V34, quality);
}

void sprintfData()
{
  sprintf(timeToString, "%02d:%02d:%02d" , myTZ.hour() , myTZ.minute(), myTZ.second());
  sprintf(dateToString, "%02d.%02d.%d", myTZ.day(), myTZ.month(), myTZ.year());
  if (myTZ.weekday() == 1) dayofweek = "Sunday";
  if (myTZ.weekday() == 2) dayofweek = "Monday";
  if (myTZ.weekday() == 3) dayofweek = "Tuesday";
  if (myTZ.weekday() == 4) dayofweek = "Wednesday";
  if (myTZ.weekday() == 5) dayofweek = "Thursday";
  if (myTZ.weekday() == 6) dayofweek = "Friday";
  if (myTZ.weekday() == 7) dayofweek = "Saturday";
}

void air_sensor()
{
  gasLevel = analogRead(pin);

  if (gasLevel < 181) quality = "Good";

  else if (gasLevel > 181 && gasLevel < 225) quality = "Poor";

  else if (gasLevel > 225 && gasLevel < 300) quality = "Bad";

  else if (gasLevel > 300 && gasLevel < 350) quality = "Dead";

  else quality = "Toxic";
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if (pinValue == 1)
  {
    irsend.sendNEC(0x2FD48B7); // NEC: 2FD48B7 - TOSHIBA POWER
    irsend.sendNEC(0xFFFFFFFFFFFFFFFF); // NEC: 2FD48B7 - TOSHIBA POWER
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
