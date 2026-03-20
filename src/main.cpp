#include <Arduino.h>
#include "Led.h"
#include "Button.h"

constexpr uint8_t LED_1_PIN = 15;
constexpr uint8_t BUTTON_1_PIN = 4;

Led led_1(LED_1_PIN);
Button button_1(BUTTON_1_PIN);

void handleClick() {
  led_1.toggleBlinkerPattern();
}

void setup() {
  Serial.begin(115200);

  button_1.init();
  button_1.onClick(handleClick);

  led_1.init();

  // Minimalistic example
  led_1.attachBlinker(Led::REGULAR);
  // Custom pattern
  // led_1.attachBlinker("--_--___-__", 140);
}

void loop() {
  led_1.update();
  button_1.update();
}
