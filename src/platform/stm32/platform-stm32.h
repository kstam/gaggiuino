#ifndef PLATFORM_STM32_H
#define PLATFORM_STM32_H

ADS1115 ADS(0x48);

void ads1115Init() {
  ADS.begin();
  ADS.setGain(0);      // 6.144 volt
  ADS.setDataRate(7);  // fast
  ADS.setMode(0);      // continuous mode
  ADS.readADC(0);      // first read to trigger
}

void platformInit(int hz) {
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

// Actuating the heater element
void setBoiler(int val) {

  if (val == HIGH) {
    digitalWrite(relayPin, HIGH);  // boilerPin -> HIGH
  } else {
    digitalWrite(relayPin, LOW);   // boilerPin -> LOW
  }
}

#endif
