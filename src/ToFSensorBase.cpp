#include "ToFSensorBase.h"

void ToFSensorBase::updateValueAndHysterisis(uint8_t thresholdNumber, uint8_t value, uint8_t hysterisis) {
  // Find the specified threshold.
  for (auto & threshold : thresholds) {
    if (threshold.thresholdNumber == thresholdNumber) {
      // Update the threshold's near and far values.
      threshold.valueNear = value - (hysterisis/2);
      threshold.valueFar = value + (hysterisis/2);

      if (threshold.valueNear < 0) threshold.valueNear = 0; //??? required for a uint8 ???
      if (threshold.valueFar > 255) threshold.valueFar = 255; //??? required for a uint8 ???
    }
  }
}

bool ToFSensorBase::eventIndexMatches(uint16_t index) {
  for (auto & threshold : thresholds) {
    if ((index == threshold.eventIndexNear) ||
        (index == threshold.eventIndexFar)) return true;
  }

  return false;
}

bool ToFSensorBase::eventIndexMatchesCurrentState(uint16_t index) {
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

void ToFSensorBase::sendEventsForCurrentState() {
  for (auto & threshold : thresholds) {
    if (threshold.currentState == ToFThreshold::State::Near) {
      if (sendEvent) sendEvent(threshold.eventIndexNear);
    }
    if (threshold.currentState == ToFThreshold::State::Far) {
      if (sendEvent) sendEvent(threshold.eventIndexFar);
    }
  }
}

void ToFSensorBase::loop() {
  // Need to cause a non blocking delay here to allow the LCC code to run smoothly.
  // Only read the sensor every READ_SENSOR_DELAY_mS.
  if (millis() < nextRead) {
    // Not ready to read yet.
    return;
  }

  // Schedule the next read for READ_SENSOR_DELAY_mS in the future.
  nextRead = millis() + READ_SENSOR_DELAY_mS;

  if (this->sensorConnected) {
    // Switch the mux to this port.
    Wire.beginTransmission(MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << this->multiplexorPort);
    Wire.endTransmission();

    // Get the current range and check if any threshold events need to be sent.
    int range = this->read();
    this->check(range);
  }
}

bool ToFSensorBase::isThresholdActive(uint8_t thresholdNumber) {
  // Find the required threshold object.
  for (auto & threshold : thresholds) {
    if (threshold.thresholdNumber == thresholdNumber) {
      // This is the required threshold object.
      return threshold.isActive();
    }
  }

  // Keep the compiler happy!!
  return false;
}

void ToFSensorBase::check(int range) {
  int eventIndexToSend;

  for (auto & threshold : thresholds) {
    eventIndexToSend = threshold.check(range);

    if (eventIndexToSend != -1) {
      if (sendEvent) sendEvent(eventIndexToSend);
    }
  }
}
