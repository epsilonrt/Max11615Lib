//////////////////////////////////////////////////////////
///
//  Copyright (c) 2023
//  Author: Jacob Garner, mlgtex4127@gmail.com
//
//  Filename: MAX11615.cpp
//
//  Description:
//  This code is designed to integrate functionality of the MAX11615 chip manufactured
//  by Analog Devices/Maxim for the Teensy. I2C functionality is now supported for use with
//  the Teensy 4.1 micro controller.
//
///
//////////////////////////////////////////////////////////

#include "MAX11615.h"

// @brief Reads the last conversion from the chip
// @return uint16_t the last conversion value
// @note This function is private and should not be called outside of the class
uint16_t MAX11615::getLastConversion() {

  m_i2c_dev->requestFrom (m_slave_addr, (uint8_t) 2);
  byte b1 = m_i2c_dev->read();
  byte b2 = m_i2c_dev->read();
  m_last_conversion = ( (b1 & 0x0F) << 8) | b2;
  return m_last_conversion;
}

// @brief Sends a single byte to our chip and updates our begun based on endTransmission status
// @param data the byte to be sent
// @return true if the byte was sent, false if not
// @note This function is private and should not be called outside of the class
bool MAX11615::sendByte (uint8_t data) {

  m_i2c_dev->beginTransmission (m_slave_addr);
  m_i2c_dev->write (data);
  m_isconnected = !m_i2c_dev->endTransmission(); //ALWAYS update our begun status
  return m_isconnected;
}

//-----------------------------------------------------------------------------
MAX11615::MAX11615 (uint8_t slave_addr) :
  m_i2c_dev (nullptr),
  m_slave_addr (slave_addr),
  m_isconnected (false),
  m_setup_byte (0b10000010),
  m_config_byte (0b01100001),
  m_last_conversion (0) {}


//-----------------------------------------------------------------------------
bool MAX11615::begin (TwoWire &i2c_bus) {

  m_i2c_dev = &i2c_bus;                          // Set our I2C Bus
  // Try to write our config byte | check end transmission
  m_i2c_dev->beginTransmission (m_slave_addr);    // Start Comms on I2C bus
  m_i2c_dev->write (m_setup_byte);          // Write our config byte for something to send
  m_isconnected = !m_i2c_dev->endTransmission();  // endTransmission returns the 1 if data not sent but 0 if data was sent
  return m_isconnected;
}

//-----------------------------------------------------------------------------
bool MAX11615::isConnected() const {

  return m_isconnected;
}

//-----------------------------------------------------------------------------
void MAX11615::setReference (Reference ref) {

  m_setup_byte &= (uint8_t) MAX11615_REFERENCE_MASK;
  m_setup_byte |= (uint8_t) ref;
  sendByte (m_setup_byte);
}

//-----------------------------------------------------------------------------
void MAX11615::setClock (bool useInternalClock) {

  m_setup_byte &= (uint8_t) MAX11615_CLK_MASK;
  m_setup_byte |= useInternalClock ? (uint8_t) MAX11615_USE_INTERNAL_CLK : (uint8_t) MAX11615_USE_EXTERNAL_CLK ;
  sendByte (m_setup_byte);
}

//-----------------------------------------------------------------------------
void MAX11615::setPolarity (bool bipolar) {

  m_setup_byte &= (uint8_t) MAX11615_POLARITY_MASK;
  m_setup_byte |= bipolar ? (uint8_t) MAX11615_BIPOLAR : (uint8_t) MAX11615_UNIPOLAR;
  sendByte (m_setup_byte);
}

//-----------------------------------------------------------------------------
int16_t MAX11615::readSingleEnded (Channel channel) {

  m_config_byte = 0b01100001;
  m_config_byte |= (uint8_t) channel;
  if (!sendByte (m_config_byte)) {

    return INT16_MAX;
  }

  uint16_t conversion = getLastConversion();

  // Now, if we are running in bipolar mode, we will need to use 2s compliment..
  if (! (m_setup_byte & MAX11615_UNIPOLAR)) {

    return (int16_t) conversion;
  }

  // if in bipolar mode, we have to check if we are above 2^11
  // if we have a value greater than 2047 then we have a negative value
  if (conversion > 2047) {

    conversion |= 0xF000;  // force the leading bit for the uint16 to be 1, then cast to an int
  }
  return (int16_t) conversion;
}

//-----------------------------------------------------------------------------
int16_t MAX11615::readDifferential (Channel channel) {

  m_config_byte = 0b01100000;
  m_config_byte |= (uint8_t) channel;
  if (!sendByte (m_config_byte)) {

    return INT16_MAX;
  }

  uint16_t conversion = MAX11615::getLastConversion();
  if (! (m_setup_byte & MAX11615_UNIPOLAR)) {

    return (int16_t) conversion;
  }

  // if in bipolar mode, we have to check if we are above 2^11
  // if we have a value greater than 2047 then we have a negative value
  if (conversion > 2047) {

    conversion |= 0xF000;  //force the leading bit for the uint16 to be 1, then cast to an int
  }
  return (int16_t) conversion;
}

