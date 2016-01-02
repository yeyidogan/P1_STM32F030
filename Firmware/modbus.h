/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Modbus declarations
*	Created date: 2015.01.06
*******************************************************************************/
#ifndef __MODBUS_H
#define __MODBUS_H

#include <stdint.h>
#include "uart.h"
#include "pub_s.h"

/* Private typedef */
typedef struct {
	uint8_t mbFunction;
	UINT16_TYPE mbRegAddress;
	UINT16_TYPE mbNumOfRegOrCoil;
}MODBUS_REQUEST_FRAME;

typedef struct {
	UART_TX_BUFFER_TYPE *uartTxPtr;
	UART_RX_BUFFER_TYPE *uartRxPtr;
	uint8_t mbAddress;
}MODBUS_TX_RX_DATA;
/* Private define */
#define MB_MIN_FRAME_SIZE 0x08
#define MB_WRITE_MULTIPLE_COIL_FRAME_SIZE 0x0A
#define MB_WRITE_MULTIPLE_REG_FRAME_SIZE 0x0B

#define MB_COIL_SIZE 0x10 //16 output
#define MB_INPUT_SIZE 0x10 //16 input

enum {
	MBFN_READ_COILS = 1, //8 bytes request frame
	MBFN_READ_DISCRETE_INPUTS, //8 bytes request frame
	MBFN_READ_HOLDING_REGISTERS, //8 bytes request frame
	MBFN_READ_INPUT_REGISTERS, //8 bytes request frame
	MBFN_WRITE_SINGLE_COIL, //8 bytes request frame
	MBFN_WRITE_SINGLE_REGISTER, //8 bytes request frame
	MBFN_WRITE_MULTIPLE_COILS = 15, //11 bytes request frame
	MBFN_WRITE_MULTIPLE_REGISTERS //13 bytes request frame
}MODBUS_FUNCTION;
enum {
	EXCEPTION_NONE = 0x00,
	FUNCTION_UNSUPPORTED,
	OUT_OF_DATA_REGION,
	OUT_OF_MB_LIMIT,
	PROCESS_ERROR
}MODBUS_EXCEPTION;
/* Private macro */
/* Private variables */
extern MODBUS_REQUEST_FRAME mbRequestFrame;
extern uint8_t *ptrData, *ptrMax, *ptrMin;
extern MODBUS_TX_RX_DATA mbTxRxData;
//extern const uint16_t wCrcTable[];

/* Private functions */
void modbusRTU(void);
//void incPtrData(void);
//uint16_t crc16(const uint8_t *ptrCell, uint16_t length);
//uint8_t checkCrc16(const uint8_t *ptrCell, uint16_t length);

#endif
