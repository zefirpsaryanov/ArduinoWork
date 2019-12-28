#include <Arduino.h>
#include <U8g2lib.h>
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);




U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void draw(void) {
  //  // graphic commands to redraw the complete screen should be placed here
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  u8g2.setFont(u8g2_font_helvB10_tf);
  //u8g2.setFont(u8g2_font_7x14_tf);  
  u8g2.setCursor(2, 16);
  u8g2.print("Temp & Humidity");
  u8g2.setFont(u8g2_font_helvB10_tf);
  u8g2.setCursor(2, 42);
  u8g2.print("T= "); u8g2.print(t); u8g2.print(" *C");
  u8g2.setCursor(2, 64);
  u8g2.print("H= "); u8g2.print(h); u8g2.print(" %");
}

void setup(void) {
  
  dht.begin();

  u8g2.begin();  
}

void loop(void) {

    // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  u8g2.firstPage();
  do {
    draw();
  } while ( u8g2.nextPage() );
  delay(1000);
}

