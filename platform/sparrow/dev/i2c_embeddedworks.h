#ifndef _I2C_H
#define _I2C_H

#include <avr/io.h>
#include "stdutils.h"

void I2C_Init();
void I2C_Start();
void I2C_Stop(void);
void I2C_Write(uint8_t v_i2cData_u8);
uint8_t I2C_Read(uint8_t v_ackOption_u8);
/**************************************************************************************************/

#endif
