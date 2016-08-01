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
uint32_t ulMfTmp;//, *ptruLong;
uint16_t uiMfTmp, *ptruInt;
UINT16_TYPE uistMfTmp;
uint8_t *ptruChar;

uint32_t ulPar1 = 0x01234567;

extern const uint16_t sizeOfHoldingRegister; //defined at the end of this file
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
* @brief       func 0x03, read holding registers
* @param[in]
* @param[in]
* @param[out] application status TRUE or FALSE
*******************************************************************************
*/
uint8_t readHoldingRegister(uint16_t indexOfHoldingRegister){
	PTR_READ_HOLDING_REQ->functionCode = FUNC_READ_HOLDING_REGISTERS;
	PTR_READ_HOLDING_REQ->byteCount = uiWordQty * 2;

	ptruChar = (uint8_t *)&(PTR_READ_HOLDING_REQ->word1.word);

	while (uiWordQty--){
		switch (stHoldingRegArray[indexOfHoldingRegister].registerType){
		case W_U16_PTR_VAL:
			*ptruChar++ = 0x00;
			*ptruChar++ = 0x00;
			break;
		case R_U16_PTR_VAL:
		case RW_U16_PTR_VAL:
			uistMfTmp.word = *stHoldingRegArray[indexOfHoldingRegister].ptrU16;
			*ptruChar++ = uistMfTmp.bytes.high_byte;
			*ptruChar++ = uistMfTmp.bytes.low_byte;
			break;
		case RW_U16_FROM_PTR_FUNC:
			break;
		case LAST_DUMMY_REGITER:
			uiWordQty = 0x00;
			break;
		default:
			break;
		}
		++indexOfHoldingRegister;
	}
	crc16(mbTxRxData.ptrTxData, 0x03 + PTR_READ_HOLDING_REQ->byteCount);
	mbTxRxData.txLength = 0x05 + PTR_READ_HOLDING_REQ->byteCount;
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
/**
*******************************************************************************
* @brief       r+w dummy function used if there is no r+w function
* @param[in]
* @param[out] application status TRUE
*******************************************************************************
*/
uint16_t rwDummyFunc(void){
	return TRUE;
}
/**
*******************************************************************************
* @brief       modbus holding register list
*******************************************************************************
*/
const ST_MODBUS_HOLDING_REGISTER_TYPE stHoldingRegArray[] = {
	{RW_U16_PTR_VAL,
	(uint16_t *)&ulPar1 + 1,
	rwDummyFunc,
	rwDummyFunc,
	START_OF_WORD_REGISTER},

	{RW_U16_PTR_VAL,
	((uint16_t *)&ulPar1),
	rwDummyFunc,
	rwDummyFunc,
	END_OF_DWORD_REGISTER},

	{LAST_DUMMY_REGITER}
};
const uint16_t sizeOfHoldingRegister = sizeof(stHoldingRegArray)/sizeof(ST_MODBUS_HOLDING_REGISTER_TYPE) - 1;
/* * * END OF FILE * * */
