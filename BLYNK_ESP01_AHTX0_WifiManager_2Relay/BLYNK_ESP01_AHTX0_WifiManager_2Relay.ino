#define BLYNK_PRINT Serial
#include <Arduino.h>
#include <Wire.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

#define SCL   2 // ESP01 // 5 // 8266
#define SDA   0 // ESP01 // 4 // 8266  
#define R1Pin 1 // ESP01 // 1 // 8266
#define R2Pin 3 // ESP01 // 3 // 8266

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"

//change this !!!!!!!!!!!!!!!!
#define tempVPin  V51
#define HumVPin   V52
#define startTemp V53
#define startHum  V54
#define interval  V55
#define LedPin1   V56
#define LedPin2   V57
//change this !!!!!!!!!!!!!!!!

float aht20_Temperature;
float aht20_Humidity;

int startAHT_temp, stopAHT_temp, startAHT_Hum, stopAHT_Hum;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned long period = 10000;  //the value is a number of milliseconds

char auth[] = "3h2E6uN9NXc54o_6lNlln_D5MRs9516H";

WidgetLED led1(LedPin1);
WidgetLED led2(LedPin2);

WiFiManager wifiManager;
sensors_event_t humidity, temp;

void setup(void)
{
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  aht.begin();

  WiFi.mode(WIFI_STA);
  wifiManager.autoConnect();

  if (wifiManager.autoConnect() != false) // reset if last wifi is not found :)
  {
    WiFi.hostname("AHTX0_2R_5");
    Blynk.config(auth, "blynk.techbg.net", 18080); // oop@techbg.net /weather mVaRSybNBd7HBaXqsGP3YsgijTXJ-vkY /relays 3h2E6uN9NXc54o_6lNlln_D5MRs9516H
    //Blynk.config(auth, "psaryanov.org", 18080);
    //Blynk.config(auth, IPAddress(192, 168, 10, 10), 8080);

    Blynk.connect();
  }
  else
    wifiManager.resetSettings(); // resetSettings

  pinMode(R1Pin, OUTPUT);
  pinMode(R2Pin, OUTPUT);

  led1.on(); // blynk led1 ON
  led2.on(); // blynk led2 ON
}

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

void loop(void)
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    //startAHT_temp, stopAHT_temp , aht20_Temperature , led1;

    if (aht20_Temperature >= startAHT_temp )
    {
      digitalWrite(R1Pin, LOW);
      led1.setColor(BLYNK_GREEN);
    }
    if (aht20_Temperature < startAHT_temp )
    {
      digitalWrite(R1Pin, HIGH);
      led1.setColor(BLYNK_RED);
    }

    //startAHT_Hum, stopAHT_Hum , aht20_Humidity , led2;

    if (aht20_Humidity >= startAHT_Hum )
    {
      digitalWrite(R2Pin, LOW);
      led2.setColor(BLYNK_GREEN);
    }
    if (aht20_Humidity < startAHT_Hum )
    {
      digitalWrite(R2Pin, HIGH);
      led2.setColor(BLYNK_RED);
    }
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }

  // restart 1h !!!!
  // if (currentMillis >= 7200000) ESP.restart();

  aht.getEvent(&humidity, &temp);
  aht20_Temperature = temp.temperature - 1.5;
  aht20_Humidity = humidity.relative_humidity + 0.5;

  Blynk.run();
  Blynk.virtualWrite(tempVPin, aht20_Temperature);  //Blynk V1 is for Temperature
  Blynk.virtualWrite(HumVPin, aht20_Humidity);     //Blynk V2 is for Humidity
  delay(100);
}

BLYNK_WRITE(startTemp)
{
  startAHT_temp = param.asInt();
}
BLYNK_WRITE(startHum)
{
  startAHT_Hum = param.asInt();
}
BLYNK_WRITE(interval)
{
  period = param.asInt() * 1000;
}
