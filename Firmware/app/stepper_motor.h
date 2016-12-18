/******************************************************************************
*	Written by Yilmaz Eyidogan
*	stepper motor functions
*	Created date: 2016.12.15
*******************************************************************************/
#ifndef __STEPPER_MOTOR_H_
#define __STEPPER_MOTOR_H_

#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include <stdbool.h>

/* typedef -----------------------------------------------------------*/
typedef struct{
	uint8_t ucStepperMotorAindex;
	uint8_t ucStepperMotorBindex;
}STEPPER_MOTOR_CONTROL_TYPE;
/* define ------------------------------------------------------------*/
#define MAX_INDEX_OF_STEPPER_MOTOR 0x03

enum{
	STEPPER_FORWARD,
	STEPPER_BACKWARD
};
/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
extern const uint8_t stepperMotorFullStepArray[];

#endif
/* * * END OF FILE * * */
