/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Uart declarations
*	Created date: 2014.12.22
*******************************************************************************/
#ifndef __UART_H
#define __UART_H

#include <stdint.h>

/* Private define */
#define UART_RX_BUFFER_SIZE 0x20 //use power of 2
#define UART_TX_BUFFER_SIZE 0x20
#define UART1_RX_TIMEOUT 0x23 //38 bit = 3.5 character

enum {
	UART_TX_READY = 0x00, //no data is transmitting
	UART_TX_BUSY //transmitting data
};

#define UART_RX_BUFFER1_WAITING TRUE
#define UART_RX_BUFFER2_WAITING FALSE

/* Private macro */

/* Private function declarations */
void initUart1(uint32_t baudRate);
void uart1NvicConfig(void);
void initUartDma(void);
void uart1TxCmd(uint8_t *ptr, uint8_t length);
void taskUart1 (void *pdata);

/* Private typedef */
typedef struct {
	uint8_t length1; //length of data to be read of buffer1
	uint8_t length2; //length of data to be read of buffer1
	uint8_t buffer1[UART_RX_BUFFER_SIZE];
	uint8_t buffer2[UART_RX_BUFFER_SIZE];
}UART_RX_BUFFER_TYPE;

typedef struct {
	uint8_t bufferId;
	uint8_t length; //length of data to be transferred
	uint8_t buffer[UART_TX_BUFFER_SIZE];
}UART_TX_BUFFER_TYPE;

typedef struct {
	uint8_t
		rxTimeOut:1,
		rxError:1,
		rxOnlineBuffer,
		rxBuffer1Received,
		rxBuffer2Received,
		txBusy:1,
		txError:1;
}UART_STATUS_TYPE;

/* Private variables */
extern UART_RX_BUFFER_TYPE uart1Rx;
extern UART_TX_BUFFER_TYPE uart1Tx;
extern UART_STATUS_TYPE uart1Flags;

#endif
