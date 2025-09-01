#ifndef TOF_SENSOR_H
#define TOF_SENDOR_H

#include <Arduino.h>
#include <vector>
#include "LCC_Node_Component_Base.h"

/**
 * Class ToFThreshold represents one threshold for a ToFSensor.
 */
class ToFThreshold {
  public:
    /**
     * A constructor which allows near and far values to be set directly.
     * valueNear and valueFar are in mm.
     */
    ToFThreshold(uint16_t valueNear, uint16_t valueFar, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      setValues(valueNear, valueFar);
      setEventIndexes(eventIndexNear, eventIndexFar);
      this->currentState = State::Unknown;
    }

    /**
     * Alternative constructor using a single value and a hysteris value.
     * value and hysterisis are in mm.
     */
    ToFThreshold(uint16_t value, uint8_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      int valueNear = value - (hysterisis/2);
      int valueFar = value + (hysterisis/2);

      if (valueNear < 0) valueNear = 0;
      if (valueFar > 65536) valueFar = 65536;

      setValues((uint16_t) valueNear, (uint16_t) valueFar);
      setEventIndexes(eventIndexNear, eventIndexFar);
      this->currentState = State::Unknown;
    }

    enum State { Unknown, Near, Far };
    State currentState;

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
    ToFSensor(uint8_t multiplexorPort) { this->multiplexorPort = multiplexorPort; }

    void addThreshold(uint16_t valueNear, uint16_t valueFar, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      thresholds.push_back(ToFThreshold(valueNear, valueFar, eventIndexNear, eventIndexFar));
    }

    void addThreshold(uint16_t value, uint8_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      thresholds.push_back(ToFThreshold(value, hysterisis, eventIndexNear, eventIndexFar));
    }

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
     * Called regularly to read the range from this sensor.
     * Returns the range or -1 if there is an error.
     */
    int read();

    /**
     * Called when a range has been received from the sensor.
     * Checks all thresholds for this sensor to see if one has been passed.
     * Sends the appropriate event if required.
     */
    void check(uint8_t range);

    void print();

  private:
    std::vector<ToFThreshold> thresholds; // Stores all thresholds for this ToF sensor.

    /**
     * The port number on the I2C multiplexor for this ToF sensor.
     */
    uint8_t multiplexorPort;
};

#endif
