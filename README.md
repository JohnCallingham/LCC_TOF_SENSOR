# LCC_TOF_SENSOR
This component is part of a suite of components which can be used as part of a program which implements an OpenLCB/LCC node. It has been developed using PlatformIO and has been tested on an Arduino Nano ESP32.

This component implements two classes;-
- ```ToFSensor```. This class represents one of the four sensors which can be connected.
- ```ToFThreshold```. This class represents one of the four thresholds for a sensor.

This component has the following dependencies;-
- LCC_NODE_COMPONENT_BASE: https://github.com/JohnCallingham/LCC_NODE_COMPONENT_BASE.git
- Adafruit_VL6180X: https://github.com/adafruit/Adafruit_VL6180X.git

These will be automatically downloaded by PlatformIO's Library Dependency Finder.

## Functions

The following functionality is implemented for each ToF sensor;-


