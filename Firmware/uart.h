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

/* Private macro */
/* Private variables */

void initUart1(uint32_t baudRate);
void Uart1_NVIC_Config(void);
void initUartDma(void);
void uart1TxCmd(uint8_t *ptr, uint8_t length);
void taskUart1 (void *pdata);

/* Private typedef */
typedef struct {
	uint8_t firstDataId; //buffer cell id, to be read data
	uint8_t newDataId; //buffer cell id, to be written
	uint8_t length; //length of data to be read
	uint8_t buffer[UART_RX_BUFFER_SIZE];
}UART_RX_BUFFER_TYPE;

typedef struct {
	uint8_t bufferId;
	uint8_t length; //length of data to be transferred
	uint8_t buffer[UART_TX_BUFFER_SIZE];
}UART_TX_BUFFER_TYPE;

#endif
