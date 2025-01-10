#include <Arduino.h>
#include "MAX11615.h"

MAX11615* adc;

/// @brief Sweep through channels 1-4 in singled ended mode with filtering over 1 second
void singleEndedSweep();
/// @brief Sweep through channels 1-4 in differential mode with filtering over 1 second
void differentialSweep();

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  Serial.println("Starting MAX11615 ADC Tester");
  //Create our new object
  adc = new MAX11615(&Wire);
  //Start our object
  if(!adc->begin())
  {
    Serial.println("Did not find MAX11615 on specified I2C bus.\nPlease check wiring.");
    while(1)
      ;
  }
  //Configure the chip;
  adc->setClock(true);        //true == Internal Clock  | False == External Clock
  adc->setPolarity(false);    //true == Bipolar         | False == Unipolar
  adc->setReference(MAX11615_REF_VDD);
  delay(100);
  Serial.println("Moving to Measurements!");
}

void loop() 
{
  singleEndedSweep();
  //differentialSweep();
  delay(1000);
  digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN));
}

void singleEndedSweep()
{
  float readings[4] = {};
  readings[0] = adc->readADC_SingleEnded(MAX11615_AIN0);
  readings[1] = adc->readADC_SingleEnded(MAX11615_AIN1);
  readings[2] = adc->readADC_SingleEnded(MAX11615_AIN2);
  readings[3] = adc->readADC_SingleEnded(MAX11615_AIN3);
  Serial.printf("Single-Ended Measurements\nAIN0: %.4f\nAIN1: %.4f\nAIN2: %.4f\nAIN3: %.4f\n", readings[0], readings[1], readings[2], readings[3]);
}

void differentialSweep()
{
  float readings[4] = {};
  readings[0] = adc->readADC_Differential(MAX11615_AIN0);
  readings[1] = adc->readADC_Differential(MAX11615_AIN1);
  readings[2] = adc->readADC_Differential(MAX11615_AIN2);
  readings[3] = adc->readADC_Differential(MAX11615_AIN3);
  
  Serial.printf("Differential Measurements\nAIN0: %.4f\nAIN1: %.4f\nAIN2: %.4f\nAIN3: %.4f\n", readings[0], readings[1], readings[2], readings[3]);
}