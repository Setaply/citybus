#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
 
// Constants
AltSoftSerial gpsSerial;             // GPS on pins 8 (RX) and 9 (TX)
SoftwareSerial sim900(6, 7);         // SIM900 on pins 6 (RX) and 7 (TX)
TinyGPSPlus gps;
const int SIM900POWERPIN = 5; // SIM900 Power On Button
const char* apn = "internet.telekom";
const char* baseUrl = "http://217.154.87.99:3000/post-gps";
const unsigned long gpsDelay = 1000; // Update frequency in ms
const int busID = 1;
 
// Function declarations
void setupGPRS();
void sendGPSData(double latitude, double longitude);
void sendCommand(const String& command);
void waitForResponse(unsigned long timeout = 5000);
 
void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  sim900.begin(9600);
  delay(5000);
  Serial.println("Starting SIM900...");
  powerOnSIM900()
  delay(1000);
  Serial.println("Starting Citybus GPS...");
  setupGPRS();
  Serial.println("Citybus GPS Setup Finished!");
}
 
void loop() {
  bool gpsDataWasUpdated = false; 
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    double lat = gps.location.lat();
    double lng = gps.location.lng();

    sendGPSData(lat, lng);
    Serial.println("Sent GPS Data. Lat: " + String(lat, 6) + " | Lng: " + String(lng, 6));
  }
  //delay(gpsDelay); 
}
 
void setupGPRS() {
  sendCommand("AT");
  sendCommand("AT+CPIN?");
  sendCommand("AT+CREG?");
  sendCommand("AT+CSQ");
  sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  sendCommand("AT+SAPBR=3,1,\"APN\",\"" + String(apn) + "\"");
  sendCommand("AT+SAPBR=1,1");
  delay(3000);
  sendCommand("AT+SAPBR=2,1");
}
 
void sendGPSData(double latitude, double longitude) {
  String urlWithParams = String(baseUrl) + "?latitude=" + String(latitude, 6) + "&longitude=" + String(longitude, 6) + "&id=" + String(id, 6);
 
  sendCommand("AT+HTTPINIT");
  sendCommand("AT+HTTPPARA=\"CID\",1");
  sendCommand("AT+HTTPPARA=\"URL\",\"" + urlWithParams + "\"");
 
  sendCommand("AT+HTTPACTION=1");  // POST
  delay(3000); // Wait for Response
  sendCommand("AT+HTTPREAD");
  sendCommand("AT+HTTPTERM");
}
 
void sendCommand(const String& command) {
  sim900.println(command);
  waitForResponse();
}
 
void powerOnSIM900() {
  sim900.println("AT");
  delay(500);
  
  bool isOn = false;
  unsigned long start = millis();
  while (millis() - start < 2000) {
    if (sim900.available()) {
      String response = sim900.readString();
      if (response.indexOf("OK") != -1) {
        isOn = true;
        break;
      }
    }
  }

  if (isOn) {
    Serial.println("SIM900 already on.");
    return;
  }

  Serial.println("Powering on SIM900...");
  pinMode(SIM900POWERPIN, OUTPUT);
  digitalWrite(SIM900POWERPIN, LOW);
  delay(1000);
  digitalWrite(SIM900POWERPIN, HIGH);
  delay(2000);
  digitalWrite(SIM900POWERPIN, LOW);
  delay(3000);
}

 
void waitForResponse(unsigned long timeout) {
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while (sim900.available()) {
      char c = sim900.read();
    }
  }
}