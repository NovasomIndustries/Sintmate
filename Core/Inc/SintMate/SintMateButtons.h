/*
 * SintMateButtons.h
 *
 *  Created on: Feb 10, 2021
 *      Author: fil
 */

#ifndef INC_SINTMATE_SINTMATEBUTTONS_H_
#define INC_SINTMATE_SINTMATEBUTTONS_H_

#define	PLAYPAUSE_BUTTON_POSX		((ILI9341_WIDTH-BUTTONS_WIDTH)/2)
#define	PLAYPAUSE_BUTTON_POSY		220
#define	INCREASE_BUTTON_POSX		2
#define	INCREASE_BUTTON_POSY		140
#define	DECREASE_BUTTON_POSX		190
#define	DECREASE_BUTTON_POSY		INCREASE_BUTTON_POSY
#define	SETTINGS_BUTTON_POSX		2
#define	SETTINGS_BUTTON_POSY		60
#define	HOME_BUTTON_POSX			190
#define	HOME_BUTTON_POSY			SETTINGS_BUTTON_POSY
#define	BACK_BUTTON_POSX			((ILI9341_WIDTH-BACK_WIDTH)/2)
#define	BACK_BUTTON_POSY			240
#define	NFC_XPOS 					((ILI9341_WIDTH-ICONS_WIDTH)/2)
#define	NFC_YPOS 					80
#define	HOMING_POSX					190
#define	HOMING_POSY					220


extern	void DrawIdleButtons(void);
extern	void DrawPlayButtons(void);

#endif /* INC_SINTMATE_SINTMATEBUTTONS_H_ */
