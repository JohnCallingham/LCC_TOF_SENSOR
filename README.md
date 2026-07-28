# LCC_TOF_SENSOR
This component is part of a suite of components which can be used as part of a program which implements an OpenLCB/LCC node. It has been developed using PlatformIO and has been tested on an Arduino Nano ESP32. The program which includes this component will drive an I2C multiplexor and the multiplexor's output ports will be connected to one or more VL6180X ToF sensors.

This component implements two classes;-
- ```ToFSensor```. This class represents a sensor which can be connected to one of the I2C multiplexor ports.
- ```ToFThreshold```. This class represents a threshold which is associated with a sensor.

This component has the following dependencies;-
- LCC_NODE_COMPONENT_BASE: https://github.com/JohnCallingham/LCC_NODE_COMPONENT_BASE.git
- Adafruit_VL6180X: https://github.com/adafruit/Adafruit_VL6180X.git

The following software components are dependencies of one or more of the above components;-
- Adafruit BusIO: https://github.com/adafruit/Adafruit_BusIO
- Adafruit GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
- Adafruit SSD1306: https://github.com/adafruit/Adafruit_SSD1306



The PlatformIO Library Dependency Finder handles downloading all dependencies.

## Functions

The following functionality is implemented for each ToF sensor;-

## Class hierarchy
```mermaid
classDiagram
    LCC_Node_Component_Base <|-- ToFSensorBase
    ToFSensorBase <|-- ToFSensorVL53X0L
    ToFSensorBase <|-- ToFSensorVL6180

    class LCC_Node_Component_Base {
      uint16_t testStartEventIndex
      uint16_t testStopEventIndex
      bool testing
      unsigned long testingTimer

      void setSendEventCallbackFunction()
      void setTestStartEventIndex(uint16_t index)
      void setTestStopEventIndex(uint16_t index)

      virtual bool eventIndexMatches()
      virtual bool eventIndexMatchesCurrentState()
      virtual void sendEventsForCurrentState()

    }

    class ToFSensorBase {
      addThreshold()
      initialiseI2C()
      updateValueAndHysterisis()


    }

    class ToFSensorVL53X0L {
      -Adafruit_VL53L0X *vl
      +ToFSensorVL53L0X()
      +initialise()
      +read()
    }

    class ToFSensorVL6180 {
      -Adafruit_VL6180X *vl
      -ToFSensorVL6180()
      +initialise()
      +read()
    }
```


