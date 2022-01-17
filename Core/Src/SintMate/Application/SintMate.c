/*
 * SintMate.c
 *
 *  Created on: Feb 4, 2021
 *      Author: fil
 */
/*
 Clocks :
 pll1 			:  2  80 2 2 2
 pll2 			: 12 200 2 2 2
 pll3 			: 12  94 4 4 4
 clk spi 1,2,3 	: pll2p 200
 clk spi6      	: pclk4 120
 processor	   	: 480 240 120
 usb			: pll3q 48
 */
#include "main.h"

SystemParametersTypeDef	SystemParameters;
SystemVarDef			SystemVar;
SystemLogsTypeDef		SystemLogs;

void SintMate_SystemSetDefaults(void)
{
	bzero((uint8_t *)&SystemParameters,sizeof(SystemParameters));
	sprintf(SystemParameters.Header,SintMateNAME);
	sprintf(SystemParameters.Version,SintMateVERSION);
	SystemParameters.step_rpm = STEP_SPEED_RPM;
	SystemParameters.max_running_time = DOWNCOUNTER_TYP;
}


void SintMate_Ask_Card(void)
{
	HAL_GPIO_WritePin(NFC0_GPIO_Port, NFC0_Pin, GPIO_PIN_RESET);
}

static void SintMate_Release_Card(void)
{
	HAL_GPIO_WritePin(NFC0_GPIO_Port, NFC0_Pin, GPIO_PIN_SET);
}

uint8_t SintMate_Get_Card(void)
{
	return HAL_GPIO_ReadPin(NFC1_GPIO_Port, NFC1_Pin);
}

void SintMate_PinSetDefaults(void)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port, FLASH_SS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, DIRSTEP_Pin|RST_Pin|DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, ENASTEP_Pin|CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DEBUG_GPIO_Port, DEBUG_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(VL53L0X_RST_GPIO_Port, VL53L0X_RST_Pin, GPIO_PIN_RESET);
	SintMate_Release_Card();
}

extern	uint16_t Logo[];

void SintMateLedCheck(void)
{
uint32_t	i;
uint8_t		r , g , b;

	WS2812_WormReset(5);

	r = 0;
	g = 0x1f;
	b = 0;

	for(i=0;i<NUMLEDS;i++)
	{
		ringled_frame_complete=0;
		WS2812_Worm(r,g,b);
		HAL_Delay(1);
		while(ringled_frame_complete == 0);
	}

	r = 0x1f;
	g = 0;
	b = 0;

	for(i=0;i<NUMLEDS;i++)
	{
		ringled_frame_complete=0;
		WS2812_Worm(r,g,b);
		HAL_Delay(1);
		while(ringled_frame_complete == 0);
	}

	r = 0;
	g = 0;
	b = 0x1f;

	for(i=0;i<NUMLEDS;i++)
	{
		ringled_frame_complete=0;
		WS2812_Worm(r,g,b);
		HAL_Delay(1);
		while(ringled_frame_complete == 0);
	}
	ringled_frame_complete=0;
	WS2812_LedsOff();
	HAL_Delay(1);
	while(ringled_frame_complete == 0);
	ringled_frame_complete=0;
	WS2812_LedsOff();
	HAL_Delay(1);
	while(ringled_frame_complete == 0);
	WS2812_WormReset(WORMLEN);
}




//#define	FORCE_CALIBRATE	1
void Init_SintMate(void)
{
	SintMate_PinSetDefaults();
	SintMateLedCheck();
	ILI9341_Init();
	ILI9341_FillScreen(ILI9341_BLACK);
	HAL_TIM_PWM_Start(&BACKLIGHT_TIMER, BACKLIGHT_TIMER_CHANNEL);
	SetupFlash();
#ifdef FORCE_CALIBRATE
	SystemParameters.touch_is_calibrated = 0;
#endif
	if (( SystemParameters.touch_x_correction == 0 ) || ( SystemParameters.touch_y_correction == 0 ))
		SystemParameters.touch_is_calibrated = 0;
	if ( SystemParameters.touch_is_calibrated == 0 )
	{
		HAL_TIM_Base_Start_IT(&TICK100MS_TIMER);
		ILI9341_calibrate_touch();
		StoreSettingsInFlash();
		HAL_TIM_Base_Stop_IT(&TICK100MS_TIMER);
		HAL_Delay(200);
	}

	SystemVar.Session_DownCounter = SystemParameters.max_running_time;
	GetDigitsFromFlash();
	ILI9341_DrawImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT-1, SyntMateLogo);
	while(SystemVar.lcd_dma_busy == 1);
	InitCounter();
	DrawPlayButtons();
	SystemVar.run_state = RUN_STATE_IDLE;
	SystemVar.homing_time = 0;
	SystemVar.nfc_timeout = 0;
	//SintMate_VL53L0X_Init();
	SystemVar.worm_r = WORM_R_RUNNING;
	SystemVar.worm_g = WORM_G_RUNNING;
	SystemVar.worm_b = WORM_B_RUNNING;
	WS2812_LedsOff();
	HAL_TIM_Base_Start_IT(&TICK100MS_TIMER);
	WS2812_WormReset(WORMLEN);
	HAL_UART_Receive_IT(&huart7, BT_RxBuf, 1);
	SystemLogs.counter = 0;

	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	StepperInit();
	/* FIL */
	//SetCounter(10);
	/* FIL */
}

extern	uint16_t nfc_active[], nfc_inactive[], nfc_fail[], nfc_ok[];


void SintMate_DrawCheckNFC_Active(void)
{
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(NFC_XPOS, NFC_YPOS, ICONS_WIDTH, ICONS_HEIGHT-1, nfc_active);
}

void SintMate_DrawCheckNFC_InActive(void)
{
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(NFC_XPOS, NFC_YPOS, ICONS_WIDTH, ICONS_HEIGHT-1, nfc_inactive);
}

void SintMate_DrawCheckNFC_Fail(void)
{
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(NFC_XPOS, NFC_YPOS, ICONS_WIDTH, ICONS_HEIGHT-1, nfc_fail);
}

void SintMate_DrawCheckNFC_Ok(void)
{
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(NFC_XPOS, NFC_YPOS, ICONS_WIDTH, ICONS_HEIGHT-1, nfc_ok);
}

void SintMate_ClearNFC(void)
{
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_FillRectangle(NFC_XPOS, NFC_YPOS, ICONS_WIDTH, ICONS_HEIGHT-1, ILI9341_BLACK);
}
uint8_t	initial_delay=0 , first_homing=1;

#define	TESTING	1
void SintMateLoop(void)
{
	if ( SystemVar.counter_flag100Ms == 1 )
	{
		initial_delay++;
		if ( initial_delay == 5 )
		{
			SystemVar.run_state = RUN_STATE_HOMING;
			DrawPlayButtons();
			SintMate_VL53L0X_Init();
			SintMate_VL53L0X_StartMeasure();
			SystemVar.MotorHomeHit = 0;
			WS2812_Worm(SystemVar.worm_r,SystemVar.worm_g,SystemVar.worm_b);
			SintMate_do_homing_steps();
		}
		if ( initial_delay > 6 )
			initial_delay = 7;

		SystemVar.homing_counter_1000ms ++;
		if ( SystemVar.homing_counter_1000ms == 50 )
		{
			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
			ILI9341_DrawImage(HOMING_POSX, HOMING_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, HomingBlack);
		}
		if ( SystemVar.homing_counter_1000ms > 50 )
			SystemVar.homing_counter_1000ms = 51;

		if (SystemVar.run_state == RUN_STATE_NFC)
		{
			SintMate_Ask_Card();
			if ( SystemVar.nfc_timeout < NFC_TIMEOUT )
			{
				if (( SystemVar.nfc_timeout & 0x4 ) == 0x4 )
					SintMate_DrawCheckNFC_Active();
				else
					SintMate_DrawCheckNFC_InActive();
			}
			if ( SintMate_Get_Card() == 0 )
			{
				SintMate_ClearNFC();
				SintMate_do_rotations(((OC_PULSES_PER_ROTATION*SystemParameters.step_rpm)/60)*SystemVar.DownCounter);
				SystemVar.run_state = RUN_STATE_RUNNING;
				DrawPlayButtons();
				SystemVar.doubletouch_flag = 0;
				SintMate_Release_Card();
				SystemVar.nfc_timeout = 0;
			}
			else
			{
				SystemVar.nfc_timeout++;
				if ( SystemVar.nfc_timeout > NFC_TIMEOUT )
				{
					SintMate_DrawCheckNFC_Fail();
				}
				if ( SystemVar.nfc_timeout > NFC_ERRORTIMEOUT )
				{
					SintMate_DrawCheckNFC_Ok();
				}
				if ( SystemVar.nfc_timeout > NFC_BACK2NORMAL )
				{
					SystemVar.nfc_timeout = 0;
					SintMate_Release_Card();
#ifndef TESTING
					SystemVar.nfc_timeout = 0;
					DrawIdleButtons();
					SystemVar.run_state = RUN_STATE_IDLE;
#else
					SintMate_DrawCheckNFC_InActive();
					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
					HAL_Delay(500);
					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
					HAL_Delay(200);
					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
					HAL_Delay(500);
					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
					SintMate_do_rotations(((OC_PULSES_PER_ROTATION*SystemParameters.step_rpm)/60)*SystemVar.DownCounter);
					SystemVar.run_state = RUN_STATE_RUNNING;

					DrawPlayButtons();
					SintMate_ClearNFC();
					SystemVar.doubletouch_flag = 0;
#endif
				}
			}
		}

		if (SystemVar.run_state == RUN_STATE_RUNNING)
		{
			if ( SystemVar.counter_flag1000Ms == 1 )
			{
				SystemVar.counter_flag1000Ms = 0;
				DecrementCounter();
			}
			if (ringled_frame_complete == 1)
			{
				ringled_frame_complete=0;
				WS2812_Worm(SystemVar.worm_r,SystemVar.worm_g,SystemVar.worm_b);
			}
			SystemVar.homing_time = SystemVar.homing_timeout = SystemVar.homing_counter_1000ms = 0;
		}
		if  (SystemVar.run_state == RUN_STATE_HOMING)
		{
			SystemVar.homing_time++;
			SystemVar.homing_counter_1000ms ++;
			if ( SystemVar.homing_counter_1000ms > 19 )
			{
				SystemVar.homing_counter_1000ms = 0;
				ILI9341_DrawImage(HOMING_POSX, HOMING_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, HomingBlack);
			}
			if ( SystemVar.homing_counter_1000ms == 10 )
				ILI9341_DrawImage(HOMING_POSX, HOMING_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, HomingBlue);
			if ( SystemVar.homing_time > HOMING_TIME )
			{
				SystemVar.homing_time = 0;
				if (ringled_frame_complete == 1)
				{
					ringled_frame_complete=0;
					WS2812_Worm(SystemVar.worm_r,SystemVar.worm_g,SystemVar.worm_b);
				}
			}
			if ( measure_in_progress == 0 )
			{
				if (( range != 0 ) && ( range < CAVITY_HOLE_SIZE ))
				{
					SystemVar.MotorFlagDistance = range;
					SintMate_stop_rotations();
					if ( first_homing == 0 )
					{
						HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
						HAL_Delay(1000);
						HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
					}
					else
						first_homing = 0;
					SystemVar.DownCounter = SystemVar.Session_DownCounter;
					SetCounter(SystemVar.DownCounter);
					/* FIL */
					SetCounter(10);
					/* FIL */
					SystemVar.run_state = RUN_STATE_IDLE;
					DrawIdleButtons();
					while(ringled_frame_complete == 0);
					ringled_frame_complete=0;
					WS2812_Worm(0,0,0);
					HAL_Delay(1);
					while(ringled_frame_complete == 0);
					ringled_frame_complete=0;
					WS2812_Worm(0,0,0);
					HAL_Delay(1);
					while(ringled_frame_complete == 0);
					ILI9341_DrawImage(HOMING_POSX, HOMING_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, HomingGreen);
				}
				else
					SintMate_VL53L0X_StartMeasure();
			}
			else
			{
				SystemVar.homing_timeout++;
				if ( SystemVar.homing_timeout > HOMING_TOUT )
				{
					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
					SintMate_stop_rotations();
					SystemVar.DownCounter = SystemVar.Session_DownCounter;
					SetCounter(SystemVar.DownCounter);
					SystemVar.run_state = RUN_STATE_IDLE;
					DrawIdleButtons();
					WS2812_Worm(0xff,0,0);
					HAL_Delay(1);
					while(ringled_frame_complete == 0);
					ringled_frame_complete=0;
					WS2812_Worm(0,0,0);
					HAL_Delay(1);
					while(ringled_frame_complete == 0);
					ILI9341_DrawImage(HOMING_POSX, HOMING_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, HomingRed);
					SystemVar.homing_timeout = 0;
				}
			}
		}
		SystemVar.counter_flag100Ms = 0;
	}

	if ( SystemVar.doubletouch_flag == 1 )
	{
		DrawPlayButtons();
		SystemVar.run_state = RUN_STATE_HOMING;
		SintMate_VL53L0X_Init();
		SystemVar.MotorHomeHit = 0;
		WS2812_Worm(SystemVar.worm_r,SystemVar.worm_g,SystemVar.worm_b);
		SintMate_do_homing_steps();
		SystemVar.doubletouch_flag = 0;
	}

	if ( SystemVar.touch_flag == 1 )
	{
		SystemVar.touch_flag = 0;
		if ( ILI9341_GetTouch(&SystemVar.touch_x,&SystemVar.touch_y) != 0 )
		{
			if ( SintMateTouchProcess() == 1 )
			{
				ringled_frame_complete=0;
				WS2812_Worm(SystemVar.worm_r,SystemVar.worm_g,SystemVar.worm_b);
			}
		}
	}

	if ( SystemVar.usb_packet_ready == 1 )
	{
		USB_ImageUploader();
		SystemVar.usb_packet_ready = 0;
	}
	if (SystemVar.run_state == RUN_STATE_SETTINGS)
		SettingsLoop();
#ifdef	INDUSTRY_4_0
	if ( SystemVar.bt_packet_ready == 1 )
	{
		if ( BT_PacketParser() == 0 )
			BT_CheckForErrors(SintMateBtProcess());
		SystemVar.bt_packet_ready = 0;
		HAL_UART_Receive_IT(&huart7, BT_RxBuf, 1);
	}
	if ( SystemVar.bt_tx_queue_not_empty == 1 )
		Bt_SintMateLogDump();
#endif

}




