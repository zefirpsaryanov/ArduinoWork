// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Replace with your network details
const char* ssid = "magicbox";
const char* password = "013423944";

//time setup time 
WiFiUDP ntpUDP;
int16_t utc = +3; //UTC +3:00 Sofia/Bulgaria
NTPClient timeClient(ntpUDP, "ntp.comnet.bg", utc*3600, 60000);
 
// Web Server on port 80
WiFiServer server(80);

// DHT Sensor
const int DHTPin = D5;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Initialize Air Quality sensor.
int airquality = 0;

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

Adafruit_BMP085 bmp;


// only runs once on boot
void setup() {
 
  pinMode(LED_BUILTIN, OUTPUT);
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  timeClient.begin();
  dht.begin();
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
   
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
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(5000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

// runs over and over again
void loop() {
  
   // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
       if (client.available()) {
       char c = client.read();
        
        if (c == '\n' && blank_line) {
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            float h = dht.readHumidity();                       // Read temperature as Celsius (the default)
            float t = dht.readTemperature();                    // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);                // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println("Failed to read from DHT sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              airquality = analogRead(A0);    //Read air quality variable
              timeClient.update();
  
              digitalWrite(LED_BUILTIN, LOW);     // turn the LED on (HIGH is the voltage level)
              delay(100);                         
              digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
              delay(3000);  
              
              
              // You can delete the following Serial.print's, it's just for debugging purposes
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.print(" *F");
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.print(" *F");
              Serial.print(" Air Quality = ");
              Serial.print(airquality);
              Serial.print(" *PPM");
              Serial.println();
              Serial.println(timeClient.getFormattedTime());
              
              Serial.print("Temperature = ");
              Serial.print(bmp.readTemperature());
              Serial.println(" *C");
    
              Serial.print("Pressure = ");
              Serial.print(bmp.readPressure());
              Serial.println(" Pa");
    
              // Calculate altitude assuming 'standard' barometric
              // pressure of 1013.25 millibar = 101325 Pascal
    
              Serial.print("Altitude = ");
              Serial.print(bmp.readAltitude());
              Serial.println(" meters");

              Serial.print("Pressure at sealevel (calculated) = ");
              Serial.print(bmp.readSealevelPressure());
              Serial.println(" Pa");

              // you can get a more precise measurement of altitude
              // if you know the current sea level pressure which will
              // vary with weather and such. If it is 1015 millibars
              // that is equal to 101500 Pascals.
    
              Serial.print("Real altitude = ");
              Serial.print(bmp.readAltitude(101500));
              Serial.println(" meters");
              delay(200);
              
              
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature and humidity
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<meta http-equiv=\"refresh\" content=\"30\"></head>");
            client.println("<head></head><body><h1>ESP8266 Web Service</h1>");

            /*          
            client.println("</h3><h3>Temp in Celsius: ");
            if(atoi(celsiusTemp)>=30)
            {
            client.println("<div style=\"color: #b22222;\">"); //red
            }
            else if(atoi(celsiusTemp)<30 && atoi(celsiusTemp)>=12)
            {
            client.println("<div style=\"color: #228b22;\">"); //green
            }
            else if(atoi(celsiusTemp)<15)
            {
            client.println("<div style=\"color: #1e90ff;\">"); //blue
            }
            client.println(celsiusTemp);
            client.println(" * C</h3><h3>");


            client.println("</h3><h3>Temp in Fahrenheit: ");
            if(atoi(fahrenheitTemp)>=86)
            {
            client.println("<div style=\"color: #b22222;\">"); //red
            }
            else if(atoi(fahrenheitTemp)<86 && atoi(fahrenheitTemp)>=54)
            {
            client.println("<div style=\"color: #228b22;\">"); //green
            }
            else if(atoi(fahrenheitTemp)<54)
            {
            client.println("<div style=\"color: #1e90ff;\">"); //blue
            }       
            client.println(fahrenheitTemp);
            client.println(" * F</h3><h3>");

            
            client.println("</h3><h3>Humidity: ");
            if(atoi(humidityTemp)>=85)
            {
            client.println("<div style=\"color: #b22222;\">"); //red
            }
            else if(atoi(humidityTemp)<85 && atoi(humidityTemp)>=35)
            {
            client.println("<div style=\"color: #228b22;\">"); //green
            }
            else if(atoi(humidityTemp)<35)
            {
            client.println("<div style=\"color: #1e90ff;\">"); //blue
            }       
            client.println(humidityTemp);
            client.println("%</h3><h3>");


            /*          
            client.println("</h3><h3>AirQuality: ");
            if(atoi(airquality)>=151)
            {
            client.println("<div style=\"color: #b22222;\">"); //red
            }
            else if(atoi(airquality)<150 && atoi(airquality)>=90)
            {
            client.println("<div style=\"color: #228b22;\">"); //green
            }
            else if(atoi(airquality)<90)
            {
            client.println("<div style=\"color: #1e90ff;\">"); //blue
            }       
            client.println(airquality);
            client.println("*PPM</h3><h3>");
            */            

                     
            client.println("</h3><h3>Temp in Celsius:");
            client.println(celsiusTemp);
            client.println(" * C</h3><h3>");
            
            client.println("</h3><h3>Temp in Fahrenheit: ");
            client.println(fahrenheitTemp);
            client.println(" * F</h3><h3>");
            
            client.println("</h3><h3>Humidity: ");
            client.println(humidityTemp);
            client.println(" %</h3><h3>");
                        
            client.println("</h3><h3>AirQuality: ");
            client.println(airquality);
            client.println("* PPM</h3><h3>");
            
            client.println("</h3><h3>Time: ");
            client.println(timeClient.getFormattedTime());
            client.println("</h3><h3>");

            client.println("</h3><h3>BMP180 Digital Barometric,Pressure,Temperature Sensor ");
            
            client.println("</h3><h3>Temp in Celsius: ");
            client.println(bmp.readTemperature());
            client.println(" * C</h3><h3>");
            
            client.println("</h3><h3>Pressure: ");
            client.println(bmp.readPressure()/100);
            client.println(" hPa</h3><h3>");
            
            client.println("</h3><h3>Altitude: ");
            client.println(bmp.readAltitude());
            client.println(" m</h3><h3>");
            
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}   
