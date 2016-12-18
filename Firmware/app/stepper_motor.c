/******************************************************************************
*	Written by Yilmaz Eyidogan
*	stepper motor functions
*	Created date: 2016.12.15
*******************************************************************************/
#include "stepper_motor.h"
/* typedef -----------------------------------------------------------*/
/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/

const uint8_t stepperMotorFullStepArray[] = {0x09, 0x05, 0x06, 0x0A};
STEPPER_MOTOR_CONTROL_TYPE stStepperMotorControl = {0x00};
/* functions ---------------------------------------------------------*/
/**
  * @brief drive stepper motor
  * @param  None
  * @retval None
  */
void driveStepper(uint8_t ucDir, uint16_t uiStepSize) {
	uint16_t i = 0x00;
	
	if (ucDir == STEPPER_FORWARD){
		for (i=0; i++; i<uiStepSize){
			//stStepperMotorControl.
		}
	}
	else {
	}
}




/* * * END OF FILE * * */
