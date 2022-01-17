/*
 * StepperMotor.c
 *
 *  Created on: Mar 12, 2021
 *      Author: fil
 */

#include "main.h"

uint16_t	step_acceleration_tab[OC_TABLE_SIZE];
uint16_t	step_steady_tab[OC_TABLE_SIZE];
uint16_t	step_deceleration_tab[OC_TABLE_SIZE];
uint16_t	step_homing_tab[OC_HOMING_TABLE_SIZE];
uint32_t	step_rotations = 0;
uint32_t	step_phase_state = STEP_IDLE;

void StepperInit(void)
{
uint32_t	i;

	HAL_GPIO_WritePin(DIRSTEP_GPIO_Port, DIRSTEP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIRSTEP_GPIO_Port, DIRSTEP_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_SET);
	/*
	for(i=0;i<OC_TABLE_SIZE;i++)
	{
		if ( (OC_START_VALUE-(i*OC_ACCELERATION_FACTOR)) < OC_END_VALUE)
			 step_acceleration_tab[i] = step_deceleration_tab[OC_TABLE_SIZE-i-1] = OC_END_VALUE;
		else
		{
			step_acceleration_tab[i] = OC_START_VALUE - (i*OC_ACCELERATION_FACTOR);
			step_deceleration_tab[i] = OC_END_VALUE+(i*OC_ACCELERATION_FACTOR);
		}
		step_steady_tab[i] = OC_END_VALUE;
	}
	*/
	for(i=0;i<OC_HOMING_TABLE_SIZE;i++)
		step_homing_tab[i] = OC_HOME_VALUE;
	for(i=0;i<OC_TABLE_SIZE;i++)
		step_acceleration_tab[i] = OC_END_VALUE;
	step_rotations = 0;
}

static void step_start_t(uint32_t tab,uint32_t state)
{
	HAL_TIM_OC_Stop(&htim16, TIM_CHANNEL_1);
	StepperInit();
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_RESET);
	step_phase_state = state;
	htim16.Instance->CNT = 0;
	htim16.Instance->ARR = OC_END_VALUE;
	HAL_TIM_OC_Start(&htim16, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_RESET);
}

static void step_home_t(uint32_t steps)
{
	HAL_TIM_OC_Stop(&htim16, TIM_CHANNEL_1);
	StepperInit();
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_RESET);
	step_phase_state = STEP_HOMING;
	htim16.Instance->CNT = 0;
	htim16.Instance->ARR = OC_HOME_VALUE;
	HAL_TIM_OC_Start(&htim16, TIM_CHANNEL_1);
}

void SintMate_do_rotations(uint32_t rotation_number)
{
	if ( step_phase_state == STEP_IDLE)
	{
		step_rotations = (rotation_number*4)-1;
		step_start_t((uint32_t)&step_acceleration_tab,STEP_ACCELERATION);
		Bt_SintMateLogInsert(SystemParameters.step_rpm,SystemVar.DownCounter,'R');
	}
}

void SintMate_stop_rotations(void)
{
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_SET);
	HAL_TIM_OC_Stop(&htim16, TIM_CHANNEL_1);
	step_phase_state = STEP_IDLE;
	Bt_SintMateLogInsert(SystemParameters.step_rpm,SystemVar.DownCounter,'H');
}

uint32_t	home_hit;
uint32_t SintMate_do_homing_steps(void)
{
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_RESET);
	step_rotations = OC_HOMING_STEPS;
	step_home_t(OC_HOMING_TABLE_SIZE);
	SystemVar.MotorTimeOut = HOMING_TIMEOUT;
	SintMate_VL53L0X_StartMeasure();
#ifdef BOH
	/*
	while(measure_in_progress == 1)
	{
		SystemVar.MotorTimeOut--;
		if ( SystemVar.MotorTimeOut <=2 )
		{
			return 1;
		}
		HAL_Delay(1);
	}
	*/
	if (( range != 0 ) && ( range < CAVITY_HOLE_SIZE ))
	{
		SystemVar.MotorFlagDistance = range;
		SintMate_stop_rotations();
/*
		SystemVar.MotorHomeHit++;
		if ( SystemVar.MotorHomeHit > 2 )
		{
			SystemVar.MotorFlagDistance = range;
			SintMate_stop_rotations();
			return 0;
		}
		*/
	}
	else
		SintMate_VL53L0X_StartMeasure();
#endif
	return 2;
}
