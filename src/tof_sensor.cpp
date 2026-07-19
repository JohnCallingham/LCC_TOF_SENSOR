#include "tof_sensor.h"

const char* ToFThreshold::printCurrentState() {
  switch (this->currentState) {
    case ToFThreshold::State::Unknown:
      return "Unknown";
    case ToFThreshold::State::Near:
      return "Near";
    case ToFThreshold::State::Far:
      return "Far";
  }

  return ""; // To keep the compiler happy!!
}

void ToFSensor::initialise(bool muxConnected) {

  if (! muxConnected) {
    // No multiplexor connected, so nothing to do!
    return;
  }

  vl = Adafruit_VL6180X();
  // vl = Adafruit_VL53L0X();

  // Switch the mux to this port.
  Wire.beginTransmission(MULTIPLEXER_I2C_ADDRESS);
  Wire.write(1 << this->multiplexorPort);
  Wire.endTransmission();

  int retVal;
  bool noDevices = true;

  for (int i2cAddress = 0x01; i2cAddress < 0x7F; i2cAddress++) {
    if (i2cAddress == 0x70) continue; // Ignore the multiplexor port address.
    Wire.beginTransmission(i2cAddress);
    retVal = Wire.endTransmission();

    if (retVal == 0) {
      Serial.printf("\n%6ld Found device at I2C address 0x%02X on mux port %d", millis(), i2cAddress, this->multiplexorPort);
      noDevices = false;
    }
  }

  if (noDevices) {
    Serial.printf("\n%6ld No I2C devices on mux port %d", millis(), this->multiplexorPort);
    return;
  }

  // Based on which addresses have been found, initialise the appropriate device.
  // VL53L0X is 0x29
  // The VL6180X has a default I2C address of 0x29!
  
  // Check for a sensor on this port.
  if (! vl.begin()) {
    // There is no sensor on this port.
    Serial.printf("\n%6ld No sensor on multiplexor port %d", millis(), this->multiplexorPort);
  } else {
    Serial.printf("\n%6ld Sensor on multiplexor port %d", millis(), this->multiplexorPort);
  }
}

void ToFSensor::updateValueAndHysterisis(uint8_t thresholdNumber, uint8_t value, uint8_t hysterisis) {
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
  // Serial.printf("\n%6ld In sendEventsForCurrentState(). Sensor: %d", millis(), this->sensorNumber);

  // this->print();

  for (auto & threshold : thresholds) {
    if (threshold.currentState == ToFThreshold::State::Near) {
      if (sendEvent) sendEvent(threshold.eventIndexNear);
    }
    if (threshold.currentState == ToFThreshold::State::Far) {
      if (sendEvent) sendEvent(threshold.eventIndexFar);
    }
  }
}

// void ToFSensor::check(uint8_t range) {
//   for (auto & threshold : thresholds) {
//     if (threshold.currentState == ToFThreshold::State::Far) {
//       if (range < threshold.valueNear) {
//         if (sendEvent) sendEvent(threshold.eventIndexNear);
//       }
//     } else if (threshold.currentState == ToFThreshold::State::Near) {
//       if (range > threshold.valueFar) {
//         if (sendEvent) sendEvent(threshold.eventIndexFar);
//       }
//     }
//   }
// }

void ToFSensor::loop() {



}

void ToFSensor::print() {
  for (auto & threshold : thresholds) {
    Serial.printf("\n%6ld Threshold: %d, State: %s, valueNear: %3d, valueFar: %3d,\n                eventIndexNear: 0x%02X, eventIndexFar: 0x%02X",
      millis(), threshold.thresholdNumber, threshold.printCurrentState(), threshold.valueNear, threshold.valueFar, threshold.eventIndexNear, threshold.eventIndexFar);
  }
}
