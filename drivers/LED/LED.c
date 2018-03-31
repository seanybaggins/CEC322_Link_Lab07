/*
 * LED.c
 *
 *  Created on: Feb 26, 2018
 *      Author: Sean Link
 *     Purpose: Store function definitions specific to the LED
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#include "LED.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


void setupLED(void) {
    // Turn on the peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);

    // Wait for the peripheral to be turned on
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG));

    // Enable the GPIO pin for the LED (PG2).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
}
