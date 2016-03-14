/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Modbus application functions
*	Created date: 2016.02.20
*******************************************************************************/
#include "modbus_func.h"
/* Private typedef */
/* Private define */

/* Private macro */
/* Private variables */
uint32_t ulMfTmp;
uint16_t uiMfTmp;

/**
*******************************************************************************
* @brief       func 0x01, read coils, outputs
* @param[in]
* @param[in]
* @param[out] application status TRUE or FALSE
*******************************************************************************
*/
uint8_t readCoils(void){
	readGpioOutputs();
	ulMfTmp = ulOutputs;
	ulMfTmp >>= PTR_MODBUS_READ_REQ->quantity.word;
	PTR_READ_BYTE_RESP->functionCode = FUNC_READ_COILS;
	PTR_READ_BYTE_RESP->byteCount = 0x01;
	PTR_READ_BYTE_RESP->byte1 = ulMfTmp;
	crc16(mbTxRxData.ptrTxData, sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME) - 2);
	mbTxRxData.txLength = sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME);
	return TRUE;
}
/**
*******************************************************************************
* @brief       func 0x02, read inputs
* @param[in]
* @param[in]
* @param[out] application status TRUE or FALSE
*******************************************************************************
*/
uint8_t readInputs(void){
	readGpioInputs();
	ulMfTmp = ulInputs;
	ulMfTmp >>= PTR_MODBUS_READ_REQ->quantity.word;
	PTR_READ_BYTE_RESP->functionCode = FUNC_READ_DISCRETE_INPUTS;
	PTR_READ_BYTE_RESP->byteCount = 0x01;
	PTR_READ_BYTE_RESP->byte1 = ulMfTmp;
	crc16(mbTxRxData.ptrTxData, sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME) - 2);
	mbTxRxData.txLength = sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME);
	return TRUE;
}
/**
*******************************************************************************
* @brief       func 0x05, write single coil
* @param[in]
* @param[in]
* @param[out] application status TRUE or FALSE
*******************************************************************************
*/
uint8_t writeSingleCoil(void){
	PTR_WRITE_SINGLE_COIL_RESP->functionCode = FUNC_WRITE_SINGLE_COIL;
	PTR_WRITE_SINGLE_COIL_RESP->address.word = PTR_MODBUS_WRITE_SINGLE_REQ->address.word;
	PTR_WRITE_SINGLE_COIL_RESP->value.word = PTR_MODBUS_WRITE_SINGLE_REQ->value.word;
	if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0xFF00){ //COIL=ON
		ulMfTmp = 1UL;
		ulMfTmp <<= PTR_MODBUS_WRITE_SINGLE_REQ->address.word;
		ulOutputs |= ulMfTmp;
	}
	else if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0x0000){ //COIL=OFF
		ulMfTmp = 0xFFFFFFFE;
		ulMfTmp <<= PTR_MODBUS_WRITE_SINGLE_REQ->address.word;
		ulOutputs &= ulMfTmp;
	}
	else {
		return (FALSE);
	}

	setGpioOutputs();
	crc16(mbTxRxData.ptrTxData, sizeof(MODBUS_WRITE_SINGLE_REQUEST_FRAME) - 2);
	mbTxRxData.txLength = sizeof(MODBUS_WRITE_SINGLE_REQUEST_FRAME);
	return TRUE;
}
/* * * END OF FILE * * */
