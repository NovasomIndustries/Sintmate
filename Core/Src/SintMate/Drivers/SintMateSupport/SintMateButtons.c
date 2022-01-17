/*
 * SintMateButtons.c
 *
 *  Created on: Feb 10, 2021
 *      Author: fil
 */

#include "main.h"

void DrawIdleButtons(void)
{
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(PLAYPAUSE_BUTTON_POSX, PLAYPAUSE_BUTTON_POSY, BUTTONS_WIDTH, BUTTONS_HEIGHT-1, play);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(INCREASE_BUTTON_POSX, INCREASE_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Increase);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(DECREASE_BUTTON_POSX, DECREASE_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Decrease);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(SETTINGS_BUTTON_POSX, SETTINGS_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Settings);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(HOME_BUTTON_POSX, HOME_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Home);
}

void DrawPlayButtons(void)
{
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(PLAYPAUSE_BUTTON_POSX, PLAYPAUSE_BUTTON_POSY, BUTTONS_WIDTH, BUTTONS_HEIGHT-1, stop);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(INCREASE_BUTTON_POSX, INCREASE_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Increase_disabled);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(DECREASE_BUTTON_POSX, DECREASE_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Decrease_disabled);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(SETTINGS_BUTTON_POSX, SETTINGS_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Settings_disabled);
	while(SystemVar.lcd_dma_busy == 1);
	ILI9341_DrawImage(HOME_BUTTON_POSX, HOME_BUTTON_POSY, ICONS_WIDTH, ICONS_HEIGHT-1, Home_disabled);
}
