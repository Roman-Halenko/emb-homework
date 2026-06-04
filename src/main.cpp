#include <Arduino.h>
#include <HardwareSerial.h>
#include <Button2.h>

#define LED_PIN LED_BUILTIN
#define BUTTON_PIN 0 // "Boot" on ESP32, "Key" on STM32

#if defined(PLATFORM_ESP32)
  #define LED_ACTIVE_LOW false
  #define PIN_RX 17
  #define PIN_TX 15
  HardwareSerial halfDuplexUART(1);
#elif defined(PLATFORM_STM32)
  #define LED_ACTIVE_LOW true
  #define PIN_RX PA10
  #define PIN_TX PA9
  HardwareSerial halfDuplexUART(PIN_RX, PIN_TX);
#endif

Button2 button;

enum LED_STATE {
  OFF,
  ON
};

LED_STATE ledState = OFF;

void onBtnClick(Button2& btn) {
  ledState = ledState == OFF ? ON : OFF;
  halfDuplexUART.write(ledState);
  Serial.println("Button clicked!");
}

void setup() {
  Serial.begin(115200);

  button.begin(BUTTON_PIN);
  button.setClickHandler(onBtnClick);
  button.setLongClickHandler(onBtnClick);

  #if defined(PLATFORM_ESP32)
    halfDuplexUART.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX);
  #elif defined(PLATFORM_STM32)
    halfDuplexUART.begin(115200, SERIAL_8N1);
  #endif

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_ACTIVE_LOW ? HIGH : LOW);
}

void loop() {
  if(halfDuplexUART.available()) {
    while (halfDuplexUART.available() > 0) {
      int data = halfDuplexUART.read();
      digitalWrite(LED_PIN, LED_ACTIVE_LOW ? !data : data);
      Serial.printf("Message received: %d.\n", data);
    }
  }

  button.loop();
}
