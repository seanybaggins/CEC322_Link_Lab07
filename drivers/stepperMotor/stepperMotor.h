/*
 * stepperMotor.h
 *
 *  Created on: Feb 27, 2018
 *      Author: Sean Link
 *     Purpose: Store constants and function prototypes
 *              specific to the Stepper Motor
 */

#ifndef DRIVERS_STEPPERMOTOR_STEPPERMOTOR_H_
#define DRIVERS_STEPPERMOTOR_STEPPERMOTOR_H_

// Constants
#define STEPPER_MOTOR_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define TOTAL_COIL_STATES 8
#define STEPS_PER_REVOLUTION 96

typedef enum {
    OFF = 0, COUNTER_CLOCKWISE = 1, CLOCKWISE = 2, FOLLOW = 3, TOTAL_STATES = 4
} MotorState;



// Function prototypes
void setupStepperMotor(void);


#endif /* DRIVERS_STEPPERMOTOR_STEPPERMOTOR_H_ */
