#include "MAX11615.h"


/**********************************************************

MAX11615 Methods Using TeensyI2CDevice libraries

***********************************************************/

/*!
 *  @brief Default Constructor
 */
MAX11615::MAX11615()
{
    MAX11615::addr = MAX11615_I2CADDR;
}

/*!
 *  @brief Starts the I2C Bus communications on the appropriate Address
 *  @param _i2cBus the deisred I2C Bus on which we the unit will communicate
 *  @returns True if I2C Coms started properly; False if not.
 */
bool MAX11615::begin(uint8_t _i2cBus)
{
    if(_i2cBus > 3)return false;
    m_i2c_dev = new TeensyI2CDevice( MAX11615_I2CADDR,_i2cBus);
    MAX11615::configure(); //set the configureation before taking 
    return m_i2c_dev->begin();
}

/*!
 *  @brief Getter for the status of the ADC chip
 */
bool MAX11615::getBegun()
{
    return MAX11615::begun;
}

/*!
 *  @brief reads the buffer for the selected channel
 *  @param channel the channel to be measured (between 0 and 11)
 *  @returns 12 bit value corresponding to the voltage measured on the channel parameter
 */
uint16_t MAX11615::readADC_SingleEnded(uint8_t channel)
{
    if(channel > 11) return 9999; //signals to the user that the channel selected is incorrect
    uint8_t data = (uint8_t)MAX11615_CONFIG_SIGNLESHOT;
    data |= channel << 1;
    MAX11615::m_i2c_dev->write(&data, 1);
    MAX11615::m_i2c_dev->read(buffer, 2);
    return ((buffer[0] & 0b00001111) << 8) | buffer[1];
}

/*!
 *  @brief Sets a desired configuration for the chip
 *  @param _config the desired configuration based on an enumerated value | Default is standard config (0xA2)
 *  @returns byte corresponding to the value selected and written to the I2C Device
 */
uint8_t MAX11615::configure(MAX11615_config_t _config)
{
    uint8_t _cmd = 0b00000000;
    switch (_config)
    {
        case MAX11615_config_t::CONFIG_STD:
        _cmd |= MAX11615_SETUP_STD;
        break;
        case MAX11615_config_t::CONFIG_EXTCLK:
        _cmd |= MAX11615_SETUP_EXTCLK;
        break;
        default:
        _cmd |= MAX11615_SETUP_STD;
        break;
    }
    m_i2c_dev->write(&_cmd, 1);
    return _cmd;
}