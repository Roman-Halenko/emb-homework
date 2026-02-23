#include <Arduino.h>

#define BLUE_LED_PIN 15
#define RED_LED_PIN 16
#define INTERNAL_BUTTON_PIN 0
#define EXTERNAL_BUTTON_PIN 4

enum LED_mode {
  OFF,
  ON,
  SLOW,
  FAST
};

String LED_mode_names[sizeof(LED_mode)] = {"OFF", "ON", "SLOW", "FAST"};

LED_mode led_mode = SLOW;

void blink_led(int dl) {
  digitalWrite(BLUE_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  delay(dl);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(dl);
}

void LED_handle() {
  switch (led_mode) {
    case OFF:
      digitalWrite(BLUE_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      break;
    case ON:
      digitalWrite(BLUE_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
      break;
    case SLOW:
      blink_led(200);
      break;
    case FAST:
      blink_led(100);
      break;
    default:
      digitalWrite(BLUE_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      break;
  }
}

bool button_pressed(int pin) {
  int btnState = digitalRead(pin);
  if (btnState == LOW) {
    static const int debounceDelayTime = 30;
    delay(debounceDelayTime);
    return digitalRead(pin) == btnState;
  }
  return false;
}

bool button_released(int pin) {
  return digitalRead(pin) == HIGH;
}

void btn_click_handle(int btn,  void (*on_press)() = NULL, void (*on_release)() = NULL) {
  if (button_pressed(btn)) {
    if (on_press != NULL) on_press();
    while (!button_released(btn)) {
      // long press logic
    }
    if (button_released(btn)) {
      if (on_release != NULL) on_release();
    }
  }
}

void prev_mode() {
  led_mode = led_mode == OFF ? FAST : static_cast<LED_mode>(led_mode - 1);
  Serial.printf("Current LED mode: %s\n", LED_mode_names[led_mode]);
}

void next_mode() {
  led_mode = led_mode == FAST ? OFF : static_cast<LED_mode>(led_mode + 1);
  Serial.printf("Current LED mode: %s\n", LED_mode_names[led_mode]);
}

void setup() {
  Serial.begin(115200);

  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(INTERNAL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(EXTERNAL_BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);

  Serial.printf("Current LED mode: %s\n", LED_mode_names[led_mode]);
}

void loop() {
  LED_handle();
  btn_click_handle(INTERNAL_BUTTON_PIN, NULL, &prev_mode);
  btn_click_handle(EXTERNAL_BUTTON_PIN, NULL, &next_mode);
}
