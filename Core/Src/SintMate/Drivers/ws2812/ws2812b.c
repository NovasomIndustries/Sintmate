/*
 * ws2812b.c
 *
 *  Created on: Dec 23, 2020
 *      Author: fil
 */
#include "ws2812b.h"
#include "main.h"

__attribute__	((aligned (16)))	uint16_t	FrameBuffer[BUFLEN][LEDBPP]; /* 0GRB */
uint32_t	worm_index,worm_len;
uint32_t	ringled_frame_complete;

uint32_t WS2812_led_index_from_number(uint32_t led_number)
{
	return led_number*LEDBPP;
}

static void compile_sync(void)
{
int16_t	i,k,location;

	for(location=0;location<SYNCLEN;location++)
	{
		for(i=0,k=7;i<8;i++,k--)
		{
			FrameBuffer[location][k] = 1;
			FrameBuffer[location][k+8] = 1;
			FrameBuffer[location][k+16] = 1;
		}
	}
}

void WS2812_CompilePixel(uint32_t location, uint8_t r,uint8_t g,uint8_t b)
{
int16_t	i,k;
	for(i=0,k=7;i<8;i++,k--)
	{
		if ( (g & (1 << i)) != 0 )
			FrameBuffer[SYNCLEN+location][k] = PATTERN_1;
		else
			FrameBuffer[SYNCLEN+location][k] = PATTERN_0;
		if ( (r & (1 << i)) != 0 )
			FrameBuffer[SYNCLEN+location][k+8] = PATTERN_1;
		else
			FrameBuffer[SYNCLEN+location][k+8] = PATTERN_0;
		if ( (b & (1 << i)) != 0 )
			FrameBuffer[SYNCLEN+location][k+16] = PATTERN_1;
		else
			FrameBuffer[SYNCLEN+location][k+16] = PATTERN_0;
	}
	compile_sync();
}

void WS2812_WritePixel(uint32_t location, uint8_t r,uint8_t g,uint8_t b)
{
	WS2812_CompilePixel(location, r,g,b);
	HAL_TIM_PWM_Start_DMA(&LED_TIMER, LED_TIMER_CHANNEL,(uint32_t *)FrameBuffer,BUFLEN);
}

void WS2812_LedsOff(void)
{
int16_t	location;
	for(location=0;location<NUMLEDS;location++)
		WS2812_CompilePixel(location, 0,0,0);
	HAL_TIM_PWM_Start_DMA(&LED_TIMER, LED_TIMER_CHANNEL,(uint32_t *)FrameBuffer,BUFLEN);
}

void WS2812_Worm(uint8_t r,uint8_t g,uint8_t b)
{
int16_t	i,location;
uint8_t lr=r/worm_len,lg=g/worm_len,lb=b/worm_len;

	for(location=0;location<NUMLEDS;location++)
		WS2812_CompilePixel(location, 0,0,0);

	location = worm_index;
	for(i=0;i<worm_len;i++,location++)
	{
		if ( location >= NUMLEDS)
			location=0;
		WS2812_CompilePixel(location, lr,lg,lb);
		lr += lr;
		lg += lg;
		lb += lb;
	}
	worm_index ++;
	if ( worm_index >= NUMLEDS)
		worm_index=0;
	HAL_TIM_PWM_Start_DMA(&LED_TIMER, LED_TIMER_CHANNEL,(uint32_t *)FrameBuffer,BUFLEN);
}

void WS2812_WormReset(uint32_t length)
{
	worm_index = 0;
	worm_len = length;
	HAL_TIM_PWM_Stop_DMA(&LED_TIMER, LED_TIMER_CHANNEL);

}

void WS2812_LedsInit(void)
{
int16_t	location;
	for(location=SYNCLEN;location<SYNCLEN+NUMLEDS;location++)
		WS2812_CompilePixel(location, 0,0,0);
	HAL_TIM_PWM_Start_DMA(&LED_TIMER, LED_TIMER_CHANNEL,(uint32_t *)FrameBuffer,BUFLEN);
}

