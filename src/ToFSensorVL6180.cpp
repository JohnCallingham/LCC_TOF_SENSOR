#include "ToFSensorVL6180.h"

void ToFSensorVL6180::initialise(bool muxConnected) {

  if (! I2CPeripheral::initialiseI2C(muxConnected, this->multiplexorPort)) {
    // No I2C device connected, so nothing to do!
    return;
  }

  // switch (connectionType) {
  //   case ConnectionType::DIRECT:
  //     Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Initialising ToF sensor directly connected", millis());
  //     if (! initialiseI2CDirect()) {
  //       // No I2C device connected, so nothing to do!
  //       return;
  //     }
  //     break;
  //   case ConnectionType::MULTIPLEXOR:
  //     Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Initialising ToF sensor on multiplexor port %d", millis(), this->multiplexorPort);
  //     if (! muxConnected) {
  //       // No multiplexor connected, so nothing to do!
  //       return;
  //     }

  //     if (! initialiseI2CMultiplexor(this->multiplexorPort)) {
  //       // No I2C devices connected, so nothing to do!
  //       return;
  //     }
  //     break;
  // }

  // Check for a sensor on this port.
  if (! vl->begin()) {
    // There is no sensor on this port.
    Serial.printf("\n%6ld [ToFSensorVL6180::initialise] No sensor on multiplexor port %d", millis(), this->multiplexorPort);
    sensorConnected = false;
    return;
  } 

  Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Sensor on multiplexor port %d", millis(), this->multiplexorPort);
  sensorConnected = true;

  setInitialState();
}

// int ToFSensorVL6180::read() {
//   uint8_t range = vl->readRange();
//   uint8_t status = vl->readRangeStatus();

//   if (status == VL6180X_ERROR_NONE) {
//     return (int) range;
//   } else {
//     return 255;
//   }
// }
int ToFSensorVL6180::read() {
  int range = (int) vl->readRange();
  uint8_t status = vl->readRangeStatus();

  if (status != VL6180X_ERROR_NONE) {
    range = -1; // Indicate an error occurred.
  }

  // Serial.printf("\nStatus %d, Distance (mm): %d", status, range);
  return range;
}

  // if (connectionType == ConnectionType::DIRECT) {
  //   // if (! initialiseI2CDirect()) {
  //   //   // No I2C device connected, so nothing to do!
  //   //   return;
  //   // }

  //   // // Check for a sensor on this port.
  //   // if (! vl->begin()) {
  //   //   // There is no sensor on this port.
  //   //   Serial.printf("\n%6ld [ToFSensorVL6180::initialise] No sensor on multiplexor port %d", millis(), this->multiplexorPort);
  //   //   sensorConnected = false;
  //   //   return;
  //   // } 

  //   // Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Sensor on multiplexor port %d", millis(), this->multiplexorPort);
  //   // sensorConnected = true;
 

  // } else if (connectionType == ConnectionType::MULTIPLEXOR) {
  //   // Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Initialising ToF sensor on multiplexor port %d", millis(), this->multiplexorPort);

  //   // if (! muxConnected) {
  //   //   // No multiplexor connected, so nothing to do!
  //   //   return;
  //   // }

  //   // // if (! initialiseI2C()) {
  //   // if (! initialiseI2CMultiplexor(this->multiplexorPort)) {
  //   //   // No I2C devices connected, so nothing to do!
  //   //   return;
  //   // }

  //   // // Check for a sensor on this port.
  //   // if (! vl->begin()) {
  //   //   // There is no sensor on this port.
  //   //   Serial.printf("\n%6ld [ToFSensorVL6180::initialise] No sensor on multiplexor port %d", millis(), this->multiplexorPort);
  //   //   sensorConnected = false;
  //   //   return;
  //   // } 

  //   // Serial.printf("\n%6ld [ToFSensorVL6180::initialise] Sensor on multiplexor port %d", millis(), this->multiplexorPort);
  //   // sensorConnected = true;
  // }
