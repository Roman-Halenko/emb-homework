#include <Arduino.h>
#include <ESP32Servo.h>

#define ADC_PIN 15
#define SERVO_PIN 4

Servo servo1;

const int servo1_min = 700;
const int servo1_max = 2200;
const int servo1_max_deg = 180;
const int servo1_freq = 50;
const int ADC_max = 4095;

int servo1_position = 0;

void setup() {
  Serial.begin(115200);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo1.setPeriodHertz(servo1_freq);

  analogReadResolution(12);
}

void loop() {
  uint16_t raw_val = analogRead(ADC_PIN);

  servo1.attach(SERVO_PIN, servo1_min, servo1_max);
  servo1_position = (raw_val * servo1_max_deg) / ADC_max;
  servo1.write(servo1_position);
  delay(10);
}
