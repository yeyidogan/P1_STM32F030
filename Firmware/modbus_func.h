/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Modbus application function declerations
*	Created date: 2016.02.20
*******************************************************************************/
#ifndef __MODBUS_FUNC_H
#define __MODBUS_FUNC_H

#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_misc.h"
#include "uart.h"
#include "pub_var.h"
#include "pub_s.h"
#include "util.h"
#include "modbus.h"
#include "gpio.h"

/* Private macro */
/* Private variables */
/* Private functions */
extern uint8_t readCoils(void);
extern uint8_t readInputs(void);
extern uint8_t writeSingleCoil(void);

#endif
/* * * END OF FILE * * */
