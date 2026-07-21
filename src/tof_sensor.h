#ifndef TOF_SENSOR_H
#define TOF_SENDOR_H

#include <Arduino.h>
#include <vector>
#include "LCC_Node_Component_Base.h"
#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include "Adafruit_VL53L0X.h"

#define MULTIPLEXER_I2C_ADDRESS 0x70

/**
 * Class ToFThreshold represents one threshold for a ToFSensor.
 */
class ToFThreshold {
  public:
    /**
     * A constructor which allows near and far values to be set directly.
     * valueNear and valueFar are in mm.
     */
    ToFThreshold(uint8_t thresholdNumber, uint16_t valueNear, uint16_t valueFar, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      this->thresholdNumber = thresholdNumber;
      setValues(valueNear, valueFar);
      setEventIndexes(eventIndexNear, eventIndexFar);
      this->currentState = State::Unknown;
    }

    /**
     * Alternative constructor using a single value and a hysteris value.
     * value and hysterisis are in mm.
     */
    ToFThreshold(uint8_t thresholdNumber, uint16_t value, uint8_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      this->thresholdNumber = thresholdNumber;

      int valueNear = value - (hysterisis/2);
      int valueFar = value + (hysterisis/2);

      if (valueNear < 0) valueNear = 0;
      if (valueFar > 65536) valueFar = 65536;

      setValues((uint16_t) valueNear, (uint16_t) valueFar);
      setEventIndexes(eventIndexNear, eventIndexFar);
      this->currentState = State::Unknown;
    }

    const char* printCurrentState();

    enum State { Unknown, Near, Far };
    State currentState;

    uint8_t thresholdNumber;

    uint16_t eventIndexNear;
    uint16_t eventIndexFar;

    uint16_t valueNear;
    uint16_t valueFar;

  private:
    void setValues(uint16_t valueNear, uint16_t valueFar) { this->valueNear = valueNear; this->valueFar = valueFar; }
    void setEventIndexes(uint16_t eventIndexNear, uint16_t eventIndexFar) { this->eventIndexNear=eventIndexNear, this->eventIndexFar = eventIndexFar; }

};

/**
 * Class ToFSensor represents one Tof sensor, which will have one or more thresholds.
 */
class ToFSensor : public LCC_Node_Component_Base {
  public:
    ToFSensor(uint8_t sensorNumber, uint8_t multiplexorPort, Adafruit_VL53L0X* vl){ 
      this->sensorNumber = sensorNumber;
      this->multiplexorPort = multiplexorPort;
      this->vl5 = vl;
    }

    void addThreshold(uint8_t thresholdNumber, uint16_t valueNear, uint16_t valueFar, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      thresholds.push_back(ToFThreshold(thresholdNumber, valueNear, valueFar, eventIndexNear, eventIndexFar));
    }

    void addThreshold(uint8_t thresholdNumber, uint16_t value, uint8_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      thresholds.push_back(ToFThreshold(thresholdNumber, value, hysterisis, eventIndexNear, eventIndexFar));
    }

    void initialise(bool muxConnected);

    /**
     * Updates the specified threshold's value and hysterisis properties.
     * These will be called when the user has modified the configuration values.
     */
    void updateValueAndHysterisis(uint8_t thresholdNumber, uint8_t value, uint8_t hysterisis);

    /**
     * Returns true if index matches one of this object's events, else false.
     */
    bool eventIndexMatches(uint16_t index) override;

    /**
     * Returns true if index matches the threshold's current state, else false.
     * e.g. if index == eventIndexNear and the current state is Near, then return true.
     */
    bool eventIndexMatchesCurrentState(uint16_t index) override;

    /**
     * Called when sending initial events.
     * Used to initialise JMRI when JMRI starts after the node has started.
     */
    void sendEventsForCurrentState() override;

    /**
     * Called repeatedly from the main program loop.
     * Tests all thresholds to see if a change has occurred.
     * Sends any required events.
     */
    void loop();

    // /**
    //  * Called when a range has been received from the sensor.
    //  * Checks all thresholds for this sensor to see if one has been passed.
    //  * Sends the appropriate event if required.
    //  */
    // void check(uint8_t range);

    void print();

  private:
    /**
     * Called regularly to read the range from this sensor.
     * Returns the range or -1 if there is an error.
     */
    int read();

    std::vector<ToFThreshold> thresholds; // Stores all thresholds for this ToF sensor.

    /**
     * The number of this ToF sensor (0 to NUM_SENSOR - 1).
     */
    uint8_t sensorNumber; // needed ?? can use mux port instaed???

    /**
     * The port number on the I2C multiplexor for this ToF sensor.
     */
    uint8_t multiplexorPort;

    /**
     * The driver object for this ToF sensor.
     */
    Adafruit_VL6180X vl6;
    Adafruit_VL53L0X* vl5;

    /**
     * Is there a sensor connected to thsi mux port?
     * 
     */
    bool sensorConnected = false;
};

#endif
