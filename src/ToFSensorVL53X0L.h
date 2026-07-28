#ifndef TOF_SENSOR_VL53X0L_H
#define TOF_SENSOR_VL53X0L_H

#include "ToFSensorBase.h"
#include "Adafruit_VL53L0X.h"

class ToFSensorVL53L0X : public ToFSensorBase {
  public:
    ToFSensorVL53L0X(uint8_t multiplexorPort, Adafruit_VL53L0X *vl) { 
      this->multiplexorPort = multiplexorPort;
      this->vl = vl;
    }

    void initialise(bool muxConnected) final;



  private:
    /**
     * Called regularly to read the range from this sensor.
     * Returns the range or 255 if there is an error.
     */
    int read() final;

    /**
     * The driver object for this ToF sensor.
     */
    Adafruit_VL53L0X *vl;





    
};

#endif
