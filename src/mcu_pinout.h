#ifndef MCU_PINOUT_H
#define MCU_PINOUT_H


#if defined(ARDUINO_ARCH_AVR)
  // ATMega32P pins definitions
  #define zcPin       3
  #define dimmerPin   4
  #define thermoDO    5
  #define thermoCS    6
  #define thermoCLK   7
  #define relayPin    8
  #define steamPin    9
  #define brewPin     A0
  #define pressurePin A1
  #define HX711_sck_1  10
  #define HX711_sck_2  11
  #define HX711_dout_1 12
  #define HX711_dout_2 13
  #define USART_DEBUG_RX  14
  #define USART_DEBUG_TX  15
  #define USART_CH        Serial

  #if defined(USART_DEBUG_RX) && defined (USART_DEBUG_TX)
    #include <SoftwareSerial.h>
    SoftwareSerial Serial2(USART_DEBUG_RX, USART_DEBUG_TX);
    #define USART_DEBUG Serial2
  #endif
#elif defined(ARDUINO_ARCH_STM32)
  // STM32F4 pins definitions
  #define zcPin          PA15
  #define thermoDO       PA5
  #define thermoCS       PA6
  #define thermoCLK      PA7
  #define brewPin        PA11
  #define relayPin       PB9
  #define dimmerPin      PB3
  #define steamPin       PA12
  #define pressurePin    ADS115_A0 //set here just for reference
  #define HX711_sck_1    PB0
  #define HX711_sck_2    PB1
  #define HX711_dout_1   PA1
  #define HX711_dout_2   PA2
  #define USART_DEBUG_RX PC14
  #define USART_DEBUG_TX PC15
  #define USART_CH       Serial

  #if defined(USART_DEBUG_RX) && defined (USART_DEBUG_TX)
    #include <HardwareSerial.h>
    HardwareSerial Serial2(USART_DEBUG_RX, USART_DEBUG_TX);
    #define USART_DEBUG Serial2
  #endif
#endif

#endif
