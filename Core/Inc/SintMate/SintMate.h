/*
 * SintMate.h
 *
 *  Created on: Feb 4, 2021
 *      Author: fil
 */

#ifndef INC_SINTMATE_SYSTEM_H_
#define INC_SINTMATE_SYSTEM_H_

#include <stdio.h>
#include <string.h>
#include <SintmateDownCounter.h>

#include <SintMate/DigitsBitmap.h>
#include <SintMate/flash_manager.h>
#include <SintMate/fonts.h>
#include <SintMate/ili9341.h>
#include <SintMate/ili9341_calibrate_touch.h>
#include <SintMate/ili9341_touch.h>
#include <SintMate/ImageUploader.h>
#include <SintMate/SintmateBT.h>
#include <SintMate/SpiFlash.h>
#include <SintMate/StepperMotor.h>
#include <SintMate/VL53L0X.h>
#include <SintMate/VL53L0X_utility.h>
#include <SintMate/ws2812b.h>
#include <SintMate_Digits.h>
#include <SintMateButtons.h>
#include <SintMateDebug.h>
#include <SintMateSettings.h>
#include <SintMateTouchProcess.h>

#define	SintMateNAME		"Sintmate"
#define	SintMateVERSION		"0.0.1"
#define	INDUSTRY_4_0		1
#define	LOGO_Y_POS			10

#define 	DIGIT_BUFFERS 		__attribute__((section(".digit_buffers" ".noinit")))
#define 	XDIGIT_BUFFERS 		__attribute__((section(".xdigit_buffers" ".noinit")))

extern	TIM_HandleTypeDef htim8;
extern	TIM_HandleTypeDef htim16;
extern	TIM_HandleTypeDef htim5;
extern	TIM_HandleTypeDef htim7;
extern	SPI_HandleTypeDef hspi1;
extern	SPI_HandleTypeDef hspi6;
extern	I2C_HandleTypeDef hi2c2;
extern	UART_HandleTypeDef huart7;

#define	LED_TIMER				htim8
#define	LED_TIMER_CHANNEL		TIM_CHANNEL_2
#define	BACKLIGHT_TIMER			htim5
#define	BACKLIGHT_TIMER_CHANNEL	TIM_CHANNEL_1
#define	STEPPER_TIMER			htim16
#define	STEPPER_TIMER_CHANNEL	TIM_CHANNEL_1
#define	TICK100MS_TIMER			htim7

#define	FlashSPIport	hspi6
#define	LcdSPIport		hspi1
#define	TOF_I2C			hi2c2
#define	TOF_ADDRESS		0x52

extern SPI_HandleTypeDef ILI9341_SPI_PORT;


typedef struct _SystemParametersTypeDef
{
	char					Header[16];
	char					Version[16];
	uint32_t 				step_rpm;
	uint32_t 				max_running_time;
	uint32_t 				flash_capacity;
	uint32_t 				touch_is_calibrated;	// 0 not calibrated, !=0 calibrated
	uint32_t 				touch_x_minimum;
	uint32_t 				touch_y_minimum;
	uint32_t 				touch_x_correction;
	uint32_t 				touch_y_correction;
}SystemParametersTypeDef;

#define		RUN_STATE_IDLE		0
#define		RUN_STATE_RUNNING	1
#define		RUN_STATE_WAITING	2
#define		RUN_STATE_NFC		3
#define		RUN_STATE_SETTINGS	4
#define		RUN_STATE_HOMING	5

#define		TOUCH_DISABLE_WINDOW	2	// 500 mSec delay after touch released the touch is considered not active
#define		DOUBLE_TOUCH_TIME	20		// after this time the play pressure stops
#define		USB_BUFLEN			2048

typedef struct _SystemVarDef
{
	uint32_t 				run_state;
	uint32_t 				touch_flag;
	uint32_t 				touch_x, touch_y;
	uint32_t 				touch_pen_down;
	uint32_t 				touch_disable_window;
	uint32_t 				lcd_dma_busy;
	uint32_t 				counter;
	uint32_t 				counter_flag100Ms;
	uint32_t 				counter_flag1000Ms;
	uint32_t 				doubletouch_flag;
	uint32_t 				counter_doubletouch;
	uint32_t 				Session_DownCounter;
	uint32_t 				DownCounter;
	uint32_t 				MotorFlagDistance;
	uint32_t 				MotorTimeOut;
	uint32_t 				MotorHomeHit;
	uint8_t 				suppress_trailing_zero;
	uint32_t 				usb_rxed_byte_count;
	uint32_t 				usb_byte_count;
	uint8_t 				usb_do_cp;
	uint32_t 				usb_pkt_found;
	uint16_t 				usb_rx_index;
	uint8_t 				usb_packet_ready;
	uint8_t 				usb_rxed_packet[USB_BUFLEN];
	uint8_t 				usb_packet[USB_BUFLEN];
	uint16_t 				usb_image_number;
	char 					usb_image_name[32];
	uint8_t 				worm_r;
	uint8_t 				worm_g;
	uint8_t 				worm_b;
	uint8_t 				bt_packet_ready;
	uint8_t 				bt_rxbuf_index;
	uint8_t 				bt_tx_complete;
	uint8_t 				bt_tx_queue_not_empty;
	uint8_t 				bt_cmd;
	uint8_t 				bt_param;
	uint8_t 				homing_time;
	uint8_t 				homing_timeout;
	uint8_t 				homing_counter_1000ms;
	uint8_t 				nfc_timeout;
	uint8_t 				start_nfc;
}SystemVarDef;

#define	HOMING_TIME			9
#define	HOMING_TOUT			100
#define	NFC_TIMEOUT			50
#define	NFC_ERRORTIMEOUT	80
#define	NFC_BACK2NORMAL		81
#define	LOG_MAX_DEPTH		1024

typedef struct _SystemLogsTypeDef
{
	uint32_t 				counter;
	uint32_t 				sent;
	char	 				action[LOG_MAX_DEPTH];
	uint32_t 				speed[LOG_MAX_DEPTH];
	uint32_t 				time[LOG_MAX_DEPTH];
}SystemLogsTypeDef;


#define	WORM_R_RUNNING	0x00
#define	WORM_G_RUNNING	0x1f
#define	WORM_B_RUNNING	0x00

#define	WORM_R_FINISHING	0x00
#define	WORM_G_FINISHING	0x00
#define	WORM_B_FINISHING	0x1f

#define	WORM_R_ERROR		0x1f
#define	WORM_G_ERROR		0x00
#define	WORM_B_ERROR		0x00

#define	WORMLEN				6

extern	SystemParametersTypeDef	SystemParameters;
extern	SystemVarDef			SystemVar;
extern	SystemLogsTypeDef		SystemLogs;

extern	void Init_SintMate(void);
extern	void SintMateLoop(void);
extern	void SintMate_SystemSetDefaults(void);


#define	DIGIT_GREEN			1
#define	DIGIT_RED			1
#define	DIGIT_YELLOW		1

#endif /* INC_SINTMATE_SYSTEM_H_ */
