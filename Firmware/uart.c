/******************************************************************************
*	Written by Yilmaz Eyidogan
*	UART functions
*	Created date: 2014.12.24
*******************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_dma.h"
#include "uart.h"
#include "pub_var.h"
#include "par_enums.h"
#include "modbus.h"
#include "util.h"
#include "CoOs.h"

/* Private typedef */
/* Private define */

/* Private macro */
/* Private variables */

UART_RX_BUFFER_TYPE uart1Rx;
UART_TX_BUFFER_TYPE uart1Tx = {0x00, 0x13, "uart1 transmit data\r\n"};
uint8_t uart1TimeOutFlag = FALSE;
//const uint8_t *ptrUart1RxMin = &(uart1Rx.buffer[0]);
//const uint8_t *ptrUart1RxMax = &(uart1Rx.buffer[UART_RX_BUFFER_SIZE - 1]);
/**
  * @brief Configure the USART1 Device
  * @param  baudRate
  * @retval None
  */
void initUart1(uint32_t baudRate)
{
	/* USARTx configured as follow:
	- BaudRate = 9600 baud
	- Word Length = 8 Bits
	- Stop Bit = 1 Stop Bit
	- Parity = No Parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitTypeDef USART_InitStructure;

	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = baudRate; //9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_SetReceiverTimeOut(USART1, UART1_RX_TIMEOUT);
	USART_ReceiverTimeOutCmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RTO, ENABLE); //enable usart1 timeout, required for modbus

	USART_Cmd(USART1, ENABLE); /* Enable USART */
}

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	CoEnterISR();
	if (USART_GetITStatus(USART1, USART_IT_RTO) != RESET)
	{
		uart1TimeOutFlag = TRUE;
	}
	USART_ClearITPendingBit(USART1, USART_IT_RTO);

#if 0
	//TX and RX interrupts
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		uart1Rx.buffer[uart1Rx.bufferId++] = (uint8_t)USART_ReceiveData(USART1);

		if (uart1Rx.bufferId >= UART1_RX_BUFFER_SIZE)
		{
			/* Disable the USART1 Receive interrupt */
			//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
			uart1Rx.bufferId = UART1_RX_BUFFER_SIZE - 0x01;
		}
	}

	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		/* Write one byte to the transmit data register */
		USART_SendData(USART1, uart1Tx.buffer[uart1Tx.bufferId++]);
	    if(uart1Tx.bufferId >= uart1Tx.length)
	    {
	      /* Disable the USART1 Transmit interrupt */
	      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	    }
	}
#endif
	CoExitISR();
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void Uart1_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  This function handles USART1 DMA.
  * @param  None
  * @retval None
  */
void initUartDma(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* DMA1 Channel3 Config: Uart Rx data */
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart1Rx.buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = UART_RX_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	/* DMA1 Channel3 enable */
	DMA_Cmd(DMA1_Channel3, ENABLE);

	/* DMA1 Channel2 Config: Uart Tx data */
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart1Tx.buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = uart1Tx.length;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	/* DMA1 Channel2 enable */
	DMA_Cmd(DMA1_Channel2, ENABLE);

	USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
}

/**
*******************************************************************************
* @brief       uart1 transmitter using dma
* @param[in]   *ptr: pointer of data to be transmitted
* @param[in]   length: length of data
* @details    none.
*******************************************************************************
*/
void uart1TxCmd(uint8_t *ptr, uint8_t length) {
	if (DMA_GetFlagStatus(DMA1_FLAG_TC2)) {
		DMA_ClearFlag(DMA1_FLAG_TC2);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC));
		USART_ClearFlag(USART1, USART_FLAG_TC);
	}
	DMA1_Channel2->CMAR = (uint32_t)ptr;
	DMA_Cmd(DMA1_Channel2, ENABLE);
	while (!DMA_GetFlagStatus(DMA1_FLAG_TC2));
}

/**
*******************************************************************************
* @brief       uart1 main task code
* @param[in]   None
* @param[out]  None
* @retval      None
* @par Description
* @details    none.
*******************************************************************************
*/
void taskUart1 (void* pdata)
{
	uart1Rx.firstDataId = 0x00;

	while (1)
	{
		uart1Rx.newDataId = UART_RX_BUFFER_SIZE - (uint8_t)DMA_GetCurrDataCounter(DMA1_Channel3);
		uart1Rx.newDataId &= (UART_RX_BUFFER_SIZE - 0x01); // prevent id overflow

		if (uart1Rx.newDataId > uart1Rx.firstDataId)
		{
			uart1Rx.length = uart1Rx.newDataId - uart1Rx.firstDataId;
		}
		else if (uart1Rx.firstDataId > uart1Rx.newDataId)
		{
			uart1Rx.length = UART_RX_BUFFER_SIZE + uart1Rx.newDataId - uart1Rx.firstDataId;
		}
		else // is_equal.
		{
			continue;
		}

		switch(sys_par.uart1_protocol)
		{
			case PROTOCOL_MODBUS:
				if (uart1TimeOutFlag == FALSE) //wait for timeout flag
					break;
				uart1TimeOutFlag = FALSE; //set timeout flag again for returning message

				uart1Tx.length = 0x00; //clear transmit data
				mbTxRxData.mbAddress = sys_par.uart1_address;
				mbTxRxData.uartRxPtr = &uart1Rx;
				mbTxRxData.uartTxPtr = &uart1Tx;

				modbusRTU();

				if (uart1TimeOutFlag == FALSE)
					CoTickDelay (1);
				//if (uart1Tx.length)
					//prepare dma to transfer data

				uart1Rx.firstDataId = uart1Rx.newDataId;
				break;
			case PROTOCOL_DSI:
				break;
			default:
				break;
		}
	}
}
