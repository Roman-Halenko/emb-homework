#include <Arduino.h>

#define BLUE_LED_PIN 15
#define RED_LED_PIN 16

const uint8_t blink_duration = 70;
const uint8_t blink_count = 4;
const uint8_t blinking_phase_repeat = 2;
const uint8_t constant_duration = 240;
const uint8_t constant_phase_repeat = 4;

void activate_police_lights() {
  for (int i = 0; i < blinking_phase_repeat; i++) {
    for (int j = 0; j < blink_count; j++) {
      digitalWrite(BLUE_LED_PIN, HIGH);
      delay(blink_duration);
      digitalWrite(BLUE_LED_PIN, LOW);
      delay(blink_duration);
    }
    for (int j = 0; j < blink_count; j++) {
      digitalWrite(RED_LED_PIN, HIGH);
      delay(blink_duration);
      digitalWrite(RED_LED_PIN, LOW);
      delay(blink_duration);
    }
  }
  for (int i = 0; i < constant_phase_repeat; i++) {
    digitalWrite(BLUE_LED_PIN, HIGH);
    delay(constant_duration);
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(constant_duration);
    digitalWrite(RED_LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  delay(1000);

  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}

void loop() {
  activate_police_lights();
}
