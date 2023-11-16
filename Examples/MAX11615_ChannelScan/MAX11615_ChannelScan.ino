/////////////////////////////////////////////
///
//  Copyright 2023 Rad Source Technologies, Inc
//
//  Author: Jacob Garner; jgarner@radsource.com
//  Date: 11/01/2023
///
/////////////////////////////////////////////

#include <MAX11615.h>

MAX11615 ads;
//MAX11615* sercher;
uint8_t _led;
unsigned long _startTime = 0;
unsigned long _cycleTime = 50;

uint16_t readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() 
{
  Serial.begin(115200);
  Serial.println("Initializing Micro Controller");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for(uint8_t i = 0; i < 3; i++)
  {
    MAX11615* searcher = new MAX11615();
    if(searcher->begin(i))
    {
      Serial.println("MAX11615 Device found on Serial Bus: " + String(i) + " at Address: " + MAX11615_I2CADDR);
    }
  }
  Serial.println("Starting ADS object for comunication with device on I2C bus 0");
  if(!ads.begin(0))
  {
    Serial.println("Error, MAX11615 not found on bus 0");
    while(1)
    {
      delay(200);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
  ads.init();
  Serial.println("MAX11615 started! Proceeding with scan of all channels");

}

void loop() 
{
  _startTime = millis();
  for(uint8_t i = 0; i < 8; i++)
  {
    readings[i] = ads.readADC_SingleEnded(i);
  }
  printData_f(readings, sizeof(readings)/sizeof(readings[0]));
  do
  {

  }while (millis() - _startTime < _cycleTime);
  _led++;
}


void printData_f(uint16_t* arr, size_t len)
{
  Serial.println("*************** New Data ***************");
  for(size_t i = 0; i < len; i++)
  {
    Serial.println("Channel " + String(i) + " reading (counts): " + String(readings[i]));
  }
}
