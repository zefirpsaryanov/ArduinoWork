#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <U8g2lib.h>
#include <Adafruit_AHTX0.h>

#define RelayPin 14 // D5
#define key1 13     // D6

Adafruit_AHTX0 aht;

U8G2_SH1107_SEEED_128X128_F_HW_I2C u8g2(U8G2_R1, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);



const char* host = "esp8266-webupdate";
const char* update_path = "/update";
const char* update_username = "admin";
const char* update_password = "admin";
const char* ssid = "test";
const char* password = "1q2w3e4r";

int analogMin = 10;
int analogMax = 920;
int analogVal ;
int potentiometer_temp ;
double aht_Temperature;
double aht_Humidity;

int rssi;
int rssiMin = -85; // define minimum strength of signal in dBm
int rssiMax = -25; // define maximum strength of signal in dBm
int alarm_stat = 0;


unsigned long startMillis;
unsigned long currentMillis;
unsigned long period = 5000; //10800000; //3h

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid, password);

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.on("/", handle_OnConnect);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

  startMillis = millis();   //initial start time
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  pinMode(key1, INPUT_PULLUP); // set pin  as input

  aht.begin();
  u8g2.begin();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the led off
  digitalWrite(RelayPin, LOW);      // turn relay off
}

void handle_OnConnect()
{
  httpServer.send(200, "text/html", SendHTML(aht_Temperature, aht_Humidity));
}

String SendHTML(float v1, float v2)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta http-equiv=\"refresh\" content=\"60\">\n";
  ptr += "<meta name=\"author\" content=\"Zefir\">\n";
  ptr += "<title>temperature & humidity report</title>\n";
  ptr += "<style>html { font-family: Cursive; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>temperature & humidity</h1>\n";

  ptr += "<p>temperature ";
  ptr += v1;
  ptr += " *C</p>";

  ptr += "<p>alarm_temp: ";
  ptr += potentiometer_temp;
  ptr += " *C</p>";

  ptr += "<p>humidity: ";
  ptr += v2;
  ptr += " %H</p>";

  ptr += "<p>wifi: ";
  ptr += rssi;
  ptr += " % signal</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void loop(void)
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  aht_Temperature = temp.temperature - 0.5;
  aht_Humidity = humidity.relative_humidity;
  analogVal = constrain(analogRead(A0), analogMin, analogMax);  // limits range of sensor values to between analogMin and analogMax
  potentiometer_temp = map(analogVal, analogMin, analogMax, -25, 35);  //analogVal to %

  if (alarm_stat == 1) alarma();

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)

  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    if (aht_Temperature >= potentiometer_temp)
    {
      alarm_stat = 1;
    }
    startMillis = currentMillis;  //save the start time
  }

  int key1S = digitalRead(key1); // read if key1 is pressed

  if (!key1S) ESP.restart();

  httpServer.handleClient();
  MDNS.update();
  rssi = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100); //rssi to %
  oled_print();

  
  if (millis() / 3600000  > 72) ESP.restart();
}

void alarma()
{
  digitalWrite(RelayPin, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  digitalWrite(RelayPin, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
}

void oled_print()
{
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_logisoso26_tf);

  u8g2.setCursor(0, 64); //0,32 ; // 0,64
  u8g2.print((int)potentiometer_temp);

  u8g2.setCursor(0, 96); //0,64 ; //0, 96
  u8g2.print(aht_Temperature, 1);

  u8g2.setCursor(80, 64); //80,32  ; // 80, 64
  u8g2.print((int)aht_Humidity);

  u8g2.setCursor(80, 96); //80,64 ; // 80, 96
  u8g2.print(rssi);

  u8g2.sendBuffer();
}
