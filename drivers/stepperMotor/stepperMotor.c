/*
 * stepperMotor.c
 *
 *  Created on: Feb 27, 2018
 *      Author: Sean Link
 *     Purpose: Store function definitions specific to the Stepper Motor
 */

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

#include "stepperMotor.h"

void setupStepperMotor(void) {
    // Turn on the peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

    // Wait for the peripheral to be turned on
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));

    // Configure the GPIO Pin
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, STEPPER_MOTOR_PINS);
}
