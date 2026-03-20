#ifndef Button_h
#define Button_h

#include <Arduino.h>

enum ButtonState {
  IDLE,
  PRESSED,
  RELEASED
};

class Button_Configurator {
  public:
    static const unsigned long DEBOUNCE_TIME = 50;
};

typedef void (*callbackFunction)(void);

class Button {
  private:
    const uint8_t _pin;
    const bool _activeLow = true;
    ButtonState _state = IDLE;
    unsigned long lastClickTime = 0;
    unsigned long _lastHighTime = 0;
    unsigned long _lastLowTime = 0;
    volatile bool _triggeredLow = false;
    volatile bool _triggeredHigh = false;
    static void _ISR_LOW(void* arg);
    static void _ISR_HIGH(void* arg);
    callbackFunction _clickCallback = NULL;
    void _set(ButtonState state);

  public:
    Button(uint8_t pin, bool activeLow = true);
    void init();
    void attachClick(void (*callback)());
    void update();
};

#endif // Button_h
