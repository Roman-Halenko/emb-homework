#include <Arduino.h>

class Button_Configurator {
  public:
    static const unsigned long DEBOUNCE_TIME = 50;
};

class Button {
  private:
    uint8_t _pin;
    static void IRAM_ATTR _ISR();
    static Button* _instance;

  public:
    Button(uint8_t pin);

    void init();
    void handleInterrupt();

    bool clicked = false;
    unsigned long lastClickTime = 0;
};
