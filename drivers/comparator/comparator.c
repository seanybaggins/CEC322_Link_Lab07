/*
 * comparator.c
 *
 *  Created on: Feb 20, 2018
 *      Author: Sean Link and Andrew Hostetter
 */
#include "driverlib/comp.c"
#include "driverlib/comp.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "comparator.h"

// Purpose: Turn on and fully configure the comparator.
void setupComparator(void) {
    // Turn on peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_COMP0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Wait for peripheral to be turn on
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_COMP0) ||
                !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));

    // Configuring the internal reference for the comparator to 1.65V
    ComparatorRefSet(COMP_BASE, COMP_REF_1_65V);

    // Configuring comparator
    ComparatorConfigure(COMP_BASE, 0, COMP_TRIG_NONE | COMP_INT_BOTH |
                        COMP_ASRCP_REF | COMP_OUTPUT_NORMAL);

    // Assigning pin for comparator
    GPIOPinTypeComparator(GPIO_PORTC_BASE, GPIO_PIN_7);

    // Enabling interrupts
    ComparatorIntRegister(COMP_BASE, 0, IntComp0);
    ComparatorIntEnable(COMP_BASE, 0);
    ComparatorIntClear(COMP_BASE, 0);
}

