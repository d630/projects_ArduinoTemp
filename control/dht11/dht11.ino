#include <DHT.h>

#define DEBUG 1
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  if (DEBUG) {
    Serial.begin(9600);
  }

  dht.begin();
}

void loop() {
  delay(1000);

  float l = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (DEBUG) {
    Serial.print("Luftfeuchtigkeit: ");
    Serial.print(l);
    Serial.println(" %");
    Serial.print("Temperatur: ");
    Serial.print(t);
    Serial.println(" Grad Celsius");
  }
}
