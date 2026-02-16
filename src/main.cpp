#include <Arduino.h>

#define BLUE_LED_PIN 15
#define RED_LED_PIN 46

const uint8_t short_phase_duration = 70;
const uint8_t long_phase_duration = 240;

void activate_police_lights() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(BLUE_LED_PIN, HIGH);
      delay(short_phase_duration);
      digitalWrite(BLUE_LED_PIN, LOW);
      delay(short_phase_duration);
    }
    for (int j = 0; j < 4; j++) {
      digitalWrite(RED_LED_PIN, HIGH);
      delay(short_phase_duration);
      digitalWrite(RED_LED_PIN, LOW);
      delay(short_phase_duration);
    }
  }
  for (int i = 0; i < 4; i++) {
    digitalWrite(BLUE_LED_PIN, HIGH);
    delay(long_phase_duration);
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(long_phase_duration);
    digitalWrite(RED_LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  delay(1000);

  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
}

void loop() {
  activate_police_lights();
}
