#ifndef TOF_SENSOR_VL6180_H
#define TOF_SENSOR_VL6180_H

#include "ToFSensor.h"
#include "I2CPeripheral.h"
#include "Adafruit_VL6180X.h"

class ToFSensorVL6180 : public ToFSensor, public I2CPeripheral {
  public:
    ToFSensorVL6180(uint8_t multiplexorPort, Adafruit_VL6180X *vl) { 
      connectionType = ConnectionType::MULTIPLEXOR;
      this->multiplexorPort = multiplexorPort;
      this->vl = vl;
    }

    ToFSensorVL6180(Adafruit_VL6180X *vl) { 
      connectionType = ConnectionType::DIRECT;
      this->vl = vl;
    }

    void initialise(bool muxConnected) final;

    const char* sensorType() final {
      return "VL6180";
    }

  private:
    /**
     * Called regularly to read the range from this sensor.
     * Returns the range or -1 if there is an error.
     */
    int read() final;

    /**
     * The driver object for this ToF sensor.
     */
    Adafruit_VL6180X *vl;
};

#endif
