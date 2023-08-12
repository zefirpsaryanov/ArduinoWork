#include <ESP8266WiFi.h>

#define buzPin 12
#define ledPin 13

WiFiClient client;

const char* ssid = "test";
const char* password = "1q2w3e4r";
char received[30];

IPAddress local_IP(192, 168, 10, 191);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);
const char* host = "192.168.10.190";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buzPin, OUTPUT);
  Serial.print("Connecting to ");

  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.config(local_IP, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(ledPin, HIGH);
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  client.connect(host, httpPort);  // connecting to host
  String url = "/";
  Serial.print("Requesting URL: ");
  Serial.println(url);  // Requesting URL
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  delay(2000);

  // int i = 0;

  while (client.available()) {

    //   received[i] = client.read();
    //   i++;

    //   if (i == 8 && received[7] == '3')  // 8th element
    //   {
    //     Serial.println(received[7]);
    //     digitalWrite(LED_BUILTIN, LOW);
    //     delay(250);
    //     digitalWrite(LED_BUILTIN, HIGH);
    //     delay(250);
    //     digitalWrite(LED_BUILTIN, LOW);
    //     delay(250);
    //     digitalWrite(LED_BUILTIN, HIGH);
    //   }

    //   if (i == 8 && received[7] == '5') Serial.println(received[7]);  // 8th element

    //      else digitalWrite(LED_BUILTIN, HIGH);


    char c = client.read();
    Serial.write(c);

    if (c != '3') {
      digitalWrite(ledPin, LOW);

    } else {
      digitalWrite(ledPin, HIGH);
      tone(buzPin, 200, 200);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
      digitalWrite(ledPin, HIGH);
      tone(buzPin, 200, 200);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
    }
  }
  Serial.println();
  Serial.println("closing connection");
  delay(2000);
}