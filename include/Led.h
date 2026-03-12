#include <Arduino.h>

enum LED_STATE {
  ON,
  OFF
};

class Led {
  private:
    uint8_t _pin;
    const char* _pattern = nullptr;

    uint32_t _divDuration = 0;
    uint32_t _delayMs = 0;
    uint32_t _lastTime = 0;

    size_t _index = 0;
    size_t _groupCount = 0;

    bool _running = false;
    bool _repeat = true;

    void _handleBlinker();

  public:
    Led(uint8_t pin);

    // Default patterns
    static constexpr char* REGULAR = "-_";
    static constexpr char* DOUBLE = "-_-_____";

    void init();
    void set(LED_STATE state);
    void toggle();
    void attachBlinker(const char* pattern, uint32_t divDuration = 100, bool repeat = true);
    void update();

    LED_STATE state = OFF;
};
