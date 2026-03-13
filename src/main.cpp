#include <Arduino.h>
#include <Led.h>
#include <Button.h>

constexpr uint8_t LED_1_PIN = 15;
constexpr uint8_t BUTTON_1_PIN = 0;

Led led_1(LED_1_PIN);
Button button_1(BUTTON_1_PIN);

void setup() {
  Serial.begin(115200);

  button_1.init();

  led_1.init();
  led_1.attachBlinker(Led::REGULAR);
}

void loop() {
  led_1.update();

  if(button_1.clicked) {
    led_1.attachBlinker(Led::DOUBLE);
    button_1.clicked = false;
  }
}
