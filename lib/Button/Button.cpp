#include <Arduino.h>
#include "Button.h"

Button* Button::_instance = nullptr;

Button::Button(uint8_t pin) : _pin(pin) {
}

void Button::init() {
  _instance = this;
  attachInterrupt(digitalPinToInterrupt(_pin), _ISR, FALLING);
}

void IRAM_ATTR Button::_ISR() {
  if (_instance) {
    _instance->handleInterrupt();
  }
}

void IRAM_ATTR Button::handleInterrupt() {
  unsigned long now = millis();
  if (now - lastClickTime > Button_Configurator::DEBOUNCE_TIME) {
    clicked = true;
    lastClickTime = now;
  }
}

void Button::onClick(void (*callback)()) {
  _clickCallback = callback;
}

void Button::update() {
  if (clicked) {
    _clickCallback();
    clicked = false;
  }
}
