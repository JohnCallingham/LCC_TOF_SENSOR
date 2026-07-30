#ifndef TOF_THRESHOLD_H
#define TOF_THRESHOLD_H

#include <Arduino.h>

/**
 * Class ToFThreshold represents one threshold for a ToFSensor.
 */
class ToFThreshold {
  public:
    /**
     * Value and hysterisis are in mm.
     */
    // ToFThreshold(uint8_t thresholdNumber, uint16_t value, uint8_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
    ToFThreshold(uint8_t thresholdNumber, uint16_t value, uint16_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      this->thresholdNumber = thresholdNumber;

      calculateNearAndFarValues(value, hysterisis);

      setEventIndexes(eventIndexNear, eventIndexFar);

      this->currentState = State::Unknown;
    }

    /**
     * Converts from value and hysterisis to near and far values.
     * Then stores near and far in this object.
     */
    void calculateNearAndFarValues(uint16_t value, uint16_t hysterisis) {
      int valueNear = value - (hysterisis/2);
      int valueFar = value + (hysterisis/2);

      if (valueNear < 0) valueNear = 0;
      if (valueFar > 65536) valueFar = 65536;

      setValues((uint16_t) valueNear, (uint16_t) valueFar);
    }

    /**
     * Called when the sensor is first initialised so
     * that this threshold can have its state set correctly.
     */
    void setInitialState(int range);

    /**
     * Returns true if this threshold is in state Near, else false.
     * Used to set the RGB LED if required.
     */
    bool isActive();

    /**
     * Checks whether range has caused this threshold to change its state.
     * Returns the event index to be sent or -1 if no event to send.
     */
    int check(int range);

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
