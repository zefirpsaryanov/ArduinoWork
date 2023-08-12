#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <U8x8lib.h>

char ssid[] = "test";
char pass[] = "1q2w3e4r";


const int SENSOR_PIN = 13;  // Arduino pin D7 connected to DS18B20 sensor's DQ pin

ESP8266WebServer server(80);
OneWire oneWire(SENSOR_PIN);             // setup a oneWire instance
DallasTemperature tempSensor(&oneWire);  // pass oneWire to DallasTemperature library
U8X8_SSD1306_64X48_ER_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE);

int tempCelsius;  // temperature in Celsius

char bffrTemp[4];
char bffrAlarm[4];

int alarma;

IPAddress local_IP(192, 168, 10, 190);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  WiFi.config(local_IP, gateway, subnet);
  tempSensor.begin();  // initialize the sensor
  u8x8.begin();

  server.begin();
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
}

void loop() {
  tempSensor.requestTemperatures();             // send the command to get temperatures
  tempCelsius = tempSensor.getTempCByIndex(0);  // read temperature in Celsius

  int alarmTemp = map(analogRead(0), 0, 1024, -25, 40);

  Serial.print("Temperature: ");
  Serial.print(tempCelsius);  // print the temperature in Celsius
  Serial.println(" °C");
  tempCelsius > alarmTemp ? alarma = 3 : alarma = 5;

  u8x8.setCursor(2, 2);
  u8x8.setFont(u8x8_font_8x13B_1x2_n);
  sprintf(bffrTemp, "%03d", tempCelsius);
  u8x8.printf(bffrTemp);

  u8x8.setCursor(2, 4);
  sprintf(bffrAlarm, "%03d", alarmTemp);
  u8x8.printf(bffrAlarm);

  server.handleClient();

  delay(100);
}
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(alarma));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(int a) {
  String ptr = "<!DOCTYPE html><html>";
  ptr += a;
  ptr += "</html>";
  return ptr;
}