/*
 * personalTimers.h
 *
 *  Created on: Mar 3, 2018
 *      Author: Sean Link
 *     Purpose: Store constants and function prototypes
 *              specific to the Timers Peripheral
 */

#ifndef DRIVERS_TIMERS_PERSONALTIMERS_H_
#define DRIVERS_TIMERS_PERSONALTIMERS_H_

void setupTimers(void);
extern void oneSecondTimer(void);
extern void stepperMotorTimer(void);
#endif /* DRIVERS_TIMERS_PERSONALTIMERS_H_ */
