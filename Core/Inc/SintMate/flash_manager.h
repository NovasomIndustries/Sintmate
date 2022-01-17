/*
 * flash_manager.h
 *
 *  Created on: Feb 5, 2021
 *      Author: fil
 */

#ifndef INC_SINTMATE_FLASH_MANAGER_H_
#define INC_SINTMATE_FLASH_MANAGER_H_

#include <SintMate/SpiFlash.h>

#define	HEADER_SECTOR			0x000
#define	DIGITOFF_BASE			0x001
#define	LOGO_BASE				0x010
#define	DIGIT0_BASE				0x020
#define	DIGIT1_BASE				0x022
#define	DIGIT2_BASE				0x024
#define	DIGIT3_BASE				0x026
#define	DIGIT4_BASE				0x028
#define	DIGIT5_BASE				0x02A
#define	DIGIT6_BASE				0x02C
#define	DIGIT7_BASE				0x02E
#define	DIGIT8_BASE				0x030
#define	DIGIT9_BASE				0x032
#define	INCREASE_BASE			0x040
#define	INCREASE_DISABLED_BASE	0x042
#define	DECREASE_BASE			0x044
#define	DECREASE_DISABLED_BASE	0x046
#define	SETTINGS_BASE			0x048
#define	SETTINGS_DISABLED_BASE	0x04a
#define	HOME_BASE				0x04c
#define	HOME_DISABLED_BASE		0x04e
#define	PLUS_BASE				0x050
#define	MINUS_BASE				0x052
#define	BACK2NORMAL_BASE		0x060
#define	PLAY_BASE				0x070
#define	STOP_BASE				0x080
//#define	BACKGROUND_BASE			0x090
#define	HOMING_GREEN_BASE		0x090
#define	HOMING_RED_BASE			0x092
#define	HOMING_BLUE_BASE		0x094
#define	HOMING_BLACK_BASE		0x096

#define	DIGITSECTORPERCOLOR		32


extern	uint32_t SetupFlash(void);
extern	void StoreSettingsInFlash(void);
extern	void LoadSettingsFromFlash(void);

#endif /* INC_SINTMATE_FLASH_MANAGER_H_ */
