/******************************************************************************
*	Written by Yilmaz Eyidogan
*	util functions
*	Created date: 2016.02.22
*******************************************************************************/
#include "util.h"

/**
 *******************************************************************************
 * @brief       endian changer
 * @param[in]   None
 * @retval      None
 * @par Description little endian to big endian
 *******************************************************************************
 */
uint16_t wordEndianer(uint16_t wordIn) {
	uint16_t tmp;

	tmp = wordIn;
	tmp <<= 8;
	wordIn >>= 8;
	tmp += wordIn;
	return tmp;
}
/* * * END OF FILE * * */
