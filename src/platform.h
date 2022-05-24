#ifndef PLATFORM_H
#define PLATFORM_H

#include <Arduino.h>

void  pinInit();
void  initUART();
void  platformInit(int hz);
float getPressure();
void  setBoiler(int val);

#if defined(ARDUINO_ARCH_AVR)

  // ATMega32P pins definitions
  #define zcPin       2
  #define thermoDO    4
  #define thermoCS    5
  #define thermoCLK   6
  #define brewPin     A0
  #define relayPin    8
  #define dimmerPin   9
  #define steamPin    7
  #define pressurePin A1
  #define HX711_sck_1 10
  #define HX711_sck_2 11
  #define HX711_dout_1 12
  #define HX711_dout_2 13
  #define USART_DEBUG_RX  14
  #define USART_DEBUG_TX  15
  #define USART_CH        Serial

  #define ZC_MODE FALLING

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

  #if defined(USART_DEBUG_RX) && defined (USART_DEBUG_TX)
    #include <SoftwareSerial.h>
    SoftwareSerial Serial2(USART_DEBUG_RX, USART_DEBUG_TX);
    #define USART_DEBUG Serial2
  #endif

  #include <TimerInterrupt_Generic.h>
  #include "platform/avr/platform-avr.h"

#elif defined(ARDUINO_ARCH_STM32)
  #include "ADS1X15.h"
  #include "FlashStorage_STM32.h" // Probably must be kept here, see https://github.com/khoih-prog/FlashStorage_STM32/tree/main/examples/multiFileProject

  // STM32F4 pins definitions
  #define zcPin       PA15
  #define thermoDO    PA5
  #define thermoCS    PA6
  #define thermoCLK   PA7
  #define brewPin     PA11
  #define relayPin    PB9
  #define dimmerPin   PB3
  #define steamPin    PA12
  #define pressurePin ADS115_A0 //set here just for reference
  #define HX711_sck_1 PB0
  #define HX711_sck_2 PB1
  #define HX711_dout_1  PA1
  #define HX711_dout_2  PA2
  #define USART_DEBUG_RX  PC14
  #define USART_DEBUG_TX  PC15
  #define USART_CH      Serial

  #define ZC_MODE RISING

  #if defined(DEBUG_ENABLED)
    #include "dbg.h"
  #endif

  #if defined(USART_DEBUG_RX) && defined (USART_DEBUG_TX)
    #include <HardwareSerial.h>
    HardwareSerial Serial2(USART_DEBUG_RX, USART_DEBUG_TX);
    #define USART_DEBUG Serial2
  #endif

  #include "platform/stm32/platform-stm32.h"
#endif

/////////////////////////////////////////////
//////////// COMMON FUNCTIONS ///////////////
/////////////////////////////////////////////

void initUART() {
  USART_DEBUG.begin(115200);
  USART_CH.begin(115200);
}

void pinInit() {
  pinMode(relayPin, OUTPUT);
  pinMode(brewPin, INPUT_PULLUP);
  pinMode(steamPin, INPUT_PULLUP);
  pinMode(HX711_dout_1, INPUT_PULLUP);
  pinMode(HX711_dout_2, INPUT_PULLUP);
}

#endif
