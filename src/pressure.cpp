#include "pressure.h"

#define PUMP_RANGE 127


#if defined(ARDUINO_ARCH_AVR)
// configuration for TimerInterruptGeneric
#define TIMER_INTERRUPT_DEBUG     0
#define _TIMERINTERRUPT_LOGLEVEL_ 0

#define USING_16MHZ   true
#define USING_8MHZ    false
#define USING_250KHZ  false

#define USE_TIMER_0 false
#define USE_TIMER_1 true
#define USE_TIMER_2 false
#define USE_TIMER_3 false
#include <TimerInterrupt_Generic.h>

#define ZC_MODE FALLING

//Banoz PSM - for more cool shit visit https://github.com/banoz  and don't forget to star
PSM pump(zcPin, dimmerPin, PUMP_RANGE, ZC_MODE);
volatile int presData[2];
volatile char presIndex = 0;

void presISR() {
  presData[presIndex] = ADCW;
  presIndex ^= 1;
}

void pressureInit(int hz) {
  int pin = pressurePin - 14;
  ADMUX = (DEFAULT << 6) | (pin & 0x07);
  ADCSRB = (1 << ACME);
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ITimer1.init();
  ITimer1.attachInterrupt(hz * 2, presISR);
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

#elif defined(ARDUINO_ARCH_STM32)
#include "ADS1X15.h"
#define ZC_MODE RISING

PSM pump(zcPin, dimmerPin, PUMP_RANGE, ZC_MODE);
ADS1115 ADS(0x48);

void ads1115Init() {
  ADS.begin();
  ADS.setGain(0);      // 6.144 volt
  ADS.setDataRate(7);  // fast
  ADS.setMode(0);      // continuous mode
  ADS.readADC(0);      // first read to trigger
}

void pressureInit(int hz) {
  ads1115Init();
}

float getPressure() {  //returns sensor pressure data
    // 5V/1024 = 1/204.8 (10 bit) or 6553.6 (15 bit)
    // voltageZero = 0.5V --> 102.4(10 bit) or 3276.8 (15 bit)
    // voltageMax = 4.5V --> 921.6 (10 bit) or 29491.2 (15 bit)
    // range 921.6 - 102.4 = 819.2 or 26214.4
    // pressure gauge range 0-1.2MPa - 0-12 bar
    // 1 bar = 68.27 or 2184.5

  return ADS.getValue() / 1706.6f - 1.49f;
}

#endif

void setPump(int pct) {
  pump.set(pct * PUMP_RANGE / 100);
}

void setPressure(float targetValue) {
  int pumpValue;
  float livePressure = getPressure();
  float diff = targetValue - livePressure;

  if (targetValue == 0 || livePressure > targetValue) {
    pumpValue = 0;
  } else {
    float diff = targetValue - livePressure;
    pumpValue = PUMP_RANGE / (1.f + exp(1.7f - diff / 0.9f));
  }

  pump.set(pumpValue);
}
