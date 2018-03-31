/*
 * Lab7_I2C.c
 *
 * Class: CEC322
 * University: ERAU - Prescott
 * Authors: Sean Link
 * Date: 3/20/2018
 *
 */

// Base includes with the timers Examples
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "grlib/grlib.h"
#include "drivers/cfal96x64x16.h"

// Necessary for blinking LED
#include "drivers/LED/LED.h"

// Necessary for lower case string conversion
#include <ctype.h>

// Necessary for blinking LED
#include "driverlib/gpio.h"

// Necessary for printing to the OLED
#include "drivers/OLED/displays.h"

// Necessary for the functionality of the UART
#include "drivers/UART/personalUART.h"

// Necessary for timers
#include "drivers/Timers/personalTimers.h"
#include "driverlib/timer.h"

// Necessary for the I2C
#include "drivers/I2C/personalI2C.h"

//Necessary for the Sine Waveform
#include <math.h>

//****************************************************************************
// Defines
//****************************************************************************
// Necessary for Sine Function
#define PI 3.14159265

//****************************************************************************
// Globals
//****************************************************************************
tContext sContext;
uint8_t menuSelection = '\0';

//****************************************************************************
// Interrupts
//****************************************************************************

void oneSecondTimer(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    static uint32_t timerCount = 0;
 //   clearBlack();

    timerCount++;
    //displayInfoOnBoard("%d", timerCount, 25, DISPLAY_NUMBER);
}

void IntUART0(void) {
    uint32_t ui32Status;

    // Get the interrupt status.
    ui32Status = UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    UARTIntClear(UART0_BASE, ui32Status);

    // Get the character from the UART buffer
    while(UARTCharsAvail(UART0_BASE)) {
        menuSelection = tolower((uint8_t)UARTCharGetNonBlocking(UART0_BASE));
    }
}

//****************************************************************************
// Main function
//****************************************************************************
int
main(void)
{

    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Initialize the display driver.
    CFAL96x64x16Init();

    // Disabling all Interrupts
    IntMasterDisable();

    // Initialize the graphics context and find the middle X coordinate.
    GrContextInit(&sContext, &g_sCFAL96x64x16);
    GrContextFontSet(&sContext, g_psFontFixed6x8);

    //*************************************************************************
    // Configuration
    //*************************************************************************
    setupUART();

    setupLED();

    setupTimers();

    setupI2C();

    //************************************************************************
    // Initializing Variables
    //************************************************************************
    uint32_t blinkingLightCounter = 0;
    bool exitProgram = false;

    //************************************************************************
    // starting functional calls and main while loop
    //************************************************************************
    // Displaying Splash Screen
    diplaySplashOnOLED();

    // Displaying UART Menu
    printMainMenu();

    //************************************************************************
    // Enable processor interrupts.
    //************************************************************************
    IntMasterEnable();

    //************************************************************************
    // Local Variables
    //************************************************************************
    WaveFormSate waveFormState = SAWTOOTH;

    //************************************************************************
    // Main while loop
    //************************************************************************
    while(exitProgram == false)
    {
        // Blinking the LED
        if (blinkingLightCounter %
                (FIVE_PERCENT_CYCLE_ON + NIENTYFIVE_PERCENT_CYCLE_OFF) <=
                FIVE_PERCENT_CYCLE_ON) {
            GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_PIN_2);
        }
        else {
            GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0);
        }

        //********************************************************************
        // Functional calls dependent on menu selection
        //********************************************************************

        switch(menuSelection) {
            case 'm' :
                printMainMenu();
                break;
            case 's' :
                waveFormState = SAWTOOTH;
                break;
            case 't' :
                waveFormState = TANGENT;
                break;
            case 'i' :
                waveFormState = SINE;
                break;
            case 'r' :
                waveFormState = SQUARE;
                break;
            case 'q' :
                exitProgram = true;
                break;
        }
        menuSelection = '\0';

        //********************************************************************
        // Creating Waveforms
        //********************************************************************
        static uint8_t I2CData[2] = {0x00, 0x00};
        static uint32_t waveFormCounter = 0;
        static uint8_t* previousBuffer;

        previousBuffer = I2CData;

        switch(waveFormState) {
            case SAWTOOTH: {
                // Writing data to I2C Buffer
                I2CData[1] = waveFormCounter & 0xFF;
                I2CData[0] = (waveFormCounter & 0xF00) >> 8;
                break;
            }
            case SQUARE: {
                if (waveFormCounter >= 4095) {
                    waveFormCounter = 0;
                }

                if (waveFormCounter <= MAX_I2C_VALUE / 2) {
                    // Writing a value of 0 to the I2CData Buffer
                    I2CData[1] = 0x00;
                    I2CData[0] = 0x00;
                }
                else {
                    // Writing a value of 4095 to the I2CData Buffer
                    I2CData[1] = 0xFF;
                    I2CData[0] = 0x0F;
                }
                break;
            }
            case SINE: {
                uint32_t digitalValue = SIN_I2C_SCALING_FACTOR * (sin(waveFormCounter * PI/180) + 1);
                I2CData[1] = digitalValue & 0xFF;
                I2CData[0] = (digitalValue & 0xF00) >> 8;
                //SysCtlDelay(10000);
                break;
            }
            case TANGENT: {
                uint32_t digitalValue = tan(waveFormCounter * PI/180) + 2047;
                I2CData[1] = digitalValue & 0xFF;
                I2CData[0] = (digitalValue & 0xF00) >> 8;
                break;
            }
        }

        if (previousBuffer[1] == I2CData[1] && previousBuffer[0] == I2CData[0]) {
            bool shitWentWrong = true;
        }
        // Creating waveform based on the I2C Buffer
        i2c_write(I2C2_BASE, 0x60, 2, I2CData);

        waveFormCounter++;
        blinkingLightCounter++;
    }
    clearBlack();
}


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif
