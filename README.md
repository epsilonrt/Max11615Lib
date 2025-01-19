*MAX11612-MAX11617 ADC Library for Arduino*

[![Build](https://github.com/epsilonrt/Max11615Lib/actions/workflows/build.yml/badge.svg)](https://github.com/epsilonrt/Max11615Lib/actions/workflows/build.yml)
[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/epsilonrt/Max11615Lib?include_prereleases)](https://github.com/epsilonrt/Max11615Lib/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/epsilonrt/library/Max11615Lib.svg)](https://registry.platformio.org/libraries/epsilonrt/Max11615Lib)
[![Arduino Registry](https://www.ardu-badge.com/badge/Max11615Lib.svg)](https://www.arduinolibraries.info/libraries/Max11615Lib)  
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue)](https://www.arduino.cc/)

This library is designed to work with the MAX11612, MAX11613, MAX11614, MAX11615, MAX11616, MAX11617 12-bit ADC. 
It is designed to work with the Arduino platform.

Example usage:
```cpp
#include <MAX11615.h>

MAX11615 adc; // slave address is 0x33

void setup() {
  Serial.begin(115200);
  Wire.begin(); // can be called with arguments to specify the bus speed and the pins
  adc.begin(); // Initialize the ADC, default I2C bus is Wire, but another bus can be passed as an argument
}

void loop() {
  Serial.print(">AIN0:");
  Serial.println(adc.readSingleEnded(MAX11615::AIN0));
  delay(1000);
}
```

The library is designed to be easy to use and to be as close as possible to the datasheet. 
Another examples are provided in the examples folder.