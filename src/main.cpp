#include <Arduino.h>
#include "Led.h"

#define LED_1_PIN 15
#define LED_2_PIN 16
#define LED_3_PIN 17

Led leds[] = {
  Led(LED_1_PIN),
  Led(LED_2_PIN),
  Led(LED_3_PIN)
};

void setup() {
  Serial.begin(115200);

  for (Led led : leds) {
    led.init();
  }

  leds[0].attachBlinker(Led::REGULAR, 200);
  leds[1].attachBlinker(Led::REGULAR, 500);
  leds[2].attachBlinker(Led::REGULAR, 1000);
}

void loop() {
  for (Led led : leds) {
    led.update();
  }
}
