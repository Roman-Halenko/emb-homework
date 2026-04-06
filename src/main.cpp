#include <Arduino.h>

#define FAN_PIN 15 // GPIO pin connected to the fan relay
// #define VENTILATION_INTERVAL_US 3600000000ULL // 1 hour in microseconds
// #define VENTILATION_DURATION_US 900000000ULL  // 15 minutes in microseconds
#define VENTILATION_INTERVAL_US 3000000ULL // 3 seconds in microseconds (for testing purposes)
#define VENTILATION_DURATION_US 500000ULL  // 0.5 seconds in microseconds (for testing purposes)
#define TIMER_DIVIDER 80

enum VentilationState {
  VENTILATION_OFF,
  VENTILATION_ON
};

hw_timer_t * timer = NULL;

VentilationState volatile state = VENTILATION_OFF;

IRAM_ATTR void onTimer() {
  if (state == VENTILATION_ON) {
    state = VENTILATION_OFF;
    digitalWrite(FAN_PIN, LOW);
    timerAlarmWrite(timer, VENTILATION_INTERVAL_US - VENTILATION_DURATION_US, true);
  } else {
    state = VENTILATION_ON;
    digitalWrite(FAN_PIN, HIGH);
    timerAlarmWrite(timer, VENTILATION_DURATION_US, false);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  timer = timerBegin(0, TIMER_DIVIDER, true);

  timerAlarmWrite(timer, VENTILATION_INTERVAL_US, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmEnable(timer);
}

void loop() {

}
