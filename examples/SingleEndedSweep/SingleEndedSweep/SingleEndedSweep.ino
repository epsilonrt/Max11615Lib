// Single ended sweep demo
// This example demonstrates how to read all four single-ended channels of the MAX11615 ADC.
// This example is in the public domain
#include <MAX11615.h>

// Our ADC object
MAX11615 adc;

// Change this to the number of channels you want to sweep through, if you change this you will need to update the channels array
const int NumChannels = 4;
// Channels to sweep through
const MAX11615::Channel channels[NumChannels] = {MAX11615::AIN0, MAX11615::AIN1, MAX11615::AIN2, MAX11615::AIN3};
// Channel names for printing, these labels were compatible with the VS Code Teleplot extension.
const char *channel_names[NumChannels] = {">AIN0:", ">AIN1:", ">AIN2:", ">AIN3:"};

void setup() {
  Serial.begin (115200);
  Serial.println ("Starting MAX11615 Single ended sweep demo");

  Wire.begin();
  // Start our object
  if (!adc.begin (Wire)) {
    Serial.println ("Did not find MAX11615 on specified I2C bus.\nPlease check wiring.");
    while (1)
      ;
  }
  // Configure the chip;
  adc.setClock (true);       //true == Internal Clock  | False == External Clock
  adc.setPolarity (false);   //true == Bipolar         | False == Unipolar
  adc.setReference (MAX11615::REF_VDD);
  delay (100);
  Serial.println ("Single-Ended Measurements");
  Serial.println ("------------------------");
}

void loop() {

  for (int i = 0; i < NumChannels; i++) {
    // Read the current channel
    int16_t reading = adc.readSingleEnded (channels[i]);

    if (reading == INT16_MAX) {
      // If we get INT16_MAX, there was an error
      Serial.print ("Error reading channel ");
      Serial.println (i);
    }
    else {
      // else print the reading
      Serial.print (channel_names[i]);
      Serial.println (reading);
    }
  }
  delay (1000);
}
