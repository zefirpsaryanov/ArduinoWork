#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <Adafruit_AHT10.h>

#define SDA 0
#define SCL 2

/* Your WiFi Credentials */
const char* ssid = "test"; // SSID
const char* password = "1q2w3e4r"; // Password

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server);

/*
  Dashboard Cards
  Format - (Dashboard Instance, Card Type, Card Name, Card Symbol(optional) )
*/
Card card_temp(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card card_humi(&dashboard, HUMIDITY_CARD, "Humidity", "%");

Adafruit_AHT10 aht;


void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  aht.begin();

  WiFiManager wifiManager;

  wifiManager.autoConnect();

  if (wifiManager.autoConnect() != false) // reset if last wifi is not found :)
  {
    server.begin();
    WiFi.hostname("ESP01_DASH-AHT10");
  }

  else
    wifiManager.resetSettings(); //connect last known wifi
}

void loop() 
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  card_temp.update(temp.temperature);
  card_humi.update(humidity.relative_humidity);

  dashboard.sendUpdates();

  delay(3000);
}
