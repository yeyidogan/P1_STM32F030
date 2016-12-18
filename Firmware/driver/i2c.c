/******************************************************************************
*	Written by Yilmaz Eyidogan
*	I2C functions
*	Created date: 2016.12.02
*******************************************************************************/
#include "i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t ucI2CTxBuf[I2C_TX_BUF_SIZE] = {0x00};
uint8_t ucI2CRxBuf[I2C_RX_BUF_SIZE] = {0x00};
uint8_t * i2cTxBufPtr;
uint8_t * i2cRxBufPtr;
I2C_MSG_TX_TYPE stI2cMsgTx = {0x00};
I2C_STATUS_TYPE stI2cStatus = {0x00};
uint8_t ucI2CMasterSendStartStop = 0x00;

uint32_t ulTempI2C = 0x00;
uint16_t uiTimerI2C = 0x00; //to control loops

/**
  * @brief i2c1 interrupt handler
  * @param  None
  * @retval None
  */
void I2C1_IRQHandler(void){
	CoEnterISR();
	//Transfer direction (slave mode) 0: wr, 1: rd
	//I2C1->ISR & I2C_ISR_DIR
	if (I2C1->ISR & I2C_ISR_ADDR){ //address matched in slave mode
		I2C1->ICR = I2C_ICR_ADDRCF; //clear flag
		i2cRxBufPtr = ucI2CRxBuf;
		i2cTxBufPtr = ucI2CTxBuf;
	}
	if (I2C1->ISR & I2C_ISR_STOPF){ //stop detected
		I2C1->ICR = I2C_ICR_STOPCF; //clear flag
		stI2cStatus.bits.completed_flag = true;
	}
	if (I2C1->ISR & I2C_ISR_TC){ //transfer completed in master mode
		/*This flag is set by hardware when RELOAD=0, AUTOEND=0 
		and NBYTES data have been transferred. 
		It is cleared by software when START bit or STOP bit is set.*/
		if (ucI2CMasterSendStartStop > I2C_MASTER_SEND_STOP){
			ulTempI2C = (uint32_t)(((uint32_t)stI2cMsgTx.slaveAddress << 1) & I2C_CR2_SADD);
			ulTempI2C |= I2C_CR2_RD_WRN;
			i2cRxBufPtr = ucI2CRxBuf;
			ulTempI2C |= (uint32_t)(((uint32_t)stI2cMsgTx.length << 16 ) & I2C_CR2_NBYTES);
			I2C1->CR2 = ulTempI2C;
			/* Generate a START condition */
			I2C1->CR2 |= I2C_CR2_START;
			--ucI2CMasterSendStartStop;
		}
		else{
			/* Generate a STOP condition */
			I2C1->CR2 |= I2C_CR2_STOP;
			stI2cStatus.bits.completed_flag = true;
		}
	}
	if (I2C1->ISR & I2C_ISR_TXE){ //Transmit data register empty
		/* Write in the DR register the data to be sent */
		I2C1->TXDR = (uint8_t)*i2cTxBufPtr;
		++i2cTxBufPtr;
	}
	if (I2C1->ISR & I2C_ISR_RXNE){ //Receive data register not empty
		/* Return the data in the DR register */
		*i2cRxBufPtr = (uint8_t)I2C1->RXDR;
		++i2cRxBufPtr;
	}
	if (I2C1->ISR & I2C_ISR_TXIS){ //Transmit interrupt status
		/* Write in the DR register the data to be sent */
		I2C1->TXDR = (uint8_t)*i2cTxBufPtr;
		++i2cTxBufPtr;
	}
	if (I2C1->ISR & I2C_ISR_NACKF){ //NACK received flag
		I2C1->ICR = I2C_ICR_NACKCF; //clear flag
	}
	
	//Error interrupts
	if (I2C1->ISR & I2C_ISR_OVR){ //Overrun/Underrun in slave mode
		I2C1->ICR = I2C_ICR_OVRCF; //clear flag
		stI2cStatus.bits.over_run_flag = true;
	}
	if (I2C1->ISR & I2C_ISR_ARLO){ //Arbitration lost
		I2C1->ICR = I2C_ICR_ARLOCF; //clear flag
		stI2cStatus.bits.arblost_flag = true;
	}
	if (I2C1->ISR & I2C_ISR_BERR){ //Bus error
		I2C1->ICR = I2C_ICR_BERRCF; //clear flag
		stI2cStatus.bits.bus_error_flag = true;
	}
	CoExitISR();
}

/**
  * @brief init i2c
  * @param  None
  * @retval None
  */
void initI2C(void){
	/* Enable I2C1 reset state */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	/* Release I2C1 from reset state */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	
	/* Disable I2Cx Peripheral */
	I2C1->CR1 &= (uint32_t)~((uint32_t)I2C_CR1_PE);
	I2C1->TIMINGR = 0x00200E1C;
	/* Set analog and digital filters*/
	I2C1->CR1 = 0x080FFE;
	//bit23:0 PEC calculation disabled
	//bit22:0 SMBUS alert
	//bit21:0 SMBUS device address
	//bit20:0 SMBUS host address enabled
	//bit19:1 General call enabled
	//bit18:0 reserved
	//bit17:0 Clock stretch enabled
	//bit16:0 Slave byte control disabled
	//bit15:0 RXDMA disabled
	//bit14:0 TXDMA disabled
	//bit13:0 reserved
	//bit12:0 Analog filter enabled
	//bit11-8:1111 Digital filter
	//bit7:1 Error interrupt enabled
	//bit6:1 Transfer complete interrupt enabled
	//bit5:1 Stop detection interrupt enabled
	//bit4:1 NACK received interrupt enabled
	//bit3:1 Address match interrupt enabled
	//bit2:1 RX interrupt enabled
	//bit1:1 TX interrupt enabled
	//bit0:0 Peripheral disabled
	
	I2C1->OAR1 = 0x00;
	I2C1->OAR1 = (uint32_t)I2C_OWN_ADDR << 1;
	I2C1->OAR1 |= (uint32_t)0x8000; //own address 1 enabled
	
	/* Enable I2Cx Peripheral */
	I2C1->CR1 |= I2C_CR1_PE;

	NVIC_EnableIRQ(I2C1_IRQn); //defined in core_cm0.h
	NVIC_SetPriority(I2C1_IRQn,0);
}

/**
  * @brief i2c master transmit
  * @param  None
  * @retval None
  */
uint8_t i2c_master_process(uint8_t rw){
	
	if (ucI2CMasterSendStartStop == 0x00 || ucI2CMasterSendStartStop > I2C_MASTER_SEND_MAX)
		return false;
	stI2cStatus.byte = (uint8_t)0x00;
	i2cTxBufPtr = ucI2CTxBuf;
	ulTempI2C = (uint32_t)(((uint32_t)stI2cMsgTx.slaveAddress << 1) & I2C_CR2_SADD);
	if (rw == I2C_MASTER_READ){
		ulTempI2C |= I2C_CR2_RD_WRN;
		i2cRxBufPtr = ucI2CRxBuf;
	}
	//ulTempI2C |= I2C_CR2_AUTOEND; //auto end mode after nbyte transfer in master mode
	if (stI2cMsgTx.length == 0x00)
		return false;
	ulTempI2C |= (uint32_t)(((uint32_t)stI2cMsgTx.length << 16 ) & I2C_CR2_NBYTES);
	
	I2C1->CR2 = ulTempI2C;
	
	//wait while bus is not free
	START_I2C_TIMER;
	while (I2C1->ISR & I2C_ISR_BUSY){
		if (CHECK_I2C_TIMER_REACH_TO(1000))
			return false;
	}
	/* Generate a START condition */
	I2C1->CR2 |= I2C_CR2_START;
	--ucI2CMasterSendStartStop;
	
	return true;
}
/* * * END OF FILE * * */
