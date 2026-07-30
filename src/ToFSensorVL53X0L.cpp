#include "ToFSensorVL53X0L.h"

void ToFSensorVL53L0X::initialise(bool muxConnected) {

  if (! muxConnected) {
    // No multiplexor connected, so nothing to do!
    return;
  }

  if (! initialiseI2C()) {
    // No I2C devices connected, so nothing to do!
    return;
  }

  // Check for a sensor on this port.
  if (! vl->begin()) {
    // There is no sensor on this port.
    Serial.printf("\n%6ld No sensor on multiplexor port %d", millis(), this->multiplexorPort);
    sensorConnected = false;
    return;
  }

  Serial.printf("\n%6ld Sensor on multiplexor port %d", millis(), this->multiplexorPort);
  sensorConnected = true;

  setInitialState();
}

int ToFSensorVL53L0X::read() {
  int range;

  VL53L0X_RangingMeasurementData_t measure;
    
  // Serial.print("Reading a measurement... ");
  vl->rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  // vl->rangingTest(&measure, true); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    // range = measure.RangeDMaxMilliMeter;  // Shows that the maximum range is 953 mm.
    range = measure.RangeMilliMeter; 
    // Serial.printf("\nDistance (mm): %d", range);
  } else {
    // Serial.printf("\n out of range ");
    range = 1000;
  }
    
  return range;
}
