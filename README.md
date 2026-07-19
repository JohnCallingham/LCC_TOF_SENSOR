# LCC_TOF_SENSOR
This component is part of a suite of components which can be used as part of a program which implements an OpenLCB/LCC node. It has been developed using PlatformIO and has been tested on an Arduino Nano ESP32.

The files tof_sensor.h and tof_sensor.cpp implement two classes;-
- class ```ToFSensor```
- class ```ToFThreshold```

This component has the following dependencies;-
- LCC_NODE_COMPONENT_BASE: https://github.com/JohnCallingham/LCC_NODE_COMPONENT_BASE.git
- Adafruit_VL53L0X: https://github.com/adafruit/Adafruit_VL53L0X.git
- Adafruit_VL6180X: https://github.com/adafruit/Adafruit_VL6180X.git

These will be automatically downloaded by PlatformIO's Library Dependency Finder.

## Functions

The following functionality is implemented for each ToF sensor;-


