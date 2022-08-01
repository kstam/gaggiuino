#include "pump.h"
#include "pindef.h"
#include <PSM.h>
#include "utils.h"

PSM pump(zcPin, dimmerPin, PUMP_RANGE, ZC_MODE, 2, 4);

short maxPumpClicksPerSecond = 50;

float maxFlowAt0Bar = 0.28;
float maxFlowAt5Bar = 0.151;
float maxFlowAt8Bar = 0.117;
float maxFlowAt15Bar = 0;

// Initialising some pump specific specs, mainly:
// - max pump clicks(dependant on region power grid spec)
// - pump clicks at 0 pressure in the system
void pumpInit(int powerLineFrequency, float pumpFlowAtZero) {
  maxPumpClicksPerSecond = powerLineFrequency;
  maxFlowAt0Bar = 50.f * maxFlowAt0Bar / powerLineFrequency;
  maxFlowAt5Bar = 50.f * maxFlowAt5Bar / powerLineFrequency;
  maxFlowAt8Bar = 50.f * maxFlowAt8Bar / powerLineFrequency;
  maxFlowAt15Bar = 50.f * maxFlowAt15Bar / powerLineFrequency;
}

// Function that returns the percentage of clicks the pump makes in it's current phase
float getPumpPct(float targetPressure, float flowRestriction, SensorState &currentState) {
  if (targetPressure == 0) {
    return 0;
  }

  float diff = targetPressure - currentState.pressure;
  float maxPumpPct = flowRestriction > 0 ? getClicksPerSecondForFlow(flowRestriction, currentState.pressure) / maxPumpClicksPerSecond : 1.f;
  float pumpPctToMaintainFlow = getClicksPerSecondForFlow(currentState.pumpFlow, currentState.pressure) / maxPumpClicksPerSecond;

  if (diff > 0.f) {
    return fminf(maxPumpPct, pumpPctToMaintainFlow + fmin(100.f, 0.25f + 0.20f * diff));
  }

  if (diff < 0.f &&  currentState.isPressureFalling) {
    return fminf(maxPumpPct, pumpPctToMaintainFlow * 0.2f + fmax(0.f, 0.1f + 0.2f * diff));
  }

  return 0;
}

// Sets the pump output based on a couple input params:
// - live system pressure
// - expected target
// - flow
// - pressure direction
void setPumpPressure(float targetPressure, float flowRestriction, SensorState &currentState) {
  float pumpPct = getPumpPct(targetPressure, flowRestriction, currentState);

  pump.set(pumpPct * PUMP_RANGE);
}

void setPumpOff(void) {
  pump.set(0);
}

void setPumpFullOn(void) {
  pump.set(PUMP_RANGE);
}

void setPumpToRawValue(unsigned int val) {
  pump.set(val);
}

long getAndResetClickCounter(void) {
  long counter = pump.getCounter();
  pump.resetCounter();
  return counter;
}

float estimatePressureAtPump(float pressureAtPuck, float cps) {
  float difference = fmaxf(0.f, -0.668f +  0.183f * cps + -0.00168f * cps * cps);
  return pressureAtPuck + difference;
}

// Models the flow per click taking into account
// - the pump inefficiency due to pressure (linear)
// - the pump inefficiency due to higher cps (quadratic)
float getFlowPerClick(float pressure, float cps) {
  #ifdef GROUPHEAD_PRESSURE
    float pressureAtPump = estimatePressureAtPump(pressure, cps);
  #else
    float pressureAtPump = pressure;
  #endif

  if (pressureAtPump <= 5) {
    return mapRange(pressure, 0, 5, maxFlowAt0Bar, maxFlowAt5Bar, 2);
  } else if (pressureAtPump <= 8) {
    return mapRange(pressureAtPump, 5, 8, maxFlowAt5Bar, maxFlowAt8Bar, 2);
  } else {
    return mapRange(pressureAtPump, 8, 15, maxFlowAt8Bar, maxFlowAt15Bar, 2);
  }
}

// Follows the schematic from http://ulka-ceme.co.uk/E_Models.html modified to per-click
float getPumpFlow(float cps, float pressure) {
  return cps * getFlowPerClick(pressure, cps);
}

// Binary search for correct CPS in the range of 0,maxCps
// This is needed to find the solution for CPS of the cubicfunction cps * flowPerClick(pressure, cps)
// This loop always finds the solution in log2(maxCps) iterations ~= 7 iterations.
float getClicksPerSecondForFlow(float flow, float pressure) {
  float minCps = 0;
  float maxCps = maxPumpClicksPerSecond;
  float accuracy = 0.5f;
  float cps = 0;

  while (minCps <= maxCps) {
    cps = (minCps + maxCps) / 2.f;
    float estFlow = cps * getFlowPerClick(pressure, cps);
    if (estFlow == flow) {
      return cps;
    } else if (estFlow < flow) {
      minCps = cps + accuracy;
    } else {
      maxCps = cps - accuracy;
    }
  }
  return cps;
}

// Calculates pump percentage for the requested flow and updates the pump raw value
void setPumpFlow(float targetFlow, float pressureRestriction, SensorState &currentState) {
  // If a pressure restriction exists then the we go into pressure profile with a flowRestriction
  // which is equivalent but will achieve smoother pressure management
  if (pressureRestriction > 0) {
    setPumpPressure(pressureRestriction, targetFlow, currentState);
  } else {
    float pumpPct = getClicksPerSecondForFlow(targetFlow, currentState.pressure) / (float) maxPumpClicksPerSecond;
    setPumpToRawValue(pumpPct * PUMP_RANGE);
  }
}
