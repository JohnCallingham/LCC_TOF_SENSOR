#ifndef TOF_THRESHOLD_H
#define TOF_THRESHOLD_H

#include <Arduino.h>

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

    /**
     * Called when the sensor is first initialised so
     * that this threshold can have its state set correctly.
     */
    void setInitialState(int range);

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

#endif
