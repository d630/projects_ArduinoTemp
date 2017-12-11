#include <math.h>
#include "DHT.h"
#include "SevSeg.h"

#define DEBUG 0

#define SEV_SEG_BRIGHTNESS 90

#define DHTPIN 15
#define DHTTYPE DHT11

#define HUMI_BLUE 60
#define HUMI_GREEN 50
#define LED_BLUE 17
#define LED_GREEN 19
#define LED_RED 18
#define TEMPE_GREEN 20
#define TEMPE_RED 27

#define PIN_BUTTON 14

SevSeg sevseg;
byte numDigits = 4;
byte digitPins[] = {2, 3, 4, 5};
byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};

DHT dht(DHTPIN, DHTTYPE);
char humi;
char tempe;
char buf[6];
char senState;

void setup() {
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins, 1, 1);
  sevseg.setBrightness(SEV_SEG_BRIGHTNESS);

  dht.begin();

  pinMode(PIN_BUTTON, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void loop() {
  if (digitalRead(PIN_BUTTON))
    senState ^= 1;

  if (senState) {
    humi = roundf(dht.readHumidity());
    snprintf(buf, sizeof(buf), "%d %s", humi, "H");
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
  } else {
    tempe = roundf(dht.readTemperature());
    snprintf(buf, sizeof(buf), "%d %s", tempe, "C");
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
  
  sevseg.setChars(buf);
  sevseg.refreshDisplay();
}

// vim: ft=c
