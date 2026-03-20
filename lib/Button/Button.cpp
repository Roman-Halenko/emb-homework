#include <Arduino.h>
#include "Button.h"

Button::Button(uint8_t pin, bool activeLow) : _pin(pin), _activeLow(activeLow) {
}

void Button::init() {
  pinMode(_pin, _activeLow ? INPUT_PULLUP : INPUT_PULLDOWN);
  attachInterruptArg(digitalPinToInterrupt(_pin), _ISR_LOW, this, FALLING);
  attachInterruptArg(digitalPinToInterrupt(_pin), _ISR_HIGH, this, RISING);
}

void IRAM_ATTR Button::_set(ButtonState state) {
  _state = state;
}

void IRAM_ATTR Button::_ISR_LOW(void* arg) {
  Button* btn = static_cast<Button*>(arg);
  btn->_triggeredLow = true;
}
void IRAM_ATTR Button::_ISR_HIGH(void* arg) {
  Button* btn = static_cast<Button*>(arg);
  btn->_triggeredHigh = true;
}

void Button::attachClick(void (*callback)()) {
  _clickCallback = callback;
}

// void Button::attachDoubleClick(void (*callback)()) {
//   _doubleClickCallback = callback;
// }

// void Button::attachLongPress(void (*callback)()) {
//   _longPressCallback = callback;
// }

void Button::update() {
  unsigned long now = millis();

  if(_triggeredLow) {

    if (now - _lastLowTime >= Button_Configurator::DEBOUNCE_TIME) {
      _lastLowTime = now;
      _triggeredLow = false;
      _set(PRESSED);
    }
  }

  if(_triggeredHigh) {
    if (now - _lastHighTime >= Button_Configurator::DEBOUNCE_TIME) {
      _lastHighTime = now;
      _triggeredHigh = false;
      _set(RELEASED);
    }
  }

  if(_state == PRESSED) {

  }

  if(_state == RELEASED) {
    _clickCallback();
    _set(IDLE);
  }
}
