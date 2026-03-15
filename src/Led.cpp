#include <Arduino.h>
#include <Led.h>

Led::Led(uint8_t pin) {
  _pin = pin;
}

void Led::init() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void Led::set(LED_STATE state) {
  if (this->state != state) {
    digitalWrite(_pin, state == ON ? HIGH : LOW);
    this->state = state;
  }
}

void Led::toggle() {
  set(state == OFF ? ON : OFF);
}

void Led::attachBlinker(const char* pattern, uint32_t divDuration, bool repeat) {
  _pattern = pattern;
  _divDuration = divDuration;
  _repeat = repeat;
  _index = 0;
  _groupCount = 0;
  _lastTime = millis();
  _running = true;
}

void Led::toggleBlinkerPattern() {
  if (_pattern == Led::DOUBLE) {
    _pattern = Led::REGULAR;
  } else if (_pattern == Led::REGULAR) {
    _pattern = Led::DOUBLE;
  }
}

void Led::_handleBlinker() {
  if (!_running) return;

  if (millis() - _lastTime < _delayMs) return;

  _lastTime = millis();

  char current = _pattern[_index];
  if (current == '\0') {
    if(_repeat) {
      _index = 0;
      current = _pattern[_index];
    } else {
      _running = false;
      return;
    }
  }

  _groupCount = 1;

  while (_pattern[_index + _groupCount] == current) {
    _groupCount++;
  }

  _delayMs = _groupCount * _divDuration;

  _index += _groupCount;

  set(current == '-' ? ON : OFF);
}

void Led::update() {
  if(_pattern) {
    _handleBlinker();
  }
}
