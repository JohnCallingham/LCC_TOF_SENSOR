#ifndef TOF_SENSOR_H
#define TOF_SENSOR_H

#include <Arduino.h>
#include <vector>
#include "LCC_Node_Component_Base.h"
#include "ToFThreshold.h"
#include "Wire.h"
#include "Adafruit_VL6180X.h"

#define MULTIPLEXER_I2C_ADDRESS 0x70
#define READ_SENSOR_DELAY_mS 50

/**
 * Class ToFSensor represents one Tof sensor, which will have one or more thresholds.
 */
class ToFSensor : public LCC_Node_Component_Base {
  public:
    ToFSensor(uint8_t multiplexorPort, Adafruit_VL6180X *vl){ 
      this->multiplexorPort = multiplexorPort;
      this->vl6 = vl;
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

  private:
    /**
     * Called regularly to read the range from this sensor.
     * Returns the range or 255 if there is an error.
     */
    int read();

    /**
     * Called when a range has been received from the sensor.
     * Checks all thresholds for this sensor to see if one has been passed.
     * Sends the appropriate event if required.
     */
    void check(int range);

    void print();

    std::vector<ToFThreshold> thresholds; // Stores all thresholds for this ToF sensor.

    /**
     * The port number on the I2C multiplexor for this ToF sensor.
     */
    uint8_t multiplexorPort;

    /**
     * The driver object for this ToF sensor.
     */
    Adafruit_VL6180X *vl6;

    /**
     * Is there a sensor connected to thsi mux port?
     */
    bool sensorConnected = false;

    /**
     * nextRead is the value of millis() when the sensor will be read next.
     * Allows the LCC processing to run regularly and reading the sensor to not occur every time loop() is called.
     */
    unsigned long nextRead = 0;
};

#endif
