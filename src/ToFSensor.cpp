#include "ToFSensor.h"

// void ToFSensorBase::updateValueAndHysterisis(uint8_t thresholdNumber, uint8_t value, uint8_t hysterisis) {
void ToFSensor::updateValueAndHysterisis(uint8_t thresholdNumber, uint16_t value, uint16_t hysterisis) {
  // Find the specified threshold.
  for (auto & threshold : thresholds) {
    if (threshold.thresholdNumber == thresholdNumber) {
      // Update the threshold's near and far values.
      threshold.calculateNearAndFarValues(value, hysterisis);

      // threshold.valueNear = value - (hysterisis/2);
      // threshold.valueFar = value + (hysterisis/2);

      // if (threshold.valueNear < 0) threshold.valueNear = 0; //??? required for a uint8 ???
      // if (threshold.valueFar > 255) threshold.valueFar = 255; //??? required for a uint8 ???
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
  for (auto & threshold : thresholds) {
    if (threshold.currentState == ToFThreshold::State::Near) {
      if (sendEvent) sendEvent(threshold.eventIndexNear);
    }
    if (threshold.currentState == ToFThreshold::State::Far) {
      if (sendEvent) sendEvent(threshold.eventIndexFar);
    }
  }
}

void ToFSensor::loop() {
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
    int range = this->read(); // Calls read() in the class for the appropriate sensor.
    this->check(range);
  }
}

bool ToFSensor::isThresholdActive(uint8_t thresholdNumber) {
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

void ToFSensor::check(int range) {
  int eventIndexToSend;

  for (auto & threshold : thresholds) {
    eventIndexToSend = threshold.check(range);

    if (eventIndexToSend != -1) {
      if (sendEvent) sendEvent(eventIndexToSend);
    }
  }
}

int ToFSensor::initialiseI2C() {
  // Switch the mux to this port.
  Wire.beginTransmission(MULTIPLEXER_I2C_ADDRESS);
  Wire.write(1 << this->multiplexorPort);
  Wire.endTransmission();

  int retVal;
  bool noDevices = true;

  // Find I2C devices on this port.
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
    return false;
  }

  return true;
}

void ToFSensor::setInitialState() {
  // Set the initial state for all thresholds for this sensor.
  int range = this->read();
  Serial.printf("\n%6ld initialising threshold states, range=%d", millis(), range);
  for (auto & threshold : thresholds) {
    Serial.printf("\n%6ld threshold number=%d, state=%s", millis(), threshold.thresholdNumber, threshold.printCurrentState());
    threshold.setInitialState(range);
  }
}
