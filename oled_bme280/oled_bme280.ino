#include <Adafruit_BME280.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSerif9pt7b.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);
Adafruit_BME280 bme;

void setup() 
{
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.setFont(&FreeSerif9pt7b);
  display.display();
  delay(2000);
  display.clearDisplay();
  if (!bme.begin(0x76)) 
  {
          Serial.println("Could not find a valid BME280 sensor, check wiring!");
          while (1);
        }
}

void loop() 
{
  display.clearDisplay();
  
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature()); //prints in *C
  //Serial.print(bme.readTemperature() * 9 / 5 + 32); //prints in *F
  Serial.println("*C"); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,15);
  display.print("Temp:");
  display.print((int)bme.readTemperature()); //prints in *C
  //display.print(bme.readTemperature() * 9 / 5 + 32); //prints in *F
  display.println("*C");
  display.display();
  

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure()/100.0F);
  Serial.println("hPa");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Press:");
  display.print(bme.readPressure()/100.0F);
  display.println("Pa");
  display.display();
  
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println("%");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Hum:");
  display.print((int)bme.readHumidity());
  display.println("%");
  display.display();
  Serial.println();
  delay(1000);
}
