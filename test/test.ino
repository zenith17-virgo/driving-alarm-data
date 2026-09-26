#include <WiFi.h>
#include <Wire.h>
#include <AdafruitIO_WiFi.h>
#include <HX711.h>


#define WIFI_SSID       "Hritam's M35" //will add later
#define WIFI_PASS       "6s3qpe5uuf34b72" //will add later

#define IO_USERNAME     "z3n1thviRgo" //will add later
#define IO_KEY          "aio_VDKI52NdcdXCG84Q4z1FkjoZIdMp" //will add later

#define HX711_DOUT_PIN 16 //input pin to read digital bits from sensor
#define HX711_SCK_PIN 17 //output pin for esp32 to send clock signal
#define PUMP_PIN 18
#define VALVE_PIN 19

HX711 pressureSensor;

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *healthFeed = io.feed("health-json");


unsigned long lastPublishTime = 0;
int sysval = 0;
int diaval = 0;

int ecgread() {
  if (digitalRead(32) == 1 || digitalRead(33) == 1) { //checks proper connection
    return 0;
  } else {
    return analogRead(34); //read ecg voltage
  }
}

float readPressuremmhg() {
  if (pressureSensor.is_ready()) {
    long raw = pressureSensor.get_units(1);
    float mmhg = raw /420.0;
    return (mmhg < 0) ? 0 : mmhg;
  }
  return 0.0;
}

void measurebp(int &sys, int &dia) {
  digitalWrite(VALVE_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);

  while (readPressuremmhg() < 160.0) {
    delay(50);
  }
}

  digitalWrite(VALVE_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);

  float maxOscillation = 0.0;
  float mapPressure = 100.0
  float previousreading = readPressuremmhg();

  while (readPressuremmhg() > 30.0) {
    float currentpressure = readPressuremmhg();
    float pulseDelta = abs(currentpressure - previousreading);

    if (pulseDelta > maxOscillation) {
      maxOscillation = pulseDelta;
      mapPressure = currentPressure;
    }

    previousreading = currentpressure;
    delay(50);

    sys = (int)(mapPressure * 1.25);
    dia = (int)(mapPressure * 0.72);

    digitalWrite(VALVE_PIN, LOW);
  }

void setup() {
  Serial.begin(115200);

  pinMode(32, INPUT); //loplus
  pinMode(33, INPUT); //lominus
  pinMode(34, INPUT); 

  pinMode(PUMP_PIN, OUTPUT); 
  pinMode(VALVE_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW); //shuts off motor
  digitalWrite(VALVE_PIN, LOW); //so that leftover air vents out 

  pressureSensor.begin(HX711_DOUT_PIN, HX711_SCK_PIN); //initializes comm interface, pinmodes of DOUT and SCK are done here internally

  Serial.println("Connecting to AdafruitIO")
  io.connect();
  while (io.status() < AIO_CONNECTED) {
    delay(500); //connects to adafruitio
  }
  Serial.println("Connected!");
}

void loop() {
  io.run(); //maintains mqtt


  if (millis() - lastPublishTime >= 10000) {
    lastPublishTime = millis();

    int ecgVal = ecgread(); //value published after 10 seconds
    measurebp(sysval, diaval);
    
    char jsonPayload[128];
    snprintf(jsonPayload, sizeof(jsonPayload),
      "{\"ecg\":%d, \"sys\":%d, \"dia\":%d}",
      ecgVal, sysval, diaval
    ); //JSON payload containing the three readings

    Serial.println(jsonPayload);
    healthFeed->save(jsonPayload); //displays in adafruit dashboard
  }
}