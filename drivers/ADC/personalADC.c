/*
 * adc.c
 *
 *  Created on: Mar 5, 2018
 *      Author: Sean Link
 *     Purpose: Store function definitions specific to the ADC Peripheral
 */


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "personalADC.h"

void setupADC(void) {

    // Turning power on to the peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Waiting for the peripheral to be fully turned on
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // Disabling the ADC to allow for configurations
    ADCSequenceDisable(ADC0_BASE, ADC_SEQUENCE_3);

    // Configuring the ADC to read from the ADC0_BASE, 1st sequence,
    // trigger processing, and to take priority
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQUENCE_3, ADC_TRIGGER_PROCESSOR, 0);

    // Configuring the ADCSequence to read from the 0 step
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCE_3, 0, ADC_CTL_CH7 |
                             ADC_CTL_IE | ADC_CTL_END);

    // Clear the ADC interrupt flag
    ADCIntClear(ADC0_BASE, ADC_SEQUENCE_3);

    // Enabling ADC functionality
    ADCSequenceEnable(ADC0_BASE, ADC_SEQUENCE_3);

}

void readADCData(uint32_t adcBuffer[]) {

    // Trigger the ADC conversion.
    ADCProcessorTrigger(ADC0_BASE, ADC_SEQUENCE_3);

    // Wait for conversion to be completed.
    while(!ADCIntStatus(ADC0_BASE, ADC_SEQUENCE_3, false))
    {
    }

    // Clear the ADC interrupt flag.
    ADCIntClear(ADC0_BASE, ADC_SEQUENCE_3);

    // Read ADC Value.
    ADCSequenceDataGet(ADC0_BASE, ADC_SEQUENCE_3, adcBuffer);

}
