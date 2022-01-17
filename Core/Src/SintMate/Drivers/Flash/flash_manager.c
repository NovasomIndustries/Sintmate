/*
 * flash_manager.c
 *
 *  Created on: Feb 5, 2021
 *      Author: fil
 */

#include "main.h"

void StoreSettingsInFlash(void)
{
	flash_EraseSector(HEADER_SECTOR);
	flash_WriteBytes((uint8_t *)&SystemParameters ,flash_SectorToAddress(HEADER_SECTOR),sizeof(SystemParameters));
}

void LoadSettingsFromFlash(void)
{
	flash_ReadBytes((uint8_t *)&SystemParameters,flash_SectorToAddress(HEADER_SECTOR),sizeof(SystemParameters));
}

uint32_t	FlashIs256	= 0;
#define BOOT_Y_DELTA	20
uint32_t SetupFlash(void)
{
uint32_t	FlashID,ypos=0,i;

	ILI9341_WriteString(0, ypos, "SintMate booting ...", Font_7x10, ILI9341_BLUE, ILI9341_BLACK);
	ypos += BOOT_Y_DELTA;
	ILI9341_WriteString(0, ypos, "Checking FLASH ...", Font_7x10, ILI9341_BLUE, ILI9341_BLACK);

	for(i=0;i<3;i++)
	{
		HAL_Delay(10);
		FlashID = flash_ReadID();
	}
	if (( FlashID & 0xff ) == F128 )	/* 128 MBits */
	{
		SystemParameters.flash_capacity = F128;
		ILI9341_WriteString(100, ypos, "128 MBits", Font_7x10, ILI9341_BLUE, ILI9341_BLACK);
	}
	else if (( FlashID & 0xff ) == F256 )	/* 256 MBits */
	{
		SystemParameters.flash_capacity = F256;
		ILI9341_WriteString(130, ypos, "256 MBits", Font_7x10, ILI9341_BLUE, ILI9341_BLACK);
	}
	else
	{
		ILI9341_WriteString(100, ypos, "Failed!!", Font_7x10, ILI9341_RED, ILI9341_BLACK);
		HAL_Delay(1000);
		return -1;
	}
	HAL_Delay(10);
	ypos += BOOT_Y_DELTA;
	ILI9341_WriteString(0, ypos, "Loading params ...", Font_7x10, ILI9341_BLUE, ILI9341_BLACK);
	LoadSettingsFromFlash();
	if ( strncmp(SystemParameters.Header,SintMateNAME,strlen(SintMateNAME) ) != 0 )
	{
		ILI9341_WriteString(100, ypos, "Invalid", Font_7x10, ILI9341_RED, ILI9341_BLACK);
		ypos += BOOT_Y_DELTA;
		ILI9341_WriteString(0, ypos, "Setting defaults ...", Font_7x10, ILI9341_RED, ILI9341_BLACK);
		HAL_Delay(10);
		SintMate_SystemSetDefaults();
		StoreSettingsInFlash();
		LoadSettingsFromFlash();
	}
	ILI9341_WriteString(100, ypos, "Done", Font_7x10, ILI9341_BLUE, ILI9341_BLACK);
	ypos += BOOT_Y_DELTA;
	ILI9341_WriteString(0, ypos, "Starting ...", Font_7x10, ILI9341_BLUE, ILI9341_BLACK);
	HAL_Delay(100);
	ILI9341_FillScreen(ILI9341_BLACK);
	return SystemParameters.flash_capacity;
}





