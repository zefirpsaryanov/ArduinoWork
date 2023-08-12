/*
  To upload through terminal you can use: curl -u admin:admin -F "image=@firmware.bin" esp8266-webupdate.local/firmware
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ezTime.h>
#include <ESP8266mDNS.h>
#include <Pinger.h>

const char* host = "bc_rain_o2";
const char* update_path = "/update";
const char* update_username = "admin";
const char* update_password = "admin";
const char* ssid = "O2";
const char* password = "1q2w3e4r";
String station_name = "BC_Rain_O2";


int rssi0 = 0;
int rssiMin = -85;  // define minimum strength of signal in dBm
int rssiMax = -25;  // define maximum strength of signal in dBm

char timeToStr[8];

Timezone myTZ;

Pinger pinger;

#define relayPin14 14  // relay2
#define relayPin12 12  // relay3
#define relayPin13 13  // relay4

int startHour = 9;
int stopHour = 18;
int tempminutes = 0;

//const unsigned long period = 432000000; // 5 days

int raininterval = 31000;
int mistinterval = 31000;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.print("Booting Sketch ...");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off

  WiFi.hostname(station_name);

  WiFi.begin(ssid, password);


  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  MDNS.begin(host);
  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.on("/", handle_OnConnect);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

  waitForSync();
  myTZ.setLocation(F("Europe/Sofia"));

  rssi0 = map(WiFi.RSSI(), rssiMin, rssiMax, 0, 100);  //rssi to %

  pinMode(relayPin14, OUTPUT);
  pinMode(relayPin12, OUTPUT);
  pinMode(relayPin13, OUTPUT);
  digitalWrite(relayPin14, LOW);
  digitalWrite(relayPin12, LOW);
  digitalWrite(relayPin13, LOW);
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED on
  Serial.println(" Done");
}
void handle_OnConnect() {

  httpServer.send(200, "text/html", SendHTML(startHour, stopHour));
}

String SendHTML(int v1, int v2) {

  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta http-equiv=\"refresh\" content=\"60\">\n";
  ptr += "<meta name=\"author\" content=\"Zefir\">\n";
  ptr += "<title>BC_Rain_O2</title>\n";
  ptr += "<style>html { font-family: Cursive; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>BC_Rain_O2</h1>\n";

  /*
  ptr += "<p>startTime: ";
  ptr += v1;
  ptr += "</p>";

  ptr += "<p>stopTime: ";
  ptr += v2;
  ptr += "</p>";
*/

  ptr += "<p>Time: ";
  ptr += timeToStr;
  ptr += "</p>";

  ptr += "<p>RSSI%: ";
  ptr += rssi0;
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void loop() {

  pinger.Ping(IPAddress(1, 1, 1, 1), 1);
  pinger.OnReceive([](const PingerResponse& response) {
    if (response.ReceivedResponse) {
      Serial.println("host up");
    } else {
      Serial.println("host down - restart");
      ESP.restart();
    }
    // Return true to continue the ping sequence.
    // If current event returns false, the ping sequence is interrupted.
    return true;
  });

  mist_relays();
  //rain_relay();
  sprintf(timeToStr, "%02d.%02d", myTZ.hour(), minute());  //WWW
  httpServer.handleClient();
  MDNS.update();
  delay(1000);
}


void mist_relays() {  // every H from 9-18 for 31 sec

  if (minute() == tempminutes && myTZ.hour() <= stopHour && myTZ.hour() >= startHour) {

    Serial.println("Mist1Start");
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED OFF
    digitalWrite(relayPin14, HIGH);
    delay(mistinterval);
    digitalWrite(relayPin14, LOW);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED ON
    Serial.println("Mist1Stop");

    delay(5000);

    Serial.println("Mist2Start");
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED OFF
    digitalWrite(relayPin12, HIGH);
    delay(mistinterval);
    digitalWrite(relayPin12, LOW);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED ON
    Serial.println("Mist2Stop");
  }
}

void rain_relay() {  // every H from 9-18 for 31 sec

  if (minute() == tempminutes + 2 && myTZ.hour() <= stopHour && myTZ.hour() >= startHour) {

    Serial.println("RainStart");
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED OFF
    digitalWrite(relayPin13, HIGH);
    delay(raininterval);
    digitalWrite(relayPin13, LOW);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED ON
    Serial.println("RainStop");
    delay(raininterval);
  }
}