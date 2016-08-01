/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Modbus declarations
*	Created date: 2015.01.06
*******************************************************************************/
#ifndef __MODBUS_H
#define __MODBUS_H

#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_misc.h"
#include "uart.h"
#include "pub_var.h"
#include "pub_s.h"
#include "util.h"
#include "modbus_func.h"

/* Private typedef */
typedef struct {
	uint8_t *ptrTxData;
	uint8_t txLength;
	uint8_t *ptrRxData;
	uint8_t rxLength;
	uint8_t slaveAdd;
}MODBUS_TX_RX_DATA;

typedef struct {
	uint8_t slaveAdd;
	uint8_t functionCode;
	UINT16_TYPE startAddress;
	UINT16_TYPE quantity;
	UINT16_TYPE crc;
}__attribute__((packed)) MODBUS_READ_REQUEST_FRAME;
//0x01, 0x02, 0x03, 0x04 functions

typedef struct {
	uint8_t slaveAdd;
	uint8_t functionCode;
	UINT16_TYPE address;
	UINT16_TYPE value;
	UINT16_TYPE crc;
}__attribute__((packed)) MODBUS_WRITE_SINGLE_REQUEST_FRAME;
//0x05 and 0x06 functions

typedef struct {
	uint8_t slaveAdd;
	uint8_t functionCode;
	UINT16_TYPE startAddress;
	UINT16_TYPE quantity;
	uint8_t byteCount;
	uint8_t value;
}__attribute__((packed)) MODBUS_WRITE_MULTIPLE_COILS_REQUEST_FRAME;
//0x0F function

typedef struct {
	uint8_t slaveAdd;
	uint8_t functionCode;
	UINT16_TYPE startAddress;
	UINT16_TYPE quantity;
	uint8_t byteCount;
	UINT16_TYPE value;
}__attribute__((packed)) MODBUS_WRITE_MULTIPLE_REGISTERS_REQUEST_FRAME;
//0x10 function

typedef struct {
	uint8_t slaveAdd;
	uint8_t functionCode;
	uint8_t exceptionCode;
	UINT16_TYPE crc;
}__attribute__((packed)) MODBUS_EXCEPTION_RESPONSE_FRAME;

typedef struct {
	uint8_t slaveAdd;
	uint8_t functionCode;
	uint8_t byteCount;
	uint8_t byte1;
	UINT16_TYPE crc;
}__attribute__((packed)) MODBUS_READ_BYTE_RESPONSE_FRAME;

typedef struct {
	uint8_t slaveAdd;
	uint8_t functionCode;
	uint8_t byteCount;
	UINT16_TYPE word1;
	UINT16_TYPE crc;
}__attribute__((packed)) MODBUS_READ_WORD_RESPONSE_FRAME;

/* Private define */
#define MB_MIN_REQUEST_FRAME_SIZE 0x08

#define MB_COIL_SIZE 0x08 //8 output
#define MB_INPUT_SIZE 0x08 //8 input

#define PTR_MODBUS_READ_REQ ((MODBUS_READ_REQUEST_FRAME *)mbTxRxData.ptrRxData)
#define PTR_MODBUS_WRITE_SINGLE_REQ ((MODBUS_WRITE_SINGLE_REQUEST_FRAME *)mbTxRxData.ptrRxData)
#define PTR_MODBUS_WRITE_MULTICOIL_REQ ((MODBUS_WRITE_MULTIPLE_COILS_REQUEST_FRAME *)mbTxRxData.ptrRxData)
#define PTR_MODBUS_WRITE_MULTIREGI_REQ ((MODBUS_WRITE_MULTIPLE_REGISTERS_REQUEST_FRAME *)mbTxRxData.ptrRxData)

#define PTR_EXCEPTION ((MODBUS_EXCEPTION_RESPONSE_FRAME *)mbTxRxData.ptrTxData)
#define PTR_READ_BYTE_RESP ((MODBUS_READ_BYTE_RESPONSE_FRAME *)mbTxRxData.ptrTxData)

#define PTR_WRITE_SINGLE_COIL_RESP ((MODBUS_WRITE_SINGLE_REQUEST_FRAME *)mbTxRxData.ptrTxData)

#define PTR_READ_HOLDING_REQ ((MODBUS_READ_WORD_RESPONSE_FRAME *)mbTxRxData.ptrTxData)

enum {
	FUNC_READ_COILS = 1, //8 bytes request frame
	FUNC_READ_DISCRETE_INPUTS, //8 bytes request frame
	FUNC_READ_HOLDING_REGISTERS, //8 bytes request frame
	FUNC_READ_INPUT_REGISTERS, //8 bytes request frame
	FUNC_WRITE_SINGLE_COIL, //8 bytes request frame
	FUNC_WRITE_SINGLE_REGISTER, //8 bytes request frame
	FUNC_WRITE_MULTIPLE_COILS = 15, //11 bytes request frame
	FUNC_WRITE_MULTIPLE_REGISTERS //13 bytes request frame
}MODBUS_FUNCTION_CODE;

enum {
	EXCEPTION_NONE = 0x00,
	FUNCTION_UNSUPPORTED,
	OUT_OF_DATA_REGION,
	OUT_OF_MB_LIMIT,
	PROCESS_ERROR
}MODBUS_EXCEPTION;
/* Private macro */
/* Private variables */

extern MODBUS_TX_RX_DATA mbTxRxData;
extern uint16_t uiWordQty;

/* Private functions */
extern void modbusRTU(void);
extern void crc16(uint8_t *ptrCell, uint8_t length);
//uint8_t checkCrc16(uint8_t *ptrCell, uint16_t length);

#endif
/* * * END OF FILE * * */
