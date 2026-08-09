#include "ToFSensorVL53X0L.h"

void ToFSensorVL53L0X::initialise(bool muxConnected) {

  if (! initialiseI2C(muxConnected, this->multiplexorPort)) {
    // No I2C device connected, so nothing to do!
    return;
  }

  // Check for a sensor on this port.
  if (! vl->begin()) {
    // There is no sensor on this port.
    if (muxConnected) {
      Serial.printf("\n%6ld [ToFSensorVL53L0X::initialise] No sensor on multiplexor port %d", millis(), this->multiplexorPort);
    } else {
      Serial.printf("\n%6ld [ToFSensorVL53L0X::initialise] No sensor directly connected", millis());
    }
    sensorConnected = false;
    return;
  } 

  if (muxConnected) {
    Serial.printf("\n%6ld [ToFSensorVL53L0X::initialise] Sensor initialised on multiplexor port %d", millis(), this->multiplexorPort);
  } else {
    Serial.printf("\n%6ld [ToFSensorVL53L0X::initialise] Sensor initialised directly connected", millis());
  }
  sensorConnected = true;

  setInitialState();
}

int ToFSensorVL53L0X::read() {
  int range;
  uint8_t rangeStatus;

  VL53L0X_RangingMeasurementData_t measure;
    
  vl->rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  // vl->rangingTest(&measure, true); // pass in 'true' to get debug data printout!

  rangeStatus = measure.RangeStatus;
  // if (rangeStatus != 4) {  // phase failures have incorrect data
  if (rangeStatus <= 2) {  // phase failures have incorrect data
    // range = measure.RangeDMaxMilliMeter;  // Shows that the maximum range is 953 mm.
    range = measure.RangeMilliMeter; 
  } else {
    // Serial.printf("\n out of range ");
    // range = -1;
    range = 1000;
  }

  // Serial.printf("\nStatus %d, Distance (mm): %d", rangeStatus, range);
  return range;
}
