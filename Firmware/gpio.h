/******************************************************************************
*	Written by Yilmaz Eyidogan
*	GPIO declarations
*	Created date: 2014.12.22
*******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USER_BUTTON_PORT GPIOA
#define USER_BUTTON_PIN GPIO_Pin_0

/* Private function declerations----------------------------------------------*/
extern void initGpio(void);

#endif
