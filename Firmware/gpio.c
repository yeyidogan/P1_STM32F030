/******************************************************************************
*	Written by Yilmaz Eyidogan
*	GPIO functions
*	Created date: 2014.12.22
*******************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief Configure the GPIO
  * @param  None
  * @retval None
  */
void initGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_DeInit(GPIOA);

	/* USART1 Pins configuration */
	/* Connect pin to Peripherals */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	/* Configure pins as AF push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure PA0 to PA7 in input mode */
	GPIO_InitStructure.GPIO_Pin = ((uint16_t)0x00FF); //PA0..PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_DeInit(GPIOC);
	/* Configure PC0 to PC9 in output push-pull mode */
	GPIO_InitStructure.GPIO_Pin = ((uint16_t)0x03FF); //PC0..PC9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
