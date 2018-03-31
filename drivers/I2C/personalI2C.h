/*
 * personalI2C.h
 *
 *  Created on: Mar 20, 2018
 *      Author: Sean Link
 *     Purpose: Store the definitions and function prototypes for the I2C
 */

#ifndef DRIVERS_I2C_PERSONALI2C_H_
#define DRIVERS_I2C_PERSONALI2C_H_

#define MAX_I2C_VALUE 4095
#define SIN_I2C_SCALING_FACTOR 2047

typedef enum {
    SAWTOOTH = 0, SINE = 1, SQUARE = 2, TANGENT = 3
} WaveFormSate;

void setupI2C(void);
static unsigned long ResetI2C(uint32_t i2c_base);
static unsigned long WaitI2CDone(unsigned int long ulBase);
int i2c_write(uint32_t i2c_base_addr,
              unsigned char dev_addr,
              unsigned num_char,
              unsigned char *write_buf);



#endif /* DRIVERS_I2C_PERSONALI2C_H_ */
