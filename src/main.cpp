#include <Arduino.h>
#include "Led.h"
#include "Button.h"

#define LED_1_PIN 15
#define LED_2_PIN 16
#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 0

Led led_1(LED_1_PIN);
Led led_2(LED_2_PIN);
Button button_1(BUTTON_1_PIN);
Button button_2(BUTTON_2_PIN);

void handleClick_1() {
  led_1.toggleBlinkerPattern();
}

void handleClick_2() {
  led_2.toggle();
}

void setup() {
  Serial.begin(115200);

  button_1.init();
  button_1.attachClick(handleClick_1);

  button_2.init();
  button_2.attachClick(handleClick_2);

  led_1.init();
  led_2.init();

  // Minimalistic example
  led_1.attachBlinker(Led::REGULAR);
  // Custom pattern
  // led_1.attachBlinker("--_--___-__", 140);
}

void loop() {
  led_1.update();
  led_2.update();

  button_1.update();
  button_2.update();
}
