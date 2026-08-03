#ifndef TOF_SENSOR_VL53X0L_H
#define TOF_SENSOR_VL53X0L_H

#include "ToFSensor.h"
#include "I2CPeripheral.h"
#include "Adafruit_VL53L0X.h"

class ToFSensorVL53L0X : public ToFSensor, public I2CPeripheral {
  public:
    ToFSensorVL53L0X(uint8_t multiplexorPort, Adafruit_VL53L0X *vl) { 
      connectionType = ConnectionType::MULTIPLEXOR;
      this->multiplexorPort = multiplexorPort;
      this->vl = vl;
    }

    ToFSensorVL53L0X(Adafruit_VL53L0X *vl) { 
      connectionType = ConnectionType::DIRECT;
      this->vl = vl;
    }

    void initialise(bool muxConnected) final;

    const char* sensorType() final {
      return "VL53L0X";
    }

  private:
    /**
     * Called regularly to read the range from this sensor.
     * Returns the range or 1000 if there is an error.
     */
    int read() final;

    /**
     * The driver object for this ToF sensor.
     */
    Adafruit_VL53L0X *vl;
  };

#endif
