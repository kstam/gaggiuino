/* 09:32 15/03/2023 - change triggering comment */
#include "scales.h"
#include "pindef.h"
#include "remote_scales.h"

#include <HX711_2.h>
namespace {
  class LoadCellSingleton {
  public:
    static HX711_2& getInstance() {
      static HX711_2 instance(TIM3);
      return instance;
    }
  private:
    LoadCellSingleton() = default;
    ~LoadCellSingleton() = default;
  };
}


bool hwScalesPresent = false;

#if defined SINGLE_HX711_BOARD
unsigned char scale_clk = OUTPUT;
#else
unsigned char scale_clk = OUTPUT_OPEN_DRAIN;
#endif

void scalesInit(float scalesF1, float scalesF2) {
    // Forced predicitve scales in case someone with actual hardware scales wants to use them.
  if (FORCE_PREDICTIVE_SCALES) {
    hwScalesPresent = false;
    return;
  }

#ifndef DISABLE_HW_SCALES
  auto& loadCells = LoadCellSingleton::getInstance();
  loadCells.begin(HX711_dout_1, HX711_dout_2, HX711_sck_1, 128U, scale_clk);
  loadCells.set_scale(scalesF1, scalesF2);
  loadCells.power_up();

  if (loadCells.wait_ready_timeout(500, 10)) {
    loadCells.tare(4);
    hwScalesPresent = true;
  }
#endif

  if (!hwScalesPresent && remoteScalesIsPresent()) {
    remoteScalesTare();
  }
}

void scalesTare(void) {
  if (hwScalesPresent) {
    auto& loadCells = LoadCellSingleton::getInstance();
    if (loadCells.wait_ready_timeout(150, 10)) {
      loadCells.tare(4);
    }
  }
  else if (remoteScalesIsPresent()) {
    remoteScalesTare();
  }
}

float scalesGetWeight(void) {
  if (hwScalesPresent) {
    float currentWeight = 0.f;
    auto& loadCells = LoadCellSingleton::getInstance();
    if (loadCells.wait_ready_timeout(150, 10)) {
      float values[2];
      loadCells.get_units(values);
      currentWeight = values[0] + values[1];
    }
    return currentWeight;
  }
  else if (remoteScalesIsPresent()) {
    return remoteScalesGetWeight();
  }
  return 0.f;
}

bool scalesIsPresent(void) {
  // Forced predicitve scales in case someone with actual hardware scales wants to use them.
  if (FORCE_PREDICTIVE_SCALES) {
    return false;
  }
  return hwScalesPresent || remoteScalesIsPresent();
}

float scalesDripTrayWeight() {
  long value[2] = {};
  LoadCellSingleton::getInstance().read_average(value, 4);

  return ((float)value[0] + (float)value[1]);
}
