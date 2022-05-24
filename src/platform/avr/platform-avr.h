#ifndef PLATFORM_AVR_H
#define PLATFORM_AVR_H

volatile int presData[2];
volatile char presIndex = 0;

void presISR() {
  presData[presIndex] = ADCW;
  presIndex ^= 1;
}

void initPressure(int hz) {
    int pin = pressurePin - 14;
    ADMUX = (DEFAULT << 6) | (pin & 0x07);
    ADCSRB = (1 << ACME);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    ITimer1.init();
    ITimer1.attachInterrupt(hz * 2, presISR);
}

void platformInit(int hz) {
  initPressure(hz);
}

float getPressure() {  //returns sensor pressure data
    // 5V/1024 = 1/204.8 (10 bit) or 6553.6 (15 bit)
    // voltageZero = 0.5V --> 102.4(10 bit) or 3276.8 (15 bit)
    // voltageMax = 4.5V --> 921.6 (10 bit) or 29491.2 (15 bit)
    // range 921.6 - 102.4 = 819.2 or 26214.4
    // pressure gauge range 0-1.2MPa - 0-12 bar
    // 1 bar = 68.27 or 2184.5

    return (presData[0] + presData[1]) / 136.54f - 1.49f;
}

// Actuating the heater element
void setBoiler(int val) {

  if (val == HIGH) {
    PORTB |= _BV(PB0);  // boilerPin -> HIGH
  } else {
    PORTB &= ~_BV(PB0);  // boilerPin -> LOW
  }
}

#endif
