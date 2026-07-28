#include "ToFSensorVL6180.h"

void ToFSensorVL6180::initialise(bool muxConnected) {
  // TO DO: move the first part of this function to the base class.
  // It can be called initialiseI2C().

  if (! muxConnected) {
    // No multiplexor connected, so nothing to do!
    return;
  }

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
    return;
  }

  // Check for a sensor on this port.
  if (! vl->begin()) {
    // There is no sensor on this port.
    Serial.printf("\n%6ld No sensor on multiplexor port %d", millis(), this->multiplexorPort);
    sensorConnected = false;
  } else {
    Serial.printf("\n%6ld Sensor on multiplexor port %d", millis(), this->multiplexorPort);
    sensorConnected = true;

    // Set the initial state for all thresholds for this sensor.
    int range = this->read();
    Serial.printf("\n%6ld initialising threshold states, range=%d", millis(), range);
    for (auto & threshold : thresholds) {
      Serial.printf("\n%6ld threshold number=%d, state=%s", millis(), threshold.thresholdNumber, threshold.printCurrentState());
      threshold.setInitialState(range);
    }
  }
}

int ToFSensorVL6180::read() {
  uint8_t range = vl->readRange();
  uint8_t status = vl->readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    return range;
  } else {
    return 255;
  }
}
