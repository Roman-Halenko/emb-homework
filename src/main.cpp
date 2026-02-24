#include <Arduino.h>

#define ADC_PIN 15

const float U_ref = 3.3;
const float ADC_max = 4095.0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  uint16_t raw_val = analogRead(ADC_PIN);
  float u_calc = (raw_val / ADC_max) * U_ref;
  uint32_t u_measure = analogReadMilliVolts(ADC_PIN);
  Serial.printf("Raw: %d; U_calc: %.2fV; U_m: %dmV\n", raw_val, u_calc, u_measure);
  delay(100);
}
