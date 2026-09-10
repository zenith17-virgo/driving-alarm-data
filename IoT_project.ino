#include <Wire.h>
#include <MAX30105.h>
#include <heartrate.h>

MAX30105 particleSensor;

long lastbeat = 0;
float bpm = 0;
int avgbeat = 0;

void setup() {
  Serial.begin(115200);

  while (!Serial);

  Wire.begin(21, 22);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))  {
    
  }
}