#include <Pinger.h>
#include <ESP8266WiFi.h>

Pinger pinger;

void setup() {

  Serial.begin(9600);
  bool stationConnected = WiFi.begin("O2", "1q2w3e4r");
  Serial.print("Connecting to AP...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Ok\n");
}

void loop() {
  pinger.Ping(IPAddress(192, 168, 10, 141), 1);
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

  delay(1000);
}