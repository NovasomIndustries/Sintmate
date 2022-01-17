/*
 * ws2812b.h
 *
 *  Created on: Dec 23, 2020
 *      Author: fil
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_
#include "main.h"

#define	LEDBPP		24
#define	NUMLEDS		45

#define	LED_TIMER			htim8
#define	LED_TIMER_CHANNEL	TIM_CHANNEL_2
/* 1 led is (1.5uSec. * 24 ) , pause must be > 50 uSec , so SYNCLEN is at least 2 leds */
//#define	SYNCLEN		50
#define	SYNCLEN		50
#define	BUFLEN		((SYNCLEN*LEDBPP)+(NUMLEDS*LEDBPP))

#define	TIMER_COUNTS	24
//#define	PATTERN_0		35
//#define	PATTERN_1		75

#define	PATTERN_0		35
#define	PATTERN_1		80

#define	GREEN_SHIFT		16
#define	RED_SHIFT		8
#define	BLUE_SHIFT		0

extern	uint16_t	FrameBuffer[BUFLEN][24]; /* 0GRB */
extern	uint32_t	ringled_frame_complete;

extern	void WS2812_LedsInit(void);
extern	uint32_t WS2812_led_index_from_number(uint32_t led_number);
extern	void WS2812_WritePixel(uint32_t location, uint8_t r,uint8_t g,uint8_t b);
extern	void WS2812_LedsOff(void);
extern	void WS2812_Worm(uint8_t r,uint8_t g,uint8_t b);
extern	void WS2812_WormReset(uint32_t length);

#endif /* INC_WS2812B_H_ */
