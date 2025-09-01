#include "tof_sensor.h"

bool ToFSensor::eventIndexMatches(uint16_t index) {
  for (auto & threshold : thresholds) {
    if ((index == threshold.eventIndexNear) ||
        (index == threshold.eventIndexFar)) return true;
  }

  return false;
}

bool ToFSensor::eventIndexMatchesCurrentState(uint16_t index) {
  // Determine the threshold for this event index.
  for (auto & threshold : thresholds) {
    if (index == threshold.eventIndexNear) {
      return (threshold.currentState == ToFThreshold::State::Near) ? true : false;
    }
    if (index == threshold.eventIndexFar) {
      return (threshold.currentState == ToFThreshold::State::Far) ? true : false;
    }
  }

  return false;
}

void ToFSensor::sendEventsForCurrentState() {
  for (auto & threshold : thresholds) {
    if (threshold.currentState == ToFThreshold::State::Near) {
      if (sendEvent) sendEvent(threshold.eventIndexNear);
    }
    if (threshold.currentState == ToFThreshold::State::Far) {
      if (sendEvent) sendEvent(threshold.eventIndexFar);
    }
  }
}

void ToFSensor::check(uint8_t range) {
  for (auto & threshold : thresholds) {
    if (threshold.currentState == ToFThreshold::State::Far) {
      if (range < threshold.valueNear) {
        if (sendEvent) sendEvent(threshold.eventIndexNear);
      }
    } else if (threshold.currentState == ToFThreshold::State::Near) {
      if (range > threshold.valueFar) {
        if (sendEvent) sendEvent(threshold.eventIndexFar);
      }
    }
  }
}

void ToFSensor::print() {
  for (auto & threshold : thresholds) {
    Serial.printf("\nvalueNear=%d, valueFar=%d, eventIndexNear=%d, eventIndexFar=%d",
      threshold.valueNear, threshold.valueFar, threshold.eventIndexNear, threshold.eventIndexFar);
  }
}
