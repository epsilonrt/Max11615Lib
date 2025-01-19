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