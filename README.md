*MAX11615 ADC Library for Arduino*

This library is designed to work with the MAX11615 8-channel 12-bit ADC. It is designed to work with the Arduino platform.

Example usage:
```cpp
#include <MAX11615.h>

MAX11615 adc; // Create an instance of the MAX11615 class, default I2C bus is Wire, slave address is 0x33

void setup() {
  Serial.begin(9600);
  adc.begin(); // Initialize the ADC
}

void loop() {
  for (int i = 0; i < 8; i++) {
    Serial.print("Channel ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(adc.readADC_SingleEnded(i));
  }
  delay(1000);
}
```