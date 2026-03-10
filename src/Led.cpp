#include <Arduino.h>
#include <Led.h>

Led::Led(uint8_t pin) {
  _pin = pin;
}

void Led::init() {
  pinMode(_pin, OUTPUT);
}

void Led::set(LED_STATE state) {
  digitalWrite(_pin, state == ON ? HIGH : LOW);
}

void Led::toggle() {
  set(state == OFF ? ON : OFF);
}
