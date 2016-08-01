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
const uint16_t sizeOfHoldingRegister;

enum{
	START_OF_WORD_REGISTER = (uint8_t)0x00,
	END_OF_WORD_REGISTER,
	START_OF_DWORD_REGISTER,
	END_OF_DWORD_REGISTER
};

enum MODBUS_REGISTER_LIST{
	LAST_DUMMY_REGITER = (uint8_t)0x00,
	W_U16_PTR_VAL,
	R_U16_PTR_VAL,
	RW_U16_PTR_VAL,
	RW_U16_FROM_PTR_FUNC
};

typedef struct{
	enum MODBUS_REGISTER_LIST registerType;
	uint16_t * ptrU16; //r+w to same pointer
	uint16_t (*mbReadFuncPtr)(void); //modbus read function pointer
	uint16_t (*mbWriteFuncPtr)(void); //modbus write function pointer
	uint8_t orderOfRegister; //multiple registers can be a long variable
}ST_MODBUS_HOLDING_REGISTER_TYPE;

/* Private variables */
/* Private functions */
extern uint8_t readCoils(void);
extern uint8_t readInputs(void);
extern uint8_t readHoldingRegister(uint16_t);
extern uint8_t writeSingleCoil(void);

extern const ST_MODBUS_HOLDING_REGISTER_TYPE stHoldingRegArray[];

#endif
/* * * END OF FILE * * */
