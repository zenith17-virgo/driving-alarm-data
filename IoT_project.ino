#include <WiFi.h>
#include <Wire.h>
#include <AdafruitIO_WiFi.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>

#define WIFI_SSID       "" //will add later
#define WIFI_PASS       "" //will add later

#define IO_USERNAME     "z3n1thviRgo"
#define IO_KEY          "aio_LLwC851dbM4WeORHogPPGWWkmJCz"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
MAX30105 particleSensor;
AdafruitIO_Feed *healthFeed = io.feed("health-json");

uint32_t irBuffer[100];
uint32_t redBuffer[100];
int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

unsigned long lastPublishTime = 0;

int ecgread() {
  if (digitalRead(32) == 1 || digitalRead(33) == 1) {
    return 0;
  } else {
    return analogRead(34); //ecg reading
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(32, INPUT); //loplus
  pinMode(33, INPUT); //lominus
  pinMode(34, INPUT); 

  
  io.connect();
  while (io.status() < AIO_CONNECTED) {
    delay(500); //connects to adafruit io
  }
}

void loop() {
  io.run(); //holds mqtt


  if (millis() - lastPublishTime >= 10000) {
    lastPublishTime = millis();

    int ecgVal = ecgread();
   
    char jsonPayload[128];
    snprintf(jsonPayload, sizeof(jsonPayload),
      "{\"ecg\":%d}",
      ecgVal
    ); //creates json payload

    Serial.println(jsonPayload);
    healthFeed->save(jsonPayload); //shows the data in adafruit io dashboard
  }
}