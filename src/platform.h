#ifndef PLATFORM_H
#define PLATFORM_H

#include <Arduino.h>
#include "mcu_pinout.h"

void  pinInit();
void  initUART();

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
