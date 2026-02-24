#include <Arduino.h>

#define BUTTON_PIN 4

int16_t counter = 0;

void IRAM_ATTR reaction() {
  counter++;
  Serial.printf("\nButton Pressed! Count: %d", counter);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), reaction, FALLING);
}

void loop() {
  delay(100);
}
