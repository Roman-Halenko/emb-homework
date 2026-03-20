#ifndef Button_h
#define Button_h

#include <Arduino.h>

class Button_Configurator {
  public:
    static const unsigned long DEBOUNCE_TIME = 50;
};

typedef void (*callbackFunction)(void);

class Button {
  private:
    volatile const uint8_t _pin;
    volatile bool clicked = false;
    unsigned long lastClickTime = 0;
    static void _ISR();
    static Button* _instance;
    callbackFunction _clickCallback = NULL;
    void handleInterrupt();

  public:
    Button(uint8_t pin);
    void init();
    void onClick(void (*callback)());
    void update();
};

#endif // Button_h
