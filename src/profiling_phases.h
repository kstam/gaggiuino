#ifndef PROFILING_PHASES_H
#define PROFILING_PHASES_H

#include "utils.h"

enum PHASE_TYPE {
  PHASE_TYPE_FLOW,
  PHASE_TYPE_PRESSURE,
};

struct Phase {
    PHASE_TYPE type;
    float startValue;
    float endValue;
    float startRestriction;
    float endRestriction;
    unsigned int durationMs;

    float getTarget(unsigned long timeInPhase) {
      return mapRange(timeInPhase, 0, durationMs, startValue, endValue, 1);
    }

    float getRestriction(unsigned long timeInPhase) {
      return mapRange(timeInPhase, 0, durationMs, startRestriction, endRestriction, 1);
    }
};

struct CurrentPhase {
    int index;
    Phase &phase;
    unsigned long timeInPhase;

    float getTarget() {
      return phase.getTarget(timeInPhase);
    }

    float getRestriction() {
      return phase.getRestriction(timeInPhase);
    }
};

struct Phases {
    short count;
    Phase *phases;

    CurrentPhase getCurrentPhase(unsigned long timeInPhase) {
        short phaseIdx = 0;
        unsigned long accumulatedTime = 0;

        while (phaseIdx < count - 1 && timeInPhase >= accumulatedTime + (phases[phaseIdx].durationMs)) {
            accumulatedTime += phases[phaseIdx].durationMs;
            phaseIdx += 1;
        }
        return CurrentPhase{phaseIdx, phases[phaseIdx], timeInPhase - accumulatedTime};
    }
};

#endif
