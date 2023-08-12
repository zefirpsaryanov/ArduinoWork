#include "ELMduino.h"
#include "BluetoothSerial.h"

BluetoothSerial BTSerial;

uint8_t BTaddress[6] = { 0x84, 0xEB, 0x18, 0x65, 0x1C, 0x4D };  // linker
const char *pin = "1234";

ELM327 myELM327;
float VOLTAGE = 0;
float fuelRate = 0;

void setup() 
{
  Serial.begin(115200);
  BTSerial.begin("ESP32OBD", true);
  BTSerial.setPin(pin);
  BTSerial.connect(BTaddress);
  myELM327.begin(BTSerial);
}

void loop() 
{
  readVolts();
  readFuelRate();
}

void obdData() 
{
  double volts = myELM327.batteryVoltage();
  double tmpFuelRate = myELM327.fuelRate();

  if (myELM327.nb_rx_state == ELM_SUCCESS) 
  {
    VOLTAGE = volts;
    fuelRate = tmpFuelRate;
    
    Serial.print("volt: ");
    Serial.print(VOLTAGE);

    Serial.print(" fuelRate: ");
    Serial.println(fuelRate);
  }
}