#include <Wire.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>

MAX30105 particleSensor;

uint32_t irbuffer[100];
uint32_t redbuffer[100];

int32_t spo2;
int8_t validspo2;
int32_t heartrate;
int8_t validheartrate;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))  {
    Sertial.println("Sensor not found");
    while (1 == 1);
  }
  particleSensor.setup();
  Serial.println("Initialized");
}

void loop () {
  for (byte i = 0; i < 100; i++) {
    while (!particleSensor.available())
      particleSensor.check();
    redbuffer[i] = particleSensor.getRed();
    irbuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
    }

  maxim_spo2_accuracy(irbuffer, 100, redbuffer, &spo2, &validspo2, &heartrate, &validheartrate);

  if (validheartrate == 1 && validspo2 == 1) {
    Serial.print("Spo2: ");
    Serial.print(spo2);
  }
}