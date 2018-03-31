/*
 * displays.h
 *
 *  Created on: Feb 20, 2018
 *      Author: Sean Link
 *     Purpose: Store constants and function prototypes
 *              specific to the OLED
 *
 */

#ifndef DRIVERS_DISPLAYS_H_
#define DRIVERS_DISPLAYS_H_

typedef enum {
    DISPLAY_OFF = 0, DISPLAY_NUMBER = 1, DISPLAY_BAR = 2, DISPLAY_COUNT = 3
} DisplayMode;

void diplaySplashOnOLED(void);
void diplayInfoOnBoard(uint8_t* formatString,uint32_t ADCValue,
                          uint32_t yLocationOnDisplay, DisplayMode displayMode);
void clearBlack(void);


#endif /* DRIVERS_DISPLAYS_H_ */
