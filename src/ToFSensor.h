#ifndef TOF_SENSOR_BASE_H
#define TOF_SENSOR_BASE_H

#include <vector>
#include "LCC_Node_Component_Base.h"
#include "ToFThreshold.h"
#include <Wire.h>

#define MULTIPLEXER_I2C_ADDRESS 0x70
// #define READ_SENSOR_DELAY_mS 50 // OK for VL53X0L, but fails to read the CDI when the VL6180 is added !!!
#define READ_SENSOR_DELAY_mS 100 // Seems to work OK.

class ToFSensor : public LCC_Node_Component_Base {
  public:
      
    // void addThreshold(uint8_t thresholdNumber, uint16_t valueNear, uint16_t valueFar, uint16_t eventIndexNear, uint16_t eventIndexFar) {
    //   thresholds.push_back(ToFThreshold(thresholdNumber, valueNear, valueFar, eventIndexNear, eventIndexFar));
    // }

    // void addThreshold(uint8_t thresholdNumber, uint16_t value, uint8_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
    //   thresholds.push_back(ToFThreshold(thresholdNumber, value, hysterisis, eventIndexNear, eventIndexFar));
    // }
    void addThreshold(uint8_t thresholdNumber, uint16_t value, uint16_t hysterisis, uint16_t eventIndexNear, uint16_t eventIndexFar) {
      thresholds.push_back(ToFThreshold(thresholdNumber, value, hysterisis, eventIndexNear, eventIndexFar));
    }
    /**
     * Checks for an I2C device on this mux port.
     * Returns true if an I2C device is found on this mux port, else false;
     */
    int initialiseI2C();

    // Must be defined in derived classes.
    virtual void initialise(bool muxConnected) = 0;
    virtual int read() = 0;
    virtual const char* sensorType() = 0;

    /**
     * Updates the specified threshold's value and hysterisis properties.
     * These will be called when the user has modified the configuration values.
     */
    // void updateValueAndHysterisis(uint8_t thresholdNumber, uint8_t value, uint8_t hysterisis);
    void updateValueAndHysterisis(uint8_t thresholdNumber, uint16_t value, uint16_t hysterisis);

    // Inherited from class LCC_Node_Component_Base.
    bool eventIndexMatches(uint16_t index) final;
    bool eventIndexMatchesCurrentState(uint16_t index) final;
    void sendEventsForCurrentState() final;

    /**
     * Returns true if thresholdNumber is in state Near, else false.
     * Used to set the RGB LED if required.
     */
    bool isThresholdActive(uint8_t thresholdNumber);

    /**
     * Called repeatedly from the main program loop.
     * Tests all thresholds to see if a change has occurred.
     * Sends any required events.
     */
    void loop();

    /**
     * Is there a sensor connected to this mux port?
     */
    bool sensorConnected = false;

    protected:
    void setInitialState();


    /**
     * The port number on the I2C multiplexor for this ToF sensor.
     */
    uint8_t multiplexorPort;

    // /**
    //  * Is there a sensor connected to this mux port?
    //  */
    // bool sensorConnected = false;

    std::vector<ToFThreshold> thresholds; // Stores all thresholds for this ToF sensor.

  private:
    /**
     * Called when a range has been received from the sensor.
     * Checks all thresholds for this sensor to see if one has been passed.
     * Sends the appropriate event if required.
     */
    void check(int range);

    /**
     * nextRead is the value of millis() when the sensor will be read next.
     * Allows the LCC processing to run regularly and reading the sensor to not occur every time loop() is called.
     */
    unsigned long nextRead = 0;
};

#endif
