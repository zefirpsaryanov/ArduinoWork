#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#define SSD1306_LCDWIDTH                  64
#define SSD1306_LCDHEIGHT                 48

int i;
static int moi = 0;

// Replace with your network details
const char* ssid = "magicbox";
const char* password = "013423944";

//time setup time
WiFiUDP ntpUDP;
int16_t utc = +2; //UTC +3:00 Sofia/Bulgaria
NTPClient timeClient(ntpUDP, "ntp.comnet.bg", utc * 3600, 60000);

void setup(void) {
  Wire.begin();
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen

  Serial.begin(115200);
  delay(10);
  timeClient.begin();


  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


  // Printing the ESP IP address
  Serial.println(WiFi.localIP());


}

void loop(void) {
  moi = analogRead(0);
  timeClient.update();
  oled.setTextXY(2, 4);            // Set cursor position, line 2 10th character
  oled.putString(timeClient.getFormattedTime());
  oled.setTextXY(3, 4);            // Set cursor position, line 2 10th character
  oled.putNumber(moi);
  oled.setTextXY(4, 4);            // Set cursor position, line 2 10th character
  oled.putString("Moisture");

  delay(1000);

  Serial.println(timeClient.getFormattedTime());



}
