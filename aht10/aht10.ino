#include <Adafruit_AHT10.h>

Adafruit_AHT10 aht;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit AHT10 demo!");

  if (! aht.begin()) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  Serial.print("\nTemperature: "); 
  Serial.print(temp.temperature); 
  Serial.print(" .C");
  Serial.print(" | Humidity: "); 
  Serial.print(humidity.relative_humidity); 
  Serial.print(" %rH");

  delay(500);
}
