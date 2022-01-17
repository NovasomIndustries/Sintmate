/*
 * StepperDriver.c
 *
 *  Created on: Feb 11, 2021
 *      Author: fil
 */


#include "main.h"
/*
static	uint32_t	number_of_pulses;
static	uint32_t	pulses_per_second;
*/
void StepperCallback(void)
{

}



void DoSteps(void)
{
uint32_t	CR1_reg;

	CR1_reg = STEPPER_TIMER.Instance->CR1;
	CR1_reg &= ~0x08;	// clear one pulse mode
	STEPPER_TIMER.Instance->CR1 = CR1_reg;
	STEPPER_TIMER.Instance->RCR = 1;
	HAL_TIM_OC_Start(&STEPPER_TIMER, STEPPER_TIMER_CHANNEL);
}

void DoNumberOfSteps(uint32_t steps)
{
uint32_t	CR1_reg;
	if ( steps*2 > 256 )
		return;
	HAL_TIM_OC_Stop(&STEPPER_TIMER, STEPPER_TIMER_CHANNEL);
	CR1_reg = STEPPER_TIMER.Instance->CR1;
	CR1_reg |= 0x08;	// set one pulse mode
	STEPPER_TIMER.Instance->CR1 = CR1_reg;
	STEPPER_TIMER.Instance->RCR = steps*2 -1;
	HAL_TIM_OC_Start(&STEPPER_TIMER, STEPPER_TIMER_CHANNEL);
}

void DoStepperStop(void)
{
	/*
uint32_t	CR1_reg;

	CR1_reg = htim16.Instance->CR1;
	CR1_reg &= ~0x08;	// clear one pulse mode
	htim16.Instance->CR1 = CR1_reg;
	htim16.Instance->RCR = 1;
	*/
	HAL_TIM_OC_Stop(&STEPPER_TIMER, STEPPER_TIMER_CHANNEL);
}

