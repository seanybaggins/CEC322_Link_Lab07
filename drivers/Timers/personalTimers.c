/*
 * personalTimers.c
 *
 * Created on: Mar 3, 2018
 * Author: Sean Link
 *
 * Purpose: Store functional definitions for timer functionalities
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "personalTimers.h"

// Purpose: Initialize the timer peripheral and various timers
void setupTimers(void) {
    // Turning on the timers
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);


    // Waiting for the peripherals to be fully turned on
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    // Disabling the timer functionalities to allow for configuration
    TimerDisable(TIMER0_BASE, TIMER_A);

    // Configuring the timer for periodic usage
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    //************************************************************************
    // Specifying how often the periodic timer will trigger
    //************************************************************************
    // First timer is to trigger every second
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

    //************************************************************************
    // Interrupts
    //************************************************************************

    // Creating existence of interrupts
    IntEnable(INT_TIMER0A);

    // Specifying when the timer interrupts trigger
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Registering the interrupt in the interrupt verctor table (IVT)
    TimerIntRegister(TIMER0_BASE, TIMER_A, oneSecondTimer);

    //************************************************************************
    //Enabling the Timer functionality now that configuration is finished.
    //************************************************************************
    TimerEnable(TIMER0_BASE, TIMER_A);
}
