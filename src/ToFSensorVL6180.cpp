#include "ToFSensorVL6180.h"

void ToFSensorVL6180::initialise(bool muxConnected) {

  if (connectionType == ConnectionType::DIRECT) {


  }

  if (connectionType == ConnectionType::MULTIPLEXOR) {

    if (! muxConnected) {
      // No multiplexor connected, so nothing to do!
      return;
    }

    // if (! initialiseI2C()) {
    if (! initialiseI2CMultiplexor(this->multiplexorPort)) {
      // No I2C devices connected, so nothing to do!
      return;
    }

    // Check for a sensor on this port.
    if (! vl->begin()) {
      // There is no sensor on this port.
      Serial.printf("\n%6ld [ToFSensorVL6180::initialise] No sensor on multiplexor port %d", millis(), this->multiplexorPort);
      sensorConnected = false;
      return;
    } 

    Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Sensor on multiplexor port %d", millis(), this->multiplexorPort);
    sensorConnected = true;
  }

  setInitialState();
}

int ToFSensorVL6180::read() {
  uint8_t range = vl->readRange();
  uint8_t status = vl->readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    return (int) range;
  } else {
    return 255;
  }
}
