#include <Arduino.h>

class Button_Configurator {
  public:
    static const unsigned long DEBOUNCE_TIME = 50;
};

typedef void (*callbackFunction)(void);

class Button {
  private:
    uint8_t _pin;
    static void _ISR();
    static Button* _instance;
    callbackFunction _clickCallback = NULL;

  public:
    Button(uint8_t pin);

    void init();
    void handleInterrupt();
    void onClick(void (*callback)());
    void update();

    bool clicked = false;
    unsigned long lastClickTime = 0;
};
