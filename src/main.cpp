#include <Arduino.h>
#include <Led.h>

constexpr uint8_t LED_1_PIN = 15;

Led led_1(LED_1_PIN);

void setup() {
  Serial.begin(115200);

  led_1.init();
}

void loop() {

}
