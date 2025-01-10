//////////////////////////////////////////////////////////
///
//  Copyright (c) 2023
//  Author: Jacob Garner, mlgtex4127@gmail.com
//  
//  Filename: DaugterCard.cpp
//
//  Description:
//  This code is designed to integrate functionality of the MAX11615 chip manufactured
//  by Analog Devices/Maxim for the Teensy. I2C functionality is now supported for use with 
//  the Teensy 4.1 micro controller.
//
///
//////////////////////////////////////////////////////////

#include "MAX11615.h"

/// @brief Default Constructor
/// @param i2c_bus the I2C bus on which coms will be handled
/// @param GainPin Gain Pin that will be used
MAX11615::MAX11615(TwoWire* i2c_bus)
{
    this->m_i2c_dev = i2c_bus;
    this->setupByte  = 0b10001010;
    this->configByte = 0b01100001; //default config byte to be sent | no-scan, AIN0, Single-Ended
    this->addr = MAX11615_ADDR_DEFAULT;
}

/// @brief Initializer for the Daughter Card
/// @return true if card is initialized, false if not
bool MAX11615::begin()
{
    pinMode(this->gainPin, OUTPUT);
    digitalWrite(this->gainPin, LOW); //forces again to 10x
    //Try to write our config byte | check end transmission
    this->m_i2c_dev->begin();                           //Ensure the I2C Bus has begun
    this->m_i2c_dev->setClock(1000000);                 //Set our clock freq
    this->m_i2c_dev->beginTransmission(this->addr);     //Start Comms on I2C bus
    this->m_i2c_dev->write(this->setupByte);           //Write our config byte for something to send
    this->begun = !this->m_i2c_dev->endTransmission();  //endTransmission returns the 1 if data not sent but 0 if data was sent
    return this->begun;
}

/// @brief getter for the connection status of the chip
/// @return true if the chip is still connected/begun, false if not
bool MAX11615::getBegun()
{
    return this->begun;
}

/// @brief Sets the voltage reference based on the passed parameter
/// @param ref the desired voltage reference/pin config
void MAX11615::setReference(MAX11615_Reference_e ref)
{
    this->setupByte &= (uint8_t)MAX11615_REFERENCE_MASK;
    this->setupByte |= (uint8_t)ref;
    this->sendByte(this->setupByte);
}

/// @brief Sets the clock of the chip to either internal or external
/// @param useInternalClock if true, internal clock is used, if false, external clock is used
void MAX11615::setClock(bool useInternalClock)
{
    this->setupByte &= (uint8_t)MAX11615_CLK_MASK;
    this->setupByte |= useInternalClock ? (uint8_t)MAX11615_USE_INTERNAL_CLK : (uint8_t)MAX11615_USE_EXTERNAL_CLK ;
    this->sendByte(this->setupByte);
}

/// @brief Sets the polarity of the chip to bipolar or unipolar based on the parameter
/// @param bipolar bipolar == true, unipolar == false
void MAX11615::setPolarity(bool bipolar)
{
    this->setupByte &= (uint8_t)MAX11615_POLARITY_MASK;
    this->setupByte |= bipolar ? (uint8_t)MAX11615_BIPOLAR : (uint8_t)MAX11615_UNIPOLAR;
    this->sendByte(this->setupByte);
}

/// @brief Reads a selected channel in a single ended format
/// @param channel the desired channel to read
/// @return int16_t counds that is converted based on polarity
int16_t MAX11615::readADC_SingleEnded(MAX11615_Channel_e channel)
{
    
    this->configByte = 0b01100001;
    this->configByte |= (uint8_t)channel;
    if(!this->sendByte(this->configByte))
        return 0;
    uint16_t conversion = MAX11615::getLastConversion();
    //Now, if we are running in bipolar mode, we will need to use 2s compliment..
    if(!(this->setupByte & MAX11615_UNIPOLAR))
        return (int16_t)conversion;
    //if in bipolar mode, we have to check if we are above 2^11
    //if we have a value greater than 2047 then we have a negative value
    if(conversion > 2047)
        conversion |= 0xF000; //force the leading bit for the uint16 to be 1, then cast to an int
    return (int)conversion;

}

/// @brief Reads a set of channels in differential format
/// @param channel the desired channel to be the positive input
/// @return int16_t counds that is converted based on polarity
int16_t MAX11615::readADC_Differential(MAX11615_Channel_e channel)
{
    this->configByte = 0b01100000;
    this->configByte |= (uint8_t)channel;
    if(!this->sendByte(this->configByte))
        return 0;
    uint16_t conversion = MAX11615::getLastConversion();
    if(!(this->setupByte & MAX11615_UNIPOLAR))
        return (int16_t)conversion;
    //if in bipolar mode, we have to check if we are above 2^11
    //if we have a value greater than 2047 then we have a negative value
    if(conversion > 2047)
        conversion |= 0xF000; //force the leading bit for the uint16 to be 1, then cast to an int
    return (int)conversion;
}

uint16_t MAX11615::getLastConversion()
{
    this->m_i2c_dev->requestFrom(this->addr, (uint8_t)2);
    byte b1 = this->m_i2c_dev->read();
    byte b2 = this->m_i2c_dev->read();
    this->lastConversion = ((b1 & 0x0F) << 8) | b2;
    return this->lastConversion;
}

/// @brief Sends a single byte to our chip and updates our begun based on endTransmission status
/// @param data the byte to be sent
bool MAX11615::sendByte(uint8_t data)
{
    this->m_i2c_dev->beginTransmission(this->addr);
    this->m_i2c_dev->write(data);
    this->begun = !this->m_i2c_dev->endTransmission(); //ALWAYS update our begun status
    return this->begun;
}
