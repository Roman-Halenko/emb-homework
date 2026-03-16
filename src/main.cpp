#include <Arduino.h>

#define RELAY_OUT 4
#define RELAY_IN 16

volatile bool relayTriggered = false;

const ulong writeInterval = 1200;
const ulong relayTurnOffDelay = 500;
const size_t MEASUREMENTS = 10;

ulong lastRelayOn = 0;
ulong results[MEASUREMENTS];

uint8_t currentMeasureIdx = 0;

void IRAM_ATTR ISR() {
  relayTriggered = true;
}

void onRelayInterrupt() {
  if(relayTriggered) {
    // Detaching interrupt since very first rising (no debouncing approach)
    detachInterrupt(RELAY_IN);

    ulong now = millis();
    results[currentMeasureIdx] = now - lastRelayOn;

    Serial.printf("Delay = %lu ms\n", results[currentMeasureIdx++]);

    relayTriggered = false;
  }
}


void setup() {
  Serial.begin(115200);

  pinMode(RELAY_OUT, OUTPUT);
  digitalWrite(RELAY_OUT, LOW);

  pinMode(RELAY_IN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(RELAY_IN), ISR, RISING);
}

void loop() {
  ulong now = millis();

  if (currentMeasureIdx < MEASUREMENTS) {
    if (now - lastRelayOn >= writeInterval) {
      attachInterrupt(digitalPinToInterrupt(RELAY_IN), ISR, RISING);
      digitalWrite(RELAY_OUT, HIGH);
      lastRelayOn = now;
    }

    if (now - lastRelayOn >= relayTurnOffDelay) {
      digitalWrite(RELAY_OUT, LOW);
    }

    onRelayInterrupt();
  }
}
