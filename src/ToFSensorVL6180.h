#ifndef TOF_SENSOR_VL6180_H
#define TOF_SENSOR_VL6180_H

#include "ToFSensor.h"
#include "Adafruit_VL6180X.h"

// #define SENSOR_TYPE_VL6180 "VL6180"
// const char* sensorTypeVL6180 = "VL6180";

class ToFSensorVL6180 : public ToFSensor {
  public:
    ToFSensorVL6180(uint8_t multiplexorPort, Adafruit_VL6180X *vl) { 
      this->multiplexorPort = multiplexorPort;
      this->vl = vl;
    }

    void initialise(bool muxConnected) final;

    const char* sensorType() final {
      return "VL6180";
    }

  private:
    /**
     * Called regularly to read the range from this sensor.
     * Returns the range or 255 if there is an error.
     */
    int read() final;

    /**
     * The driver object for this ToF sensor.
     */
    Adafruit_VL6180X *vl;

};

#endif
