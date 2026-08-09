#include "ToFSensorVL6180.h"

void ToFSensorVL6180::initialise(bool muxConnected) {

  if (! initialiseI2C(muxConnected, this->multiplexorPort)) {
    // No I2C device connected, so nothing to do!
    return;
  }

  // Check for a sensor on this port.
  if (! vl->begin()) {
    // There is no sensor on this port.
    if (muxConnected) {
      Serial.printf("\n%6ld [ToFSensorVL6180::initialise] No sensor on multiplexor port %d", millis(), this->multiplexorPort);
    } else {
      Serial.printf("\n%6ld [ToFSensorVL6180::initialise] No sensor directly connected", millis());
    }
    sensorConnected = false;
    return;
  } 

  if (muxConnected) {
    Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Sensor initialised on multiplexor port %d", millis(), this->multiplexorPort);
  } else {
    Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Sensor initialised directly connected", millis());
  }
  sensorConnected = true;

  setInitialState();
}

int ToFSensorVL6180::read() {
  int range = (int) vl->readRange();
  uint8_t status = vl->readRangeStatus();

  if (status != VL6180X_ERROR_NONE) {
    // range = -1; // Indicate an error occurred.
    range = 255; // Indicate an error occurred.
  }

  // Serial.printf("\nStatus %d, Distance (mm): %d", status, range);
  return range;
}
