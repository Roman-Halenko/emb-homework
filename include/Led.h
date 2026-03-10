#include <Arduino.h>

enum LED_STATE {
  ON,
  OFF
};

class Led {
  private:
    uint8_t _pin;

  public:
    Led(uint8_t pin);

    void init();
    void set(LED_STATE state);
    void toggle();

    LED_STATE state = OFF;
};
