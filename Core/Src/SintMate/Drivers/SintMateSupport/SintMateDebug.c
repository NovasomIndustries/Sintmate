/*
 * SintMateDebug.c
 *
 *  Created on: Feb 11, 2021
 *      Author: fil
 */

#include "main.h"

void SintMateDebug(uint32_t val)
{
	if ( val == 1)
		HAL_GPIO_WritePin(GPIOA, FLASH_SS_Pin|DEBUG_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOA, FLASH_SS_Pin|DEBUG_Pin, GPIO_PIN_RESET);

}

