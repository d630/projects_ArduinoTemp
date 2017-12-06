#define DEBUG 1

const int buttonPin = 2;
const int ledPin =  19;

int buttonState = 0;
int ledState = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  if (DEBUG) {
    Serial.begin(9600);
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    ledState = (ledState) ? 0 : 1;
    digitalWrite(ledPin, ledState);
    if (DEBUG) {
      Serial.println("HIGH");
    }
  } else {
    if (DEBUG) {
      Serial.println("LOW");
    }
  }
  
  delay(1000);
}
