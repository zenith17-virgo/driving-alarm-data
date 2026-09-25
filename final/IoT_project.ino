#include <WiFi.h>
#include <Wire.h>
#include <AdafruitIO_WiFi.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>

#define WIFI_SSID       "" //will add later
#define WIFI_PASS       "" //will add later

#define IO_USERNAME     "" //will add later
#define IO_KEY          "" //will add later

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
    return analogRead(34);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(32, INPUT); //loplus
  pinMode(33, INPUT); //lominus
  pinMode(34, INPUT); 

  Wire.begin(21, 22); //for max30102
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    while (1);
  }
  particleSensor.setup();

  io.connect();
  while (io.status() < AIO_CONNECTED) {
    delay(500); //connects to adafruitio
  }
}

void loop() {
  io.run();

  for (byte i = 0; i < 100; i++) {
    while (!particleSensor.available()) 
    particleSensor.check();
    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
  }

  maxim_spo2_accuracy(irBuffer, 100, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  if (millis() - lastPublishTime >= 10000) {
    lastPublishTime = millis();

    int ecgVal = ecgread();
    int finalSpo2 = (validSPO2 == 1) ? spo2 : 0;
    int finalHr   = (validHeartRate == 1) ? heartRate : 0;

    char jsonPayload[128];
    snprintf(jsonPayload, sizeof(jsonPayload),
      "{\"ecg\":%d,\"spo2\":%d,\"hr\":%d}",
      ecgVal, finalSpo2, finalHr
    ); //JSON payload

    Serial.println(jsonPayload);
    healthFeed->save(jsonPayload); //displays in adafruit dashboard
  }
}