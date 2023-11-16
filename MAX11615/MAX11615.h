#ifndef __MAX11615_HH__
#define __MAX11615_HH__

#include "Arduino.h"
//used for 
#include <TeensyI2CDevice.h>
#include <TeensyI2CBusIORegister.h>

#define MAX11615_I2CADDR 0x33           // 0011 0011
/**************************************************
Setup Masks for MAX11615
***************************************************/
#define MAX11615_SETUP_MASK 0x80                // 1000 0000
#define MAX11615_SETUP_REFSEL_INPUT 0x20        // 0010 0000
#define MAX11615_SETUP_REFSEL_OUTPUT 0x70       // 0110 0000
#define MAX11615_SETUP_SETEXTERNAL_CLOCK 0x08   // 0000 1000
#define MAX11615_SETUP_SETINTERNAL_CLOCK 0x00   // 0000 0000 //set to 0 on startup by default
#define MAX11615_SETUP_SETUPBIPOLAR 0x06        // 0000 0100 //set to 0 on startupp by default

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
    VDD_REF_INT_CLK = 1,
    VDD_REF_EXT_CLK,
    REF_INP_INT_CLK,
    REF_OUT_INT_CLK
}MAX11615Config_t;

class MAX11615
{
    protected:
    TeensyI2CDevice* m_i2c_dev;
    public:
    MAX11615();
    bool begin(uint8_t _i2cBus);
    void init(MAX11615Config_t _c = MAX11615Config_t::VDD_REF_INT_CLK);
    uint16_t readADC_SingleEnded(uint8_t channel);
    uint16_t readADC_Differential_0_1();
    uint16_t readADC_Differential_2_3();
    uint16_t readADC_Differential_4_5();
    uint16_t readADC_Differential_6_7();
    uint16_t* readADC_Scan();
    float computeVolts(uint16_t counts);
    private:
    void writeRegister(uint8_t reg, uint16_t value);
    void writeRegister(uint8_t reg, uint8_t value);
    uint16_t readRegister(uint8_t reg);
    uint8_t _buf[3];
    uint8_t _inBuf[3];
    uint16_t _scanBuf[10];
};

#endif