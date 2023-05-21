#ifndef REMOTE_SCALE_H
#define REMOTE_SCALE_H

#include "esp_comms.h"

bool remoteScalesPresent = false;
float remoteScalesLatestWeight = 0.f;


const uint16_t REMOTE_SCALES_TARE_DEBOUNCE = 500;
uint32_t lastRemoteScalesTare = 0;
void remoteScalesTare(void) {
  uint32_t now = millis();
  if (now - lastRemoteScalesTare < REMOTE_SCALES_TARE_DEBOUNCE) {
    return;
  }

  espCommsSendTareScalesCommand();
  lastRemoteScalesTare = millis();
}

float remoteScalesGetWeight(void) {
  return remoteScalesLatestWeight;
}

bool remoteScalesIsPresent(void) {
  return remoteScalesPresent;
}

void onRemoteScalesWeightReceived(float weight) {
  remoteScalesPresent = true;
  remoteScalesLatestWeight = weight;
}

void onRemoteScalesDisconnected() {
  remoteScalesPresent = false;
}

#endif
