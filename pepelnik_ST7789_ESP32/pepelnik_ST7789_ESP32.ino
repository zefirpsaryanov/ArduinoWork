/*
  This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######       TO SELECT THE FONTS AND PINS YOU USE, SEE ABOVE       ######
  #########################################################################
*/

#include <TFT_eSPI.h>

#include "Fonts\FreeSansBold12pt7b.h"
#include <SPI.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#define WAIT 333
#define steps 10
#define w 240
#define h 240

const char* host = "esp32-webupdate";
const char* ssid = "test";
const char* password = "1q2w3e4r";

WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

TFT_eSPI tft = TFT_eSPI();

char temprandom1[8];
char temprandom2[8];
char temprandom3[8];
char temprandom4[8];
char temprandom5[8];
char temprandom6[8];
char temprandom7[8];
char temprandom8[8];

double gpsSpeed;
double distance;
double fuelTMP;

int RPM;
double COOLANT;
double ENGINE_FUEL_RATE;
double AMBIENT_TEMP;

int RUNTIME;
double VOLTAGE;
double FUEL_LEVEL;
double ENGINE_OIL_TEMP;

char tempSPEED[20];
char tempRPM[20];
char tempCOOLANT[20];
char tempFUEL[20];
char tempAMBIENT[20];

char tempRUN[20];
char tempDISTANCE[20];
char tempVOLTAGE[20];
char tempFUEL_LEVEL[20];
char tempENGINE_OIL_TEMP[20];

int EET = 2; // +2 / +3 East Europian Time

void setup(void)
{
  tft.init();
  tft.setRotation(0);
  ota();
}

void loop()
{
  server.handleClient();
  SerialGPS.begin(9600, SERIAL_8N1, 17, 16);

  obd.begin();
  while (!obd.init());

  showDTC();

  sprints();
  mainScreen();

  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      //debugInfoGPS();
      sprintfDataCalcs();
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }

  static byte pids[] = { PID_RPM, PID_RUNTIME, PID_FUEL_LEVEL, PID_ENGINE_FUEL_RATE, PID_CONTROL_MODULE_VOLTAGE, PID_AMBIENT_TEMP, PID_COOLANT_TEMP, PID_ENGINE_OIL_TEMP };
  static byte index = 0;
  byte pid = pids[index];
  int value;

  //send a query to OBD adapter for specified OBD - II pid

  if (obd.readPID(pid, value))
  {
    readData(pid, value);
  }

  index = (index + 1) % sizeof(pids);

  if (obd.errors >= 100)
  {
    reconnect();
    setup();
  }
}


void mainScreen(void)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(String(temprandom1), 0 + steps, 0 + steps, 7);
  tft.drawString(String(temprandom2), 0 + steps, 60 + steps, 7);
  tft.drawString(String(temprandom3), 0 + steps, 120 + steps, 7);
  tft.drawString(String(temprandom4), 0 + steps, 180 + steps, 7);

  tft.drawString(String(temprandom5), 120 + steps, 0 + steps, 7);
  tft.drawString(String(temprandom6), 120 + steps, 60 + steps, 7);
  tft.drawString(String(temprandom7), 120 + steps, 120 + steps, 7);
  tft.drawString(String(temprandom8), 120 + steps, 180 + steps, 7);
}

void ota(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(host);
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.printf("Ready! Open http://%s.local in your browser\n", host);
  } else {
    Serial.println("WiFi Failed");
  }
}




void sprintfDataCalcs()
{
  /* ------------ sprintf ------------ */

  sprintf(tempSPEED, "%03d", gpsSpeed);
  sprintf(tempRPM, "%04d", RPM);
  sprintf(tempCOOLANT, "%03d", COOLANT);
  sprintf(tempFUEL, "%00.1f", fuelTMP);
  sprintf(tempAMBIENT, "%02d", AMBIENT_TEMP);

  sprintf(tempRUN, "%03d", RUNTIME / 60);
  sprintf(tempDISTANCE, "%04d", distance / 36000);
  sprintf(tempVOLTAGE, "%00.1f", VOLTAGE);
  sprintf(tempFUEL_LEVEL, "%02d", FUEL_LEVEL);
  sprintf(tempENGINE_OIL_TEMP, "%03d", ENGINE_OIL_TEMP);

  /* ------------ Data Calculations ------------*/

  if (gps.speed.isValid())
  {
    gpsSpeed = (double)gps.speed.kmph();
    distance += gps.speed.kmph();
  }

  if (!gps.speed.isValid())
  {
    gpsSpeed = 1;
  }
  if (fuelTMP <= 0) fuelTMP = 0.01;

  if (ENGINE_FUEL_RATE <= 0) ENGINE_FUEL_RATE = 0.01;

  fuelTMP = (ENGINE_FUEL_RATE / gpsSpeed) / 0.036;
}


void reconnect()
{
  tft.fillScreen(TFT_BLACK);
  tft.drawString("reconnecting", 0 + steps, 0 + steps, 4);

  for (uint16_t i = 0; !obd.init(); i++)
  {
    if (i == 3)
    {
      tft.fillScreen(TFT_BLACK);
    }
    delay(500);
  }
}

void readData(byte pid, int value)
{
  switch (pid)
  {
    case PID_RPM:
      //if (value > -1 && value < 16000)
      RPM = value;
      break;

    case PID_COOLANT_TEMP:
      //if (value > -100 && value < 240)
      COOLANT = value;
      break;

    case PID_ENGINE_FUEL_RATE:
      //if (value > -1 && value < 3200)
      ENGINE_FUEL_RATE = (double)value;
      break;

    case PID_RUNTIME:
      RUNTIME = value;
      break;

    case PID_CONTROL_MODULE_VOLTAGE:
      VOLTAGE = (double)value;
      break;

    case PID_AMBIENT_TEMP:
      AMBIENT_TEMP = value;
      break;

    case PID_FUEL_LEVEL:
      FUEL_LEVEL = value;
      break;

    case PID_ENGINE_OIL_TEMP:
      ENGINE_OIL_TEMP = value;
      break;
  }
}

void showDTC()
{
  uint16_t codes[6];
  int dtcCount = obd.readDTC(codes, 6);

  if (dtcCount == 0)
  {
    tft.drawString("no errors", 0 + steps/*някъде*/, 0 + steps/*някъде*/, 4);
  }
  else
  {
    tft.drawNumber(dtcCount, 0 + steps/*някъде*/, 0 + steps/*някъде*/, 4);
  }
}


void debugInfoGPS() // GPS Debug Info
{
  Serial.print("Sat`s: ");
  if (gps.satellites.isValid()) Serial.print(gps.satellites.value());

  if (!gps.satellites.isValid())
  {
    Serial.print(" ");
    Serial.print("NO FIX");
  }

  Serial.print(" | ");

  Serial.print("Location: ");
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(" / ");
    Serial.print(gps.location.lng(), 6);
  }

  if (!gps.location.isValid()) Serial.print("NOFIX");

  Serial.print(" | ");

  if (gps.date.isValid())
  {
    if (gps.date.day() < 10) Serial.print("0");
    Serial.print(gps.date.day());
    Serial.print(":");
    if (gps.date.month() < 10) Serial.print("0");
    Serial.print(gps.date.month());
    Serial.print(":");
    Serial.print(gps.date.year());
  }

  if (!gps.date.isValid()) Serial.print("NOFIX");

  Serial.print(" | ");

  if (gps.time.isValid())
  {
    if (gps.time.hour() + EET < 10) Serial.print("0");
    Serial.print(gps.time.hour() + EET);
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());
  }

  if (!gps.time.isValid()) Serial.print("NOFIX");

  Serial.print(" | ");
  Serial.print("Speed: ");

  if (gps.speed.isValid())
  {
    Serial.print(gps.speed.kmph());
  }

  if (!gps.speed.isValid()) Serial.print("NO FIX");

  Serial.print(" | ");

  Serial.print(distance / 36000, 2);
  Serial.print(" | ");

  Serial.print(ENGINE_FUEL_RATE);
  Serial.print(" | ");
  Serial.print(tempFUEL);
  Serial.println(" | ");
}
