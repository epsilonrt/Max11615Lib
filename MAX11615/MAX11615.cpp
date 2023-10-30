#include "MAX11615.h"


MAX11615::MAX11615()
{

}

/*!
 *  @brief Starts the I2C Bus communications on the appropriate Address
 *  @returns True if I2C Coms started properly; False if not.
 */
bool MAX11615::begin(uint8_t _i2cBus)
{
    if(_i2cBus > 3)return;
    m_i2c_dev = new TeensyI2CDevice( MAX11615_I2CADDR,_i2cBus);
    return m_i2c_dev->begin();
}

/*!
 *  @brief Reads the counts measured for a specified channel from the MAX11615 Device
 *  @param channel the channel from which we will be measuring
 *  @returns the value (12 bits) measured on the specific channel
 */
uint16_t MAX11615::readADC_SingleEnded(uint8_t channel)
{
    //uint8_t outBuf[2]; //based on the datasheet, we only need 2 bytes to send/receive data
    //build the outgoing command sequence
    uint8_t config = 0x00; //this register = 0, no scan bits; now we just need to set bits 4 - 0
    if(channel > 7) return 0;
    config |= channel << 1; //due to the channel identifier, we can just shift here channel 1 is 0001 and bitshifted is 0010 which is the same as the channel select mask
    //removed switch statement in favor of the above channel selection
    config |= 0x01; //ensures that the sgl/dif bit is set to 1 
    //based on the documentation, we just send this 
    _buf [0] = config;
    m_i2c_dev->write(config, 1);
    //we might need a write then read...
    //m_i2c_dev->write_then_read(config, 1, inData, 2);
    //now read the register to see what we have
    m_i2c_dev->read(_inBuf, 2);
    return ((_inBuf[0] << 8) | _inBuf[1]);
}

/*!
 *  @brief writes a specified set of commands/setup/configuration bytes to the I2C Device
 *  @param reg the register to read/write from
 *  @param value the value being written to the chip; This is split into 2 seperate bytes
 */
void MAX11615::writeRegister(uint8_t reg, uint16_t value)
{
    _buf[0] = reg;
    _buf[1] = value >> 8;
    _buf[2] = value & 0xFF;
    m_i2c_dev->write(_buf, 3);
}

/*!
 *  @brief writes a specified command/setup/configuration byte to the I2C Device
 *  @param reg the register to read/write from
 *  @param value the value being written to the chip; this is a single byte
 */
void MAX11615::writeRegister(uint8_t reg, uint8_t value)
{
    _buf[0] = reg;
    _buf[1] = value;
    m_i2c_dev->write(_buf, 2);
}

/*!
 *  @brief Reads and returns the value from a specified register
 *  @param reg the register to be read from, this should 
 *  @returns uint16_t value recieved by the I2C Device
 */
uint16_t MAX11615::readRegister(uint8_t reg)
{
    _buf[0] = reg;
    m_i2c_dev->write(_buf, 1); //not sure if we need this for the MAX11615.. will need to test with the chip
    m_i2c_dev->read(_buf, 2); //we are recieving 16 bits, so 2 bytes should store it all
    return ((_buf[1] >> 8) | _buf[2]);
}

uint16_t MAX11615::readADC_Differential(uint8_t channel)
{   
    /*feature not turned on yet*/
    return 0;
}

/*!
 *  @brief computes the voltage the corresponds to a passed count parameter
 *  @param counts measured counts that need to be converted, should be 12 bit value
 *  @param FSR the Full Scale Range of the chip, should be 3.3 Volts but could vary
 *  @returns float value of the voltage based on the counts passed as a parameter
 */
float MAX11615::computeVolts(uint16_t counts, float FSR)
{
    //This is a 12 bit chip, use this for the conversions
    return counts * FSR/4096.0;
}
