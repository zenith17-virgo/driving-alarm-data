#include <WiFi.h>
 const char* ssid = "CAMPUS CONNECT CUT";
 const char* password = "075289";
void setup() {
    Serial.begin(115200);
    delay(1000);

  Serial.println("System Initializing");
  Wifi.begin(ssid, password);

  while (Wifi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
  Serial.print("Connected successfully");

void loop() {
  
}
