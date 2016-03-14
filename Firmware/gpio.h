/******************************************************************************
*	Written by Yilmaz Eyidogan
*	GPIO declarations
*	Created date: 2014.12.22
*******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

/* Private variables -----------------------------------------------------------*/
extern uint32_t ulInputs, ulOutputs;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USER_BUTTON_PORT GPIOA
#define USER_BUTTON_PIN GPIO_Pin_0
#define OUTPUT_PORT_07 GPIOC
#define INPUT_PORT_07 GPIOA
/* Private function declerations----------------------------------------------*/
extern void initGpio(void);
void setGpioOutputs(void);
void readGpioOutputs(void);
void readGpioInputs(void);

#endif
/* * * END OF FILE * * */
