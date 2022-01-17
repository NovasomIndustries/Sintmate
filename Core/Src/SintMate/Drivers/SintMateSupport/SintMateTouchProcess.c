/*
 * SintMateTouchProcess.c
 *
 *  Created on: Feb 10, 2021
 *      Author: fil
 */

#include "main.h"


static uint32_t play_stop_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_PLAYPAUSE) &&
			( SystemVar.touch_x < TOUCH_X1AREA_PLAYPAUSE) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_PLAYPAUSE) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_PLAYPAUSE))
		return 1;
	return 0;
}

static uint32_t increase_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_INCREASE) &&
			( SystemVar.touch_x < TOUCH_X1AREA_INCREASE) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_INCREASE) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_INCREASE))
		return 1;
	return 0;
}

static uint32_t decrease_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_DECREASE) &&
			( SystemVar.touch_x < TOUCH_X1AREA_DECREASE) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_DECREASE) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_DECREASE))
		return 1;
	return 0;
}

static uint32_t settings_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_SETTINGS) &&
			( SystemVar.touch_x < TOUCH_X1AREA_SETTINGS) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_SETTINGS) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_SETTINGS))
		return 1;
	return 0;
}

static uint32_t home_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_HOME) &&
			( SystemVar.touch_x < TOUCH_X1AREA_HOME) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_HOME) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_HOME))
		return 1;
	return 0;
}

static uint32_t back_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_BACK) &&
			( SystemVar.touch_x < TOUCH_X1AREA_BACK) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_BACK) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_BACK))
		return 1;
	return 0;
}
/*
static uint32_t rpmplus_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_RPMPLUS) &&
			( SystemVar.touch_x < TOUCH_X1AREA_RPMPLUS) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_RPMPLUS) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_RPMPLUS))
		return 1;
	return 0;
}

static uint32_t rpmminus_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_RPMMINUS) &&
			( SystemVar.touch_x < TOUCH_X1AREA_RPMMINUS) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_RPMMINUS) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_RPMMINUS))
		return 1;
	return 0;
}
*/
static uint32_t secondplus_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_SECPLUS) &&
			( SystemVar.touch_x < TOUCH_X1AREA_SECPLUS) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_SECPLUS) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_SECPLUS))
		return 1;
	return 0;
}

static uint32_t secondminus_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_SECMINUS) &&
			( SystemVar.touch_x < TOUCH_X1AREA_SECMINUS) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_SECMINUS) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_SECMINUS))
		return 1;
	return 0;
}

static uint32_t calibratetouch_button_check(void)
{
	if (( SystemVar.touch_x > TOUCH_X0AREA_CALIBRATE) &&
			( SystemVar.touch_x < TOUCH_X1AREA_CALIBRATE) &&
			( SystemVar.touch_y > TOUCH_Y0AREA_CALIBRATE) &&
			( SystemVar.touch_y < TOUCH_Y1AREA_CALIBRATE))
		return 1;
	return 0;
}


uint32_t SintMateTouchProcess(void)
{
uint32_t	ret_val=0;	// returns 1 only if system changes state from stop to run
uint32_t	callback_id = TOUCH_CALLBACK_NOTOUCH;

	switch(SystemVar.run_state)
	{
	case	RUN_STATE_RUNNING	:
		if (play_stop_button_check() == 1)
		{
			SystemVar.counter_doubletouch = DOUBLE_TOUCH_TIME;
			SystemVar.run_state = RUN_STATE_WAITING;
			SintMate_stop_rotations();
			DrawIdleButtons();
		}
		break;
	case	RUN_STATE_WAITING	:
		if (play_stop_button_check() == 1)
		{
			SystemVar.counter_doubletouch = 0;
			SintMate_do_rotations(((OC_PULSES_PER_ROTATION*SystemParameters.step_rpm)/60)*SystemVar.DownCounter);
			SystemVar.run_state = RUN_STATE_RUNNING;
			DrawPlayButtons();
		}
		break;
	case	RUN_STATE_NFC:
		break;
	case	RUN_STATE_IDLE	:
		if (play_stop_button_check() == 1)
		{
			DrawPlayButtons();
			SystemVar.run_state = RUN_STATE_NFC;
		}
		if (increase_button_check() == 1)
		{
			if ( SystemVar.DownCounter < SystemParameters.max_running_time )
			{
				SystemVar.DownCounter /=10;
				SystemVar.DownCounter ++;
				SystemVar.DownCounter *=10;
			}
			SetCounter(SystemVar.DownCounter);
			SystemVar.Session_DownCounter = SystemVar.DownCounter;
		}
		if (decrease_button_check() == 1)
		{
			if ( SystemVar.DownCounter >= 20 )
			{
				SystemVar.DownCounter /=10;
				SystemVar.DownCounter --;
				SystemVar.DownCounter *=10;
			}
			SetCounter(SystemVar.DownCounter);
			SystemVar.Session_DownCounter = SystemVar.DownCounter;

		}
		if (home_button_check() == 1)
		{
			DrawPlayButtons();
			SystemVar.run_state = RUN_STATE_HOMING;
			SintMate_VL53L0X_Init();
			SystemVar.MotorHomeHit = 0;
			WS2812_Worm(SystemVar.worm_r,SystemVar.worm_g,SystemVar.worm_b);
			SintMate_do_homing_steps();
		}
		if (settings_button_check() == 1)
		{
			SystemVar.run_state = RUN_STATE_SETTINGS;
			SettingsEnter();
		}
		break;
	case	RUN_STATE_HOMING	:
		break;

	case	RUN_STATE_SETTINGS	:
		callback_id = TOUCH_CALLBACK_NOTOUCH;
		if (secondplus_button_check() == 1)
			callback_id=TOUCH_CALLBACK_SECPLUS;
		if (secondminus_button_check() == 1)
			callback_id=TOUCH_CALLBACK_SECMINUS;
		if (calibratetouch_button_check() == 1)
			callback_id=TOUCH_CALLBACK_CALIBRATE;
		if ( back_button_check() == 1)
			callback_id=TOUCH_CALLBACK_BACK;
		if ( callback_id != 0 )
			SettingsCallBack(callback_id);
		break;
	}
	return ret_val;
}
