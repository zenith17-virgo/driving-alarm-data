#include <WiFi.h>
 const char* ssid = "CAMPUS CONNECT CUT";
 const char* password = "075289";
void setup() {
    Serial.begin(115200);
    delay(1000);

  Serial.println("System Initializing");
  Wifi.begin(ssid, password);

  pinMode(32, INPUT); //LO-plus
  pinMode(33, INPUT); //LO-minus
  pinMode(34, INPUT); //ecg voltage
  while (Wifi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
  Serial.print("Connected successfully");

  int ecgread() {
    if (digitalRead(32) == 1 || digitalRead(33) == 1) {
      Serial.println("Leads not connected properly");
      return 0;
    }
    else {
      int value = analogRead(34);
      return value;
    }  
  }
void loop() {
  int signal = ecgread();
  Serial.print("Signal is: ");
  Serial.println(signal);

  delay(20);
}
