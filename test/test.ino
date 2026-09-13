#include <WiFi.h>
#include <Wire.h>
#include "AdafruitIO_WiFi.h"

#define WIFI_SSID       "CAMPUS CONNECT CUT"
#define WIFI_PASS       "075289"

#define IO_USERNAME     "YOUR_ADAFRUIT_USERNAME"
#define IO_KEY          "YOUR_ADAFRUIT_AIO_KEY"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *healthFeed = io.feed("health-json");

unsigned long lastPublishTime = 0;

void setup() {
  Serial.begin(115200);

  io.connect();
  while (io.status() < AIO_CONNECTED) {
    delay(500);
  }
}

void loop() {
  io.run();

  if (millis() - lastPublishTime >= 10000) {
    lastPublishTime = millis();

    // Generate dummy random sensor data
    int ecgVal    = random(200, 800);  // Simulated ECG ADC value
    int finalSpo2 = random(95, 100);   // Simulated SpO2 percentage
    int finalHr   = random(60, 100);   // Simulated Heart Rate (BPM)

    char jsonPayload[128];
    snprintf(jsonPayload, sizeof(jsonPayload),
      "{\"ecg\":%d,\"spo2\":%d,\"hr\":%d}",
      ecgVal, finalSpo2, finalHr
    );

    Serial.println(jsonPayload);
    healthFeed->save(jsonPayload);
  }
}