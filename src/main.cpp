#include <Arduino.h>
#include <ESP32Servo.h>

#define ADC_PIN 15
#define SERVO_PIN 4

struct Interval {
  unsigned long last = 0;
};

Servo servo1;
Interval servo1_task;

const int servo1_min = 700;
const int servo1_max = 2200;
const int servo1_max_deg = 180;
const int servo1_freq = 50;
const int ADC_max = 4095;

int servo1_position = 0;

template<typename F>
void setInterval(Interval &timer, unsigned long interval, F callback) {
  unsigned long now = millis();
  if (now - timer.last >= interval) {
    timer.last = now;
    callback();
  }
}

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo1.setPeriodHertz(servo1_freq);
  servo1.attach(SERVO_PIN, servo1_min, servo1_max);
}

void loop() {
  setInterval(servo1_task, 10, []() {
    uint16_t raw_val = analogRead(ADC_PIN);
    servo1_position = (raw_val * servo1_max_deg) / ADC_max;
    servo1.write(servo1_position);
  });
}
