#include <math.h>
#include "DHT.h"
#include "SevSeg.h"
#include "TimerOne.h"

#define SEVSEG_BRIGHTNESS 90

#define BUTTON_PIN 2

#define DHT_PIN 3
#define DHT_TYPE DHT11
#define DHT_HUMI_BLUE 60
#define DHT_HUMI_GREEN 50
#define DHT_TEMPE_GREEN 20
#define DHT_TEMPE_RED 27

#define LED_BLUE_PIN 17
#define LED_GREEN_PIN 19
#define LED_RED_PIN 18

#define TIMER1_INI 8192

SevSeg sevseg;
byte sevseg_num_digits = 4;
byte sevseg_digit_pins[] = {1, 0, 4, 5};
byte sevseg_segment_pins[] = {6, 7, 8, 9, 10, 11, 12, 13};
// volatile char sevseg_blocked, sevseg_state;
char sevseg_state, sevseg_toggled;

DHT dht(DHT_PIN, DHT_TYPE);
char dht_humi[2];
char dht_tempe[2];
char dht_buf[6];

void setup() {
    sevseg.begin(COMMON_ANODE, sevseg_num_digits, sevseg_digit_pins,
          sevseg_segment_pins, 1, 1);
    sevseg.setBrightness(SEVSEG_BRIGHTNESS);

    pinMode(BUTTON_PIN, INPUT);

    dht.begin();

    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);

    Timer1.initialize(TIMER1_INI);
    Timer1.attachInterrupt(sevseg_refresh_display);

    //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), set_sevseg_state, CHANGE);
}

void sevseg_refresh_display() {
    sevseg.refreshDisplay();
}

//void set_sevseg_state() {
//    if (sevseg_blocked)
//        return;
//
//    sevseg_blocked = 1;
//    sevseg_state ^= 1;
//}

int setHumidity() {
    dht_humi[1] = dht_humi[0];
    dht_humi[0] = roundf(dht.readHumidity());

    if (! sevseg_toggled && dht_humi[0] == dht_humi[1])
        return 1;

    snprintf(dht_buf, sizeof(dht_buf), "%d %s", dht_humi[0], "H");
    sevseg.setChars(dht_buf);

    if (dht_humi[0] >= DHT_HUMI_BLUE) {
        digitalWrite(LED_BLUE_PIN, HIGH);
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_GREEN_PIN, LOW);
    } else if (dht_humi[0] >= DHT_HUMI_GREEN) {
        digitalWrite(LED_GREEN_PIN, HIGH);
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_BLUE_PIN, LOW);
    } else {
        digitalWrite(LED_RED_PIN, HIGH);
        digitalWrite(LED_GREEN_PIN, LOW);
        digitalWrite(LED_BLUE_PIN, LOW);
    }

    return 0;
}

int setTemperature() {
    dht_tempe[1] = dht_tempe[0];
    dht_tempe[0] = roundf(dht.readTemperature());

    if (! sevseg_toggled && dht_tempe[0] == dht_tempe[1])
        return 1;

    snprintf(dht_buf, sizeof(dht_buf), "%d %s", dht_tempe[0], "C");
    sevseg.setChars(dht_buf);

    if (dht_tempe[0] >= DHT_TEMPE_RED) {
        digitalWrite(LED_RED_PIN, HIGH);
        digitalWrite(LED_GREEN_PIN, LOW);
        digitalWrite(LED_BLUE_PIN, LOW);
    } else if (dht_tempe[0] >= DHT_TEMPE_GREEN) {
        digitalWrite(LED_GREEN_PIN, HIGH);
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_BLUE_PIN, LOW);
    } else {
        digitalWrite(LED_BLUE_PIN, HIGH);
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_GREEN_PIN, LOW);
    }

    return 0;
}

void loop() {
    if (digitalRead(BUTTON_PIN)) {
        while (digitalRead(BUTTON_PIN)) {

        }
        sevseg_state ^= 1;
        sevseg_toggled = 1;
    }

    delay(50);

    if (sevseg_state) {
        setHumidity();
    } else {
        setTemperature();
    }

    sevseg_toggled = 0;
}

// vim: ft=c
