#include <Adafruit_Sensor.h>
#include <math.h>
#include <DHT.h>
#include <SevSeg.h>

#define DEBUG 0

#define SEV_SEG_BRIGHTNESS 90

#define DHTPIN 15
#define DHTTYPE DHT11

#define HUMI_BLUE 70
#define HUMI_GREEN 40
#define LED_BLUE 17
#define LED_GREEN 19
#define LED_RED 18
#define TEMPE_GREEN 18
#define TEMPE_RED 25

#define PIN_BUTTON 14

SevSeg sevseg;
byte numDigits = 4;
byte digitPins[] = {2, 3, 4, 5};
byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};

DHT dht(DHTPIN, DHTTYPE);
char humi;
char tempe;
char buf[14];

void setup() {
    sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins,0,1);
    sevseg.setBrightness(SEV_SEG_BRIGHTNESS);

    dht.begin();

    pinMode(PIN_BUTTON, INPUT);

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

}

void loop() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);

    if (digitalRead(PIN_BUTTON)) {
        humi = roundf(dht.readHumidity());

        if (humi >= HUMI_BLUE) {
            digitalWrite(LED_BLUE, HIGH);
        } else if (humi >= HUMI_GREEN) {
            digitalWrite(LED_GREEN, HIGH);
        } else {
            digitalWrite(LED_RED, HIGH);
        }

        snprintf(buf, sizeof(buf), "%d %s", humi, "H");
    } else {
        tempe = roundf(dht.readTemperature());

        if (tempe >= TEMPE_RED) {
            digitalWrite(LED_RED, HIGH);
        } else if (tempe >= TEMPE_GREEN) {
            digitalWrite(LED_GREEN, HIGH);
        } else {
            digitalWrite(LED_BLUE, HIGH);
        }

        snprintf(buf, sizeof(buf), "%d %s", tempe, "C");
    }
    
    sevseg.setChars(buf);

    sevseg.refreshDisplay();
}

// vim: ft=c
