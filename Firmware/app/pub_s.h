/******************************************************************************
*	Written by Yilmaz Eyidogan
*	public structures
*	Created date: 2015.01.04
*******************************************************************************/
#ifndef __PUB_S_H
#define __PUB_S_H

#include <stdint.h>

/* Public typedef */
typedef struct {
	uint8_t low_byte;
	uint8_t high_byte;
}UINT16_BYTE_TYPE;

typedef union {
	uint16_t word;
	UINT16_BYTE_TYPE bytes;
}UINT16_TYPE;

typedef struct {
	uint8_t uart1_protocol;
	uint16_t uart1_baudrate;
	uint8_t uart1_address;
	uint8_t uart2_protocol;
	uint16_t uart2_baudrate;
	uint8_t uart2_address;
}PARAMETER_TYPE;

/* Public define */

/* Public macro */
/* Public variables */


#endif

