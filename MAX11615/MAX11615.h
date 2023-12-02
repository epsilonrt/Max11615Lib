#ifndef __MAX11615_HH__
#define __MAX11615_HH__

#include "Arduino.h"
//used for 
#include <TeensyI2CDevice.h>
#include <TeensyI2CBusIORegister.h>
//#include <Wire.h>

#define MAX11615_I2CADDR 0x33           // 0011 0011
/**************************************************
Setup Masks for MAX11615
***************************************************/
#define MAX11615_SETUP_STD 0xA2                // 1010 0010 //Sets the standard mode of operation on the chip. Reference Data Sheet for what each bit means
#define MAX11615_SETUP_EXTCLK 0xAA             // 1010 1010 //Sets the internal clock to use the external clock (MIGH BE USEFUL FOR SPEED)
/**************************************************
Config Masks for MAX11615
***************************************************/
#define MAX11615_CONFIG_MASK 0x00               // 0000 0000
#define MAX11615_CONFIG_SIGNLESHOT 0x60         // 0110 0000 //per datasheet, setting bits 6 and 5 to 1 converts only the channel selected by CS3-CS0
//use bitwise or to assign values to the cs pins
#define MAX11615_CHANNELSEL_0 0x00              // 0000 0000
#define MAX11615_CHANNELSEL_1 0x02              // 0000 0010
#define MAX11615_CHANNELSEL_2 0x04              // 0000 0100
#define MAX11615_CHANNELSEL_3 0x06              // 0000 0110
#define MAX11615_CHANNELSEL_4 0x08              // 0000 1000
#define MAX11615_CHANNELSEL_5 0x0A              // 0000 1010
#define MAX11615_CHANNELSEL_6 0x0C              // 0000 1100
#define MAX11615_CHANNELSEL_7 0x0E              // 0000 1110
//Type of measurement
#define MAX11615_SINGLE_ENDED 0x01                // 0000 0001


/*!
 *  @brief used to select a setup configuration upon startup for the MAX11615
 */
typedef enum
{
    CONFIG_STD,
    CONFIG_EXTCLK
}MAX11615_config_t;

/*!
 *  @brief Standard obejct class leveraging the TeensyI2CDevice libraries
 */
class MAX11615
{
    public:
    MAX11615();
    bool begin(uint8_t _i2cBus);
    uint8_t configure(MAX11615_config_t _config = MAX11615_config_t::CONFIG_STD);
    bool getBegun();
    uint16_t readADC_SingleEnded(uint8_t channel);
    private:
    TeensyI2CDevice* m_i2c_dev;
    uint8_t addr;
    bool begun;
    bool configd = false;
    uint8_t buffer[3];
};

/*!
 *  @brief Standard obejct class leveraging the Wire.h library
 */
 /*
class MAX11615_W
{
    public:
    MAX11615_W();
    bool begin(uint8_t _i2cBus);
    bool getBegun();
    uint16_t readADC_SingleEnded(uint8_t channel);
    private:
    TwoWire* m_i2c_dev;
    uint8_t addr;
    bool begun;
}
*/
#endif