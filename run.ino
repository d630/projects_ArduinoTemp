#include <math.h>
#include "DHT.h"
#include "SevSeg.h"
#include "TimerOne.h"

#define DEBUG 0

#define SEV_SEG_BRIGHTNESS 90

#define DHTPIN 3
#define DHTTYPE DHT11

#define HUMI_BLUE 60
#define HUMI_GREEN 50
#define LED_BLUE 17
#define LED_GREEN 19
#define LED_RED 18
#define TEMPE_GREEN 20
#define TEMPE_RED 27

#define PIN_BUTTON 2

SevSeg sevseg;
byte numDigits = 4;
byte digitPins[] = {1, 0, 4, 5};
byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};

DHT dht(DHTPIN, DHTTYPE);
char humi;
char tempe;
char buf[6];
volatile char senState;

void setup() {
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins, 1, 1);
  sevseg.setBrightness(SEV_SEG_BRIGHTNESS);

  dht.begin();

  pinMode(PIN_BUTTON, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  Timer1.initialize(8192);
  Timer1.attachInterrupt(refreshDisplay);

  setTemperature();

  attachInterrupt(digitalPinToInterrupt(2), setSenState, CHANGE);
}

void loop() {
  if (senState) {
    setHumidity();
  } else {
    setTemperature();
  }
}

void refreshDisplay() {
  sevseg.refreshDisplay();
}

void setSenState() {
  // if (digitalRead(PIN_BUTTON))
    senState ^= 1;
}

void setHumidity() {
  humi = roundf(dht.readHumidity());
  
  snprintf(buf, sizeof(buf), "%d %s", humi, "H");
  sevseg.setChars(buf);
  setLEDHumidity();
}

void setLEDHumidity() {
  if (humi >= HUMI_BLUE) {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
  } else if (humi >= HUMI_GREEN) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
  } else {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  }
}

void setTemperature() {
  tempe = roundf(dht.readTemperature());
  snprintf(buf, sizeof(buf), "%d %s", tempe, "C");
  sevseg.setChars(buf);
  setLEDTemperature();
}

void setLEDTemperature() {
  if (tempe >= TEMPE_RED) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  } else if (tempe >= TEMPE_GREEN) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
  } else {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
  }
}

// vim: ft=c
