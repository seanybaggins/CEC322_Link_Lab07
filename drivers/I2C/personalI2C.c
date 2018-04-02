/*
 * personalI2C.c
 *
 *  Created on: Mar 20, 2018
 *      Author: Sean Link and Chloe Mena
 *     Purpose: Store the function definitions for the I2C peripheral
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Necessary for INT_MAX
#include <limits.h>

#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

void setupI2C(void) {
    // Turning power on to the peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Wait for the peripheral to be fully turned on
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2) ||
            !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Reseting peripheral
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);
    SysCtlDelay(5);

    // Selecting GPIO PA6 and PA7 pins for i2c communication
    GPIOPinConfigure(GPIO_PF6_I2C2SCL);
    GPIOPinConfigure(GPIO_PF7_I2C2SDA);
    SysCtlDelay(5);


    // Configuring the I2C to the GPIO Pins
    GPIOPinTypeI2CSCL(GPIO_PORTF_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTF_BASE, GPIO_PIN_7);

    // Disabling peripheral functionality to allow configuration
    I2CMasterDisable(I2C2_BASE);

    // Initializing Master and Slave
    I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), true);

    // Allowing peripheral functionality
    I2CMasterEnable(I2C2_BASE);
}

//*****************************************************************************
// Code provided by Dr. Davis
//*****************************************************************************
static unsigned long ResetI2C(uint32_t i2c_base){

    SysCtlPeripheralReset(i2c_base);

    SysCtlDelay(40);

    // while (! SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0)) {}

    return I2CMasterErr(i2c_base);
}

//**************************************************************************
//
// WaitI2CDone()
//
// Arguments:
//   ulBase -- > the base address of the i2c peripheral being used
//               i.e. I2C2_BASE
//*****************************************************************************
static unsigned long WaitI2CDone(unsigned int long ulBase){
    const unsigned threshold = (UINT_MAX-10);
    unsigned ctr = 0;
    // Wait until done transmitting
    while(I2CMasterBusy(ulBase))
    {
      ctr++;
      if (ctr >= threshold)
      {
          ResetI2C(ulBase);
          break;
      }
    }
    // Return I2C error code
    return I2CMasterErr(ulBase);
}

//*****************************************************************************
//
// I2C_write()
//
// Arguments:
//   i2c_base_address -- > the base address of the i2c peripheral being used
//                         i.e. I2C2_BASE
//   dev_addr --> the I2C address of the device being written to. This value
//                should always be in the range 0x1 - 0x7F, or 0x0 for broadcast.
//   num_char --> the number of characters to be written to the I2C bus
//                exclusive of the address byte.  A call to i2c_write with
//                num_char = 4 will result in 5 bytes total being written
//                to the bus, the address byte and 4 data bytes.
//   write_buf --> a pointer to the bytes to be written to the I2C bus.
//                 this buffer must contain at least bytes num_char of data.
//                 Any buffer data beyond num_char bytes will be ignored.
//*****************************************************************************
int i2c_write(uint32_t i2c_base_addr,
              unsigned char dev_addr,
              unsigned num_char,
              unsigned char *write_buf)
{
  int rv = 0;
  if (num_char == 0)
    return rv;

  // Make sure
  WaitI2CDone(i2c_base_addr);
  if (num_char == 1)
  {
    // Set the slave address
    // recieve = false
    I2CMasterSlaveAddrSet(i2c_base_addr, dev_addr , false);
    //
    I2CMasterDataPut(i2c_base_addr, write_buf[0]);

    // Send the command to initiate the read
    I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_SINGLE_SEND);
    // Wait for that transmission to finish
    WaitI2CDone(i2c_base_addr);
  }
  else
  {
    int burst_cont = (num_char - 2);
    int index = 0;

    // Set the slave address
    // recieve = false
    I2CMasterSlaveAddrSet(i2c_base_addr, dev_addr , false);
    // Tell the slave to start reading
    I2CMasterDataPut(i2c_base_addr, write_buf[index++]);
    // Start Burst
    I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_BURST_SEND_START);
    // Wait for that transmission to finish
    WaitI2CDone(i2c_base_addr);

    while (burst_cont > 0)
    {
      // Tell the slave to start reading
      I2CMasterDataPut(i2c_base_addr, write_buf[index++]);
      // Continue Burst
      I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_BURST_SEND_CONT);
      // Wait for that transmission to finish
      WaitI2CDone(i2c_base_addr);
      burst_cont--;
    }

    // Tell the slave to start reading
    I2CMasterDataPut(i2c_base_addr, write_buf[index++]);
    // Finish Burst
    I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_BURST_SEND_FINISH);
    // Wait for that transmission to finish
    WaitI2CDone(i2c_base_addr);
  } // case num_char > 1

  return(rv);
}
