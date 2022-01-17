/*
 * ili9341_calibrate_touch.c
 *
 *  Created on: Feb 4, 2021
 *      Author: fil
 */

#include "main.h"
#include <string.h>
#include <stdio.h>

#define	TOUCH_OFFSET_X_Y	15
#define	TOUCH_CIRCLE_RADIUS	5
#define	CALIBRATION_STRING_LEN	24
#define	CALIBRATION_STRING_XPOS	30
#define	CALIBRATION_STRING_YPOS	30

uint32_t 	raw_calibration_touch_x[5], raw_calibration_touch_y[5];
uint32_t 	calibration_touch_x, calibration_touch_y;
//uint32_t	x_minimum,x_correction,y_minimum,y_correction;

static void ILI9341_draw_calibrate(uint16_t step,uint16_t color)
{
	switch ( step)
	{
	case 0 : ILI9341_DrawCircle(TOUCH_OFFSET_X_Y,TOUCH_OFFSET_X_Y,TOUCH_CIRCLE_RADIUS,color); break;
	case 1 : ILI9341_DrawCircle(ILI9341_WIDTH-1-TOUCH_OFFSET_X_Y,TOUCH_OFFSET_X_Y,TOUCH_CIRCLE_RADIUS,color);break;
	case 2 : ILI9341_DrawCircle(ILI9341_WIDTH-1-TOUCH_OFFSET_X_Y,ILI9341_HEIGHT-1-TOUCH_OFFSET_X_Y,TOUCH_CIRCLE_RADIUS,color);break;
	case 3 : ILI9341_DrawCircle(TOUCH_OFFSET_X_Y,ILI9341_HEIGHT-1-TOUCH_OFFSET_X_Y,TOUCH_CIRCLE_RADIUS,color);break;
	case 4 : ILI9341_DrawCircle(ILI9341_WIDTH/2,ILI9341_HEIGHT/2,TOUCH_CIRCLE_RADIUS,color);break;
 	}
}

void ILI9341_calibrate_touch(void)
{
uint16_t	i;
uint32_t	xmin,ymin,xmax,ymax;
char		calibration_string[CALIBRATION_STRING_LEN];

	SystemVar.touch_flag = 0;
	ILI9341_FillScreen(ILI9341_BLACK);
	ILI9341_DrawLine(1,1,ILI9341_WIDTH-2,1,ILI9341_RED);
	ILI9341_DrawLine(1,ILI9341_HEIGHT-2,ILI9341_WIDTH-2,ILI9341_HEIGHT-2,ILI9341_RED);
	ILI9341_DrawLine(1,1,1,ILI9341_HEIGHT-2,ILI9341_RED);
	ILI9341_DrawLine(ILI9341_WIDTH-2,1,ILI9341_WIDTH-2,ILI9341_HEIGHT-2,ILI9341_RED);

	for(i=0;i<5;i++)
	{
		ILI9341_draw_calibrate(i,ILI9341_WHITE);
		while ( SystemVar.touch_flag == 0 );
		ILI9341_draw_calibrate(i,ILI9341_BLUE);
		if ( ILI9341_TouchGetCoordinates(&raw_calibration_touch_x[i],&raw_calibration_touch_y[i]) != 0 )
		{
			sprintf(calibration_string,"X : %d , y : %d",(int )raw_calibration_touch_x[i],(int )raw_calibration_touch_y[i]);
			ILI9341_WriteString(CALIBRATION_STRING_XPOS, CALIBRATION_STRING_YPOS, calibration_string, Font_7x10, ILI9341_WHITE, ILI9341_BLACK);
			HAL_Delay(1000);
			ILI9341_FillRectangle(CALIBRATION_STRING_XPOS, CALIBRATION_STRING_YPOS, CALIBRATION_STRING_LEN*7, 10, ILI9341_BLACK);
		}
		SystemVar.touch_flag = 0;
		ILI9341_draw_calibrate(i,ILI9341_GREEN);
	}
	ILI9341_FillScreen(ILI9341_BLACK);

	xmin = (raw_calibration_touch_x[0] + raw_calibration_touch_x[3]) / 2;
	ymax = (raw_calibration_touch_y[0] + raw_calibration_touch_y[1]) / 2;
	xmax = (raw_calibration_touch_x[1] + raw_calibration_touch_x[2]) / 2;
	ymin = (raw_calibration_touch_y[2] + raw_calibration_touch_y[3]) / 2;
	SystemParameters.touch_x_minimum = xmin;
	SystemParameters.touch_y_minimum = ymin;
	SystemParameters.touch_x_correction = (xmax - xmin) / (ILI9341_WIDTH  - 2*TOUCH_OFFSET_X_Y);
	SystemParameters.touch_y_correction = (ymax - ymin) / (ILI9341_HEIGHT - 2*TOUCH_OFFSET_X_Y);
	SystemParameters.touch_is_calibrated = 1;

}

