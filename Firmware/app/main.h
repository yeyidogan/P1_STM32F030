/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main.c functions
*	Created date: 2016.12.17
*******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#include "CoOs.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_usart.h"
#include "gpio.h"
#include "uart.h"
#include "pub_var.h"
#include "stm32f0xx_misc.h"
#include "i2c.h"
#include "timer.h"
#include "hdc1080.h"

/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* Functions ---------------------------------------------------------*/
extern void rccConfig(void);
/* variables ---------------------------------------------------------*/
/* mutexes ---------------------------------------------------------*/
extern OS_MutexID mutex_I2C;

#endif
