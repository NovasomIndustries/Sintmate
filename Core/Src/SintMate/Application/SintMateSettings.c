/*
 * SintMateSettings.c
 *
 *  Created on: Feb 19, 2021
 *      Author: fil
 */

#include "main.h"

#define	LINE_YPOS_DELTA	64
#define	TEXT_YPOS_DELTA	6
#define	NUM_LINES		4
#define	XGAP			8
#define	ICONS_YMAX		LINE_YPOS_DELTA*NUM_LINES

static uint32_t	params_modified=0 , touch_toggle_state = 0,calibrate_xpos , calibrate_ypos;

static void DrawIcons(uint32_t x0,uint32_t x1)
{
uint32_t	i = 1;
/*
	for(i=1;i<3;i++)
	{
	*/
		ILI9341_DrawImage(x0, i*LINE_YPOS_DELTA-8, ICONS_WIDTH, ICONS_HEIGHT-1, Minus);
		while(SystemVar.lcd_dma_busy == 1);
		ILI9341_DrawImage(x1, i*LINE_YPOS_DELTA-8, ICONS_WIDTH, ICONS_HEIGHT-1, Plus);
		while(SystemVar.lcd_dma_busy == 1);
	/* } */
	ILI9341_DrawImage(BACK_BUTTON_POSX, BACK_BUTTON_POSY, BACK_WIDTH, BACK_HEIGHT-1, Back);
	while(SystemVar.lcd_dma_busy == 1);
}

void SettingsDisplayTouchCalibrate(uint32_t xpos,uint32_t ypos)
{
char		message[32];
	sprintf(message,"Calibrate Touch");
	ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_RED, ILI9341_BLACK);
}

void SettingsDisplayNoTouchCalibrate(uint32_t xpos,uint32_t ypos)
{
char		message[32];
	sprintf(message,"   Touch OK    ");
	ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_BLUE, ILI9341_GREEN);
}

void SettingsEnter(void)
{
uint32_t	xpos=0,ypos=0,icon_xposl=0 , icon_xposr=ILI9341_WIDTH-ICONS_WIDTH;
char		message[32];
	ILI9341_FillScreen(ILI9341_BLACK);
	while(SystemVar.lcd_dma_busy == 1);
	xpos = ICONS_WIDTH+XGAP;
	ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, "Settings", Font_16x26, ILI9341_BLUE, ILI9341_BLACK);
	/*
	ypos +=	LINE_YPOS_DELTA;
	sprintf(message,"RPM : %d",(int )SystemParameters.step_rpm);
	ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_BLUE, ILI9341_BLACK);
	*/
	ypos +=	LINE_YPOS_DELTA;
	sprintf(message,"Time : %d",(int )SystemParameters.max_running_time);
	ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_BLUE, ILI9341_BLACK);
	ypos +=	LINE_YPOS_DELTA;
	SettingsDisplayNoTouchCalibrate(xpos,ypos);
	calibrate_xpos = xpos;
	calibrate_ypos = ypos;
	ypos +=	LINE_YPOS_DELTA;
	DrawIcons(icon_xposl,icon_xposr);
}

void SettingsLoop(void)
{

}

void SettingsCallBack(uint32_t callback_id)
{
uint32_t	xpos=ICONS_WIDTH+XGAP,ypos=LINE_YPOS_DELTA;
char		message[32];

	switch(callback_id)
	{
	/*
	case	TOUCH_CALLBACK_RPMPLUS	:
		if ( SystemParameters.step_rpm < MAX_STEP_SPEED_RPM )
		{
			SystemParameters.step_rpm++;
			sprintf(message,"RPM : %d",(int )SystemParameters.step_rpm);
			ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_BLUE, ILI9341_BLACK);
			params_modified = 1;
		}
		break;
	case	TOUCH_CALLBACK_RPMMINUS	:
		if ( SystemParameters.step_rpm > MIN_STEP_SPEED_RPM )
		{
			SystemParameters.step_rpm--;
			sprintf(message,"RPM : %d",(int )SystemParameters.step_rpm);
			ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_BLUE, ILI9341_BLACK);
			params_modified = 1;
		}
		break;
		*/
	case	TOUCH_CALLBACK_SECPLUS	:
		if ( SystemParameters.max_running_time < DOWNCOUNTER_MAX )
		{
			SystemParameters.max_running_time++;
			//ypos +=	LINE_YPOS_DELTA;
			sprintf(message,"Time : %d",(int )SystemParameters.max_running_time);
			ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_BLUE, ILI9341_BLACK);
			params_modified = 1;
		}
		break;
	case	TOUCH_CALLBACK_SECMINUS	:
		if ( SystemParameters.max_running_time > DOWNCOUNTER_MIN )
		{
			SystemParameters.max_running_time--;
			//ypos +=	LINE_YPOS_DELTA;
			sprintf(message,"Time : %d",(int )SystemParameters.max_running_time);
			ILI9341_WriteString(xpos, ypos+TEXT_YPOS_DELTA, message, Font_11x18, ILI9341_BLUE, ILI9341_BLACK);
			params_modified = 1;
		}
		break;
	case	TOUCH_CALLBACK_CALIBRATE	:
		touch_toggle_state++;
		touch_toggle_state &= 1;
		if ( touch_toggle_state == 1 )
		{
			SettingsDisplayTouchCalibrate(calibrate_xpos,calibrate_ypos);
			params_modified = 1;
		}
		else
		{
			SettingsDisplayNoTouchCalibrate(calibrate_xpos,calibrate_ypos);
			params_modified = 0;
		}
		break;
	case	TOUCH_CALLBACK_BACK	:
		if ( params_modified == 1 )
		{
			params_modified = 0;
			StoreSettingsInFlash();
		}
		SettingsExit();
		break;
	}
}


void SettingsExit(void)
{
	ILI9341_FillScreen(ILI9341_BLACK);
	while(SystemVar.lcd_dma_busy == 1);
	SystemVar.Session_DownCounter = SystemParameters.max_running_time;
	ILI9341_DrawImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT-1, SyntMateLogo);
	while(SystemVar.lcd_dma_busy == 1);
	InitCounter();
	DrawIdleButtons();
	SystemVar.run_state = RUN_STATE_IDLE;

}
