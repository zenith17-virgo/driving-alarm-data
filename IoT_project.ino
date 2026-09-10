#include <WiFi.h>
#include <AdafruitIO_Wifi.h>

const char* ssid = "CAMPUS CONNECT CUT";
const char* password = "075289";

#define IO_USERNAME  "z3n1thviRgo"
#define IO_KEY       "aio_LLwC851dbM4WeORHogPPGWWkmJCz"

#define ad8232_out 34
#define ad8232_lop 32
#define ad8232_lom 33

AdafruitIO_Wifi io(IO_USERNAME, IO_KEY, ssid, password);
AdafruitIO_Feed *ecgFeed = io.feed("ecg");
//other sensors will add later
unsigned long lastpublishtime = 0;
const unsigned long interval = 10000;
void setup() {
    Serial.begin(115200);
    delay(1000);
  //write pinmodes for all sensors later here
  Serial.println("Connecting to IO...");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(io.statusText());
}
  
void loop() {
  io.run(); 

  if (millis() - lastpublishtime >= interval) {
    lastpublishtime = millis();
  }

  //will configure for all sensors later on
}
