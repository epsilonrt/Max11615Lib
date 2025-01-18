#pragma once

//////////////////////////////////////////////////////////
///
//  Copyright (c) 2023
//  Author: Jacob Garner, mlgtex4127@gmail.com
//  
//  Filename: MAX11615.h
//
//  Description:
//  This code is designed to integrate functionality of the MAX11615 chip manufactured
//  by Analog Devices/Maxim for the Teensy. I2C functionality is now supported for use with 
//  the Teensy 4.1 micro controller.
//
///
//////////////////////////////////////////////////////////

#include "Arduino.h"
#include <Wire.h>

#define MAX11615_ADDR_DEFAULT 0x33

/*  Setup vs Config
    The setup byte is used to set global configurations
    The config byte is used to configure each measurment
*/
#define MAX11615_SETUP_MASK         0x7F    //Used to set our msb to 1 signalling a setup byte. We will &= our outgoing byte with this
#define MAX11615_CONFIG_MASK        0xFF    //Used to set our msb to 0 signalling a config byte.

#pragma region Setup_Byte_Foratters

/* Config Byte defined as follows
    Bit | Name      | Description
    7   | REG       | Register bit | 1 == setup byte, 0 == config byte
    6   | SEL2      | --
    5   | SEL1      | Select the reference for the chip, defaults to 000 at power up, see defines or docs for more info
    4   | SEL0      | --
    3   | CLK       | 1 = external clock, 0 = internal clock, defaults to 0
    2   | BIP/UNI   | 1 = bipolar, 0 = unipolar, defaults to 0
    1   | RST       | 1 = no action, 0 = resets configuration register to default, but setup register is unchanged
    0   | No Care   | Dont Care, set it to whatever youd like
*/
#define MAX11615_REFERENCE_MASK     0x8F    //Sets bits 6-4 to 000 via &= so that the bits can be set via |= using the following macros
#define MAX11615_REFERENCE_VDD      0x00    //Sets bits 6-4 to 00X REF==VDD | AIN_/REF==ANALOG INPUT        | REF==N/C | INTERNAL REF STATE == ALWAYS OFF
#define MAX11615_REFERENCE_EXT      0x20    //Sets bits 6-4 to 01X REF==EXT | AIN_/REF==REFERECNE INPUT     | REF==INP | INTERNAL REF STATE == ALWAYS OFF
#define MAX11615_REFERENCE_INT1     0x40    //Sets bits 6-4 to 100 REF==INT | AIN_/REF==ANALOG INPUT        | REF==N/C | INTERNAL REF STATE == ALWAYS OFF
#define MAX11615_REFERENCE_INT2     0x50    //Sets bits 6-4 to 101 REF==INT | AIN_/REF==ANALOG INPUT        | REF==N/C | INTERNAL REF STATE == ALWAYS ON
#define MAX11615_REFERENCE_INT3     0x60    //Sets bits 6-4 to 110 REF==INT | AIN_/REF==REFERENCE OUTPUT    | REF==OUT | INTERNAL REF STATE == ALWAYS OFF
#define MAX11615_REFERENCE_INT4     0x70    //Sets bits 6-4 to 111 REF==INT | AIN_/REF==REFERENCE OUTPUT    | REF==OUT | INTERNAL REF STATE == ALWAYS ON

#define MAX11615_CLK_MASK           0xF7    //Sets bit 3 to 0 via &= so that the bit can be set via |= using the following macros
#define MAX11615_USE_INTERNAL_CLK   0x00    //Sets bit 3 to 0 to use the internal clock
#define MAX11615_USE_EXTERNAL_CLK   0x08    //Sets bit 3 to 1 to use an external clock

#define MAX11615_POLARITY_MASK      0xFB    //Sets bit 2 to 0 via &= so that the bit can be set via |= using the following macros
#define MAX11615_BIPOLAR            0x02    //Sets bit 2 to 1 configuring bipolar measurements
#define MAX11615_UNIPOLAR           0x00    //Sets bit 2 to 0 configuring unipolar measurements

#pragma endregion Setup_Byte_Foratters


#pragma region Config_Byte_Formatters
/* Setup Byte defined as follows
    
    Bit | Name      | Description
    7   | REG       | Register bit | 1 == setup byte, 0 == config byte
    6   | SCAN1     | Scan bits, see defines or docs for description
    5   | SCAN2     | --
    4   | CS3       | --
    3   | CS2       | Channel select pins 
    2   | CS1       | See defines or docs for description
    1   | CS0       |--
    0   | SGL\DIF   | 1 == single ended, 0 == differential
*/
/*Scan Bit Defines*/
#define MAX11615_SCANBIT_MASK       0x9F    //Used to reset bits 6 and 5 to 0 expands to 0b10011111
#define MAX11615_SCAN_TO_PIN        0x00    //When Bits 6 and 5 are set to 0, then a scan is performed from AIN0 to AINX where x is set by bits 1-4
#define MAX11615_SCAN_REPEAT        0x20    //Scan bits ..01.. converts just the pin selected by bits 1-4 eight times
#define MAX11615_SCAN_SUBSET        0x40    //Scan bits ..10.. complets a scan of the upper quartile of channes | We do not use this but the docs explain this in more detail
#define MAX11615_SCAN_OFF           0x60    //Scan bits ..11.. only converts the selected channel by bits 1-4

/*Channel Defines for single ended and differential measures*/
#define MAX11615_CHANNEL_MASK       0xE0    //Used to reset bits 4-1 to 0 | Expands to 0b11100000
#define MAX11615_SINGLEENDED_MEAS   0x01    //Sets bit 0 to 1 to force a single eneded measurement
#define MAX11615_DIFFERENTIAL_MEAS  0x00    //Sets bit 0 to 0 to force a differential measurement
#define MAX11615_CHANNEL_AIN0       0x00    //Sets Channel to AIN0  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN1       0x02    //Sets Channel to AIN1  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN2       0x04    //Sets Channel to AIN2  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN3       0x06    //Sets Channel to AIN3  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN4       0x08    //Sets Channel to AIN4  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN5       0x0A    //Sets Channel to AIN5  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN6       0x0C    //Sets Channel to AIN6  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN7       0x0E    //Sets Channel to AIN7  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN8       0x10    //Sets Channel to AIN8  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN9       0x12    //Sets Channel to AIN9  | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN10      0x14    //Sets Channel to AIN10 | Reference is set via bit 0
#define MAX11615_CHANNEL_AIN11      0x16    //Sets Channel to AIN11 | Reference is set via bit 0



/*
//I am super indecisive... so below are the explicity channel calls, I decided to move to just setting the cs bits and changing differential or single ended in function calls...
#define MAX11615_CHANNEL_AIN0_GND   0x01    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN1_GND   0x03    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN2_GND   0x05    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN3_GND   0x07    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN4_GND   0x09    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN5_GND   0x0B    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN6_GND   0x0D    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN7_GND   0x0F    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN8_GND   0x11    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN9_GND   0x13    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN10_GND  0x15    //Sets channel to single ended AIN0 vs GND
#define MAX11615_CHANNEL_AIN11_GND  0x17    //Sets channel to single ended AIN0 vs GND

#define MAX11615_CHANNEL_AIN0_AIN1   0x00    //Sets Channel to differential AIN0  (+) vs AIN1  (-)
#define MAX11615_CHANNEL_AIN1_AIN0   0x00    //Sets Channel to differential AIN1  (+) vs AIN0  (-)
#define MAX11615_CHANNEL_AIN2_AIN3   0x00    //Sets Channel to differential AIN2  (+) vs AIN3  (-)
#define MAX11615_CHANNEL_AIN3_AIN2   0x00    //Sets Channel to differential AIN3  (+) vs AIN2  (-)
#define MAX11615_CHANNEL_AIN4_AIN5   0x00    //Sets Channel to differential AIN4  (+) vs AIN5  (-)
#define MAX11615_CHANNEL_AIN5_AIN4   0x00    //Sets Channel to differential AIN5  (+) vs AIN4  (-)
#define MAX11615_CHANNEL_AIN6_AIN7   0x00    //Sets Channel to differential AIN6  (+) vs AIN7  (-)
#define MAX11615_CHANNEL_AIN7_AIN6   0x00    //Sets Channel to differential AIN7  (+) vs AIN6  (-)
#define MAX11615_CHANNEL_AIN8_AIN9   0x00    //Sets Channel to differential AIN8  (+) vs AIN9  (-)
#define MAX11615_CHANNEL_AIN9_AIN8   0x00    //Sets Channel to differential AIN9  (+) vs AIN8  (-)
#define MAX11615_CHANNEL_AIN10_AIN11 0x00    //Sets Channel to differential AIN10 (+) vs AIN11 (-)
#define MAX11615_CHANNEL_AIN11_AIN10 0x00    //Sets Channel to differential AIN11 (+) vs AIN10 (-)
*/

#pragma Config_Byte_Formatters

/// @brief Selection Enum for Channel to measure
typedef enum
{
    MAX11615_AIN0  = MAX11615_CHANNEL_AIN0,
    MAX11615_AIN1  = MAX11615_CHANNEL_AIN1,
    MAX11615_AIN2  = MAX11615_CHANNEL_AIN2,
    MAX11615_AIN3  = MAX11615_CHANNEL_AIN3,
    MAX11615_AIN4  = MAX11615_CHANNEL_AIN4,
    MAX11615_AIN5  = MAX11615_CHANNEL_AIN5,
    MAX11615_AIN6  = MAX11615_CHANNEL_AIN6,
    MAX11615_AIN7  = MAX11615_CHANNEL_AIN7,
    MAX11615_AIN8  = MAX11615_CHANNEL_AIN8,
    MAX11615_AIN9  = MAX11615_CHANNEL_AIN9,
    MAX11615_AIN10 = MAX11615_CHANNEL_AIN10,
    MAX11615_AIN11 = MAX11615_CHANNEL_AIN11
} MAX11615_Channel_e;

/// @brief Selection Enum for Reference type
typedef enum
{
    MAX11615_REF_VDD  = MAX11615_REFERENCE_VDD,
    MAX11615_REF_EXT  = MAX11615_REFERENCE_EXT,
    MAX11615_REF_INT1 = MAX11615_REFERENCE_INT1,
    MAX11615_REF_INT2 = MAX11615_REFERENCE_INT2,
    MAX11615_REF_INT3 = MAX11615_REFERENCE_INT3,
    MAX11615_REF_INT4 = MAX11615_REFERENCE_INT4,
} MAX11615_Reference_e;

class MAX11615
{
    public:
    MAX11615(TwoWire* i2c_bus = &Wire);
    bool begin();
    void setReference(MAX11615_Reference_e ref = MAX11615_Reference_e::MAX11615_REF_VDD);
    void setClock(bool useInternalClock);
    void setPolarity(bool bipolar);
    bool getBegun();

    int16_t readADC_SingleEnded(MAX11615_Channel_e channel = MAX11615_Channel_e::MAX11615_AIN0);
    int16_t readADC_Differential(MAX11615_Channel_e channel = MAX11615_Channel_e::MAX11615_AIN0);

    private:
    TwoWire* m_i2c_dev;
    uint8_t addr;
    bool begun;
    int gainPin;
    uint8_t setupByte;
    uint8_t configByte;
    uint16_t lastConversion = 0;
    bool sendByte(uint8_t data);
    uint16_t getLastConversion();
};
