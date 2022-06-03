#include "temperature.h"

#if defined(ARDUINO_ARCH_AVR)

void setBoiler(int val) {
  if (val == HIGH) {
    PORTB |= _BV(PB0);  // boilerPin -> HIGH
  } else {
    PORTB &= ~_BV(PB0);  // boilerPin -> LOW
  }
}

#elif defined(ARDUINO_ARCH_STM32)

void setBoiler(int val) {
  if (val == HIGH) {
    digitalWrite(relayPin, HIGH);  // boilerPin -> HIGH
  } else {
    digitalWrite(relayPin, LOW);   // boilerPin -> LOW
  }
}

#endif
