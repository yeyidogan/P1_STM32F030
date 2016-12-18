/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Modbus functions
*	Created date: 2015.01.05
*******************************************************************************/
#include "modbus.h"

/* Private typedef */
/* Private define */

/* Private macro */
/* Private variables */
MODBUS_TX_RX_DATA mbTxRxData;

const UINT16_TYPE stCrcTable[] = {
	{0x0000}, {0xC1C0}, {0x81C1}, {0x4001}, {0x01C3}, {0xC003}, {0x8002}, {0x41C2},
	{0x01C6}, {0xC006}, {0x8007}, {0x41C7}, {0x0005}, {0xC1C5}, {0x81C4}, {0x4004},
	{0x01CC}, {0xC00C}, {0x800D}, {0x41CD}, {0x000F}, {0xC1CF}, {0x81CE}, {0x400E},
	{0x000A}, {0xC1CA}, {0x81CB}, {0x400B}, {0x01C9}, {0xC009}, {0x8008}, {0x41C8},
	{0x01D8}, {0xC018}, {0x8019}, {0x41D9}, {0x001B}, {0xC1DB}, {0x81DA}, {0x401A},
	{0x001E}, {0xC1DE}, {0x81DF}, {0x401F}, {0x01DD}, {0xC01D}, {0x801C}, {0x41DC},
	{0x0014}, {0xC1D4}, {0x81D5}, {0x4015}, {0x01D7}, {0xC017}, {0x8016}, {0x41D6},
	{0x01D2}, {0xC012}, {0x8013}, {0x41D3}, {0x0011}, {0xC1D1}, {0x81D0}, {0x4010},
	{0x01F0}, {0xC030}, {0x8031}, {0x41F1}, {0x0033}, {0xC1F3}, {0x81F2}, {0x4032},
	{0x0036}, {0xC1F6}, {0x81F7}, {0x4037}, {0x01F5}, {0xC035}, {0x8034}, {0x41F4},
	{0x003C}, {0xC1FC}, {0x81FD}, {0x403D}, {0x01FF}, {0xC03F}, {0x803E}, {0x41FE},
	{0x01FA}, {0xC03A}, {0x803B}, {0x41FB}, {0x0039}, {0xC1F9}, {0x81F8}, {0x4038},
	{0x0028}, {0xC1E8}, {0x81E9}, {0x4029}, {0x01EB}, {0xC02B}, {0x802A}, {0x41EA},
	{0x01EE}, {0xC02E}, {0x802F}, {0x41EF}, {0x002D}, {0xC1ED}, {0x81EC}, {0x402C},
	{0x01E4}, {0xC024}, {0x8025}, {0x41E5}, {0x0027}, {0xC1E7}, {0x81E6}, {0x4026},
	{0x0022}, {0xC1E2}, {0x81E3}, {0x4023}, {0x01E1}, {0xC021}, {0x8020}, {0x41E0},
	{0x01A0}, {0xC060}, {0x8061}, {0x41A1}, {0x0063}, {0xC1A3}, {0x81A2}, {0x4062},
	{0x0066}, {0xC1A6}, {0x81A7}, {0x4067}, {0x01A5}, {0xC065}, {0x8064}, {0x41A4},
	{0x006C}, {0xC1AC}, {0x81AD}, {0x406D}, {0x01AF}, {0xC06F}, {0x806E}, {0x41AE},
	{0x01AA}, {0xC06A}, {0x806B}, {0x41AB}, {0x0069}, {0xC1A9}, {0x81A8}, {0x4068},
	{0x0078}, {0xC1B8}, {0x81B9}, {0x4079}, {0x01BB}, {0xC07B}, {0x807A}, {0x41BA},
	{0x01BE}, {0xC07E}, {0x807F}, {0x41BF}, {0x007D}, {0xC1BD}, {0x81BC}, {0x407C},
	{0x01B4}, {0xC074}, {0x8075}, {0x41B5}, {0x0077}, {0xC1B7}, {0x81B6}, {0x4076},
	{0x0072}, {0xC1B2}, {0x81B3}, {0x4073}, {0x01B1}, {0xC071}, {0x8070}, {0x41B0},
	{0x0050}, {0xC190}, {0x8191}, {0x4051}, {0x0193}, {0xC053}, {0x8052}, {0x4192},
	{0x0196}, {0xC056}, {0x8057}, {0x4197}, {0x0055}, {0xC195}, {0x8194}, {0x4054},
	{0x019C}, {0xC05C}, {0x805D}, {0x419D}, {0x005F}, {0xC19F}, {0x819E}, {0x405E},
	{0x005A}, {0xC19A}, {0x819B}, {0x405B}, {0x0199}, {0xC059}, {0x8058}, {0x4198},
	{0x0188}, {0xC048}, {0x8049}, {0x4189}, {0x004B}, {0xC18B}, {0x818A}, {0x404A},
	{0x004E}, {0xC18E}, {0x818F}, {0x404F}, {0x018D}, {0xC04D}, {0x804C}, {0x418C},
	{0x0044}, {0xC184}, {0x8185}, {0x4045}, {0x0187}, {0xC047}, {0x8046}, {0x4186},
	{0x0182}, {0xC042}, {0x8043}, {0x4183}, {0x0041}, {0xC181}, {0x8180}, {0x4040}
};

uint16_t uiWordQty = 0x00;
/**
*******************************************************************************
* @brief       calculate crc
* @param[in]   ptrCell: pointer of data
* @param[in]   length: length of data without CRC
* @param[out]  crc
*******************************************************************************
*/
void crc16(uint8_t *ptrCell, uint8_t length) {
	uint8_t ucIndex;
	UINT16_TYPE crcWord = {0xFFFF};

	while (length--) {
		ucIndex = crcWord.bytes.high_byte ^ *ptrCell++;
		crcWord.bytes.high_byte = crcWord.bytes.low_byte ^ stCrcTable[ucIndex].bytes.high_byte;
		crcWord.bytes.low_byte = stCrcTable[ucIndex].bytes.low_byte;
	}
	*ptrCell++ = crcWord.bytes.high_byte;
	*ptrCell = crcWord.bytes.low_byte;
}

/**
*******************************************************************************
* @brief       check crc
* @param[in]   ptrCell: pointer of data
* @param[in]   length: length of data without CRC
* @param[out]  TRUE or FALSE
*******************************************************************************
*/
uint8_t checkCrc16(uint8_t *ptrCell, uint16_t length) {
	uint8_t ucIndex;
	UINT16_TYPE crcWord = {0xFFFF};

	while (length--) {
		ucIndex = crcWord.bytes.high_byte ^ *ptrCell++;
		crcWord.bytes.high_byte = crcWord.bytes.low_byte ^ stCrcTable[ucIndex].bytes.high_byte;
		crcWord.bytes.low_byte = stCrcTable[ucIndex].bytes.low_byte;
	}
	if (crcWord.bytes.high_byte == *ptrCell) {
		++ptrCell;
		if (crcWord.bytes.low_byte == *ptrCell)
			return TRUE;
	}
	return FALSE;
}

/**
*******************************************************************************
* @brief       prepare exception error message
* @param[in]   exceptionCode
* @param[out]  none
*******************************************************************************
*/
#define SIZE_OF_EXCEPTION_FRAME 0x05
void mbReturnException(uint8_t exceptionCode) {

	PTR_EXCEPTION->slaveAdd = *mbTxRxData.ptrRxData;
	PTR_EXCEPTION->functionCode = PTR_MODBUS_READ_REQ->functionCode + 0x80;
	PTR_EXCEPTION->exceptionCode = exceptionCode;

	crc16(mbTxRxData.ptrTxData, SIZE_OF_EXCEPTION_FRAME - 2);
	mbTxRxData.txLength = SIZE_OF_EXCEPTION_FRAME;
}

/**
*******************************************************************************
* @brief       modbusRTU main code
* @param[in]   none
* @param[out]  none
* @details     none.
*******************************************************************************
*/
void modbusRTU(void){
	uint16_t tmp;
	mbTxRxData.txLength = 0x00;
	if (mbTxRxData.rxLength < MB_MIN_REQUEST_FRAME_SIZE){ //return if there is not enough data
		return;
	}

	if (*mbTxRxData.ptrRxData == mbTxRxData.slaveAdd){
		if (checkCrc16(mbTxRxData.ptrRxData, mbTxRxData.rxLength - 2) == FALSE){
			return;
		}
		*mbTxRxData.ptrTxData = mbTxRxData.slaveAdd;

		//check function code and quantity
		switch (PTR_MODBUS_READ_REQ->functionCode){
			case FUNC_READ_COILS:
			case FUNC_READ_DISCRETE_INPUTS:
			case FUNC_READ_HOLDING_REGISTERS:
			case FUNC_READ_INPUT_REGISTERS:
			case FUNC_WRITE_MULTIPLE_COILS:
			case FUNC_WRITE_MULTIPLE_REGISTERS:
				tmp = wordEndianer(PTR_MODBUS_READ_REQ->quantity.word);
				if (tmp == 0 || tmp > 0x07D0){
					mbReturnException(OUT_OF_MB_LIMIT);
					return;
				}
				break;
		}

		uiWordQty = wordEndianer(PTR_MODBUS_READ_REQ->quantity.word);

		switch (PTR_MODBUS_READ_REQ->functionCode){
			case FUNC_READ_COILS:
				tmp = wordEndianer(PTR_MODBUS_READ_REQ->startAddress.word) + uiWordQty;
				if (tmp > MB_COIL_SIZE){
					mbReturnException(OUT_OF_DATA_REGION);
					break;
				}
				if (readCoils() == FALSE){
					mbReturnException(PROCESS_ERROR);
				}
				break;
			case FUNC_READ_DISCRETE_INPUTS:
				tmp = wordEndianer(PTR_MODBUS_READ_REQ->startAddress.word) + uiWordQty;
				if (tmp > MB_INPUT_SIZE) {
					mbReturnException(OUT_OF_DATA_REGION);
					break;
				}
				if (readInputs() == FALSE){
					mbReturnException(PROCESS_ERROR);
				}
				break;
			case FUNC_READ_HOLDING_REGISTERS:
				tmp = wordEndianer(PTR_MODBUS_READ_REQ->startAddress.word) + uiWordQty;
				if (tmp > sizeOfHoldingRegister) {
					mbReturnException(OUT_OF_DATA_REGION);
					break;
				}
				tmp = wordEndianer(PTR_MODBUS_READ_REQ->startAddress.word);
				if (readHoldingRegister(tmp) == FALSE){
					mbReturnException(PROCESS_ERROR);
				}
				break;
			case FUNC_READ_INPUT_REGISTERS:
				break;
			case FUNC_WRITE_SINGLE_COIL:
				tmp = wordEndianer(PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
				if (tmp >= MB_COIL_SIZE){
					mbReturnException(OUT_OF_DATA_REGION);
					break;
				}
				if (writeSingleCoil() == FALSE){
					mbReturnException(PROCESS_ERROR);
				}
				break;
			case FUNC_WRITE_SINGLE_REGISTER:
				break;
			case FUNC_WRITE_MULTIPLE_COILS:
				break;
			case FUNC_WRITE_MULTIPLE_REGISTERS:
				break;
			default:
				mbReturnException(FUNCTION_UNSUPPORTED);
				break;
		}
	}
}
/* * * END OF FILE * * */
