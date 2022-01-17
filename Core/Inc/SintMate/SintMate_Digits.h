/* Digits.h
*  Created on: Feb 4, 2021
 *      Author: bmp2ili
 */
#ifndef INC_SINTMATE_DIGIT_H_
#define INC_SINTMATE_DIGIT_H_

#include "main.h"

extern uint16_t D0red[];
extern uint16_t D0green[];
extern uint16_t D0yellow[];
extern uint16_t D0blue[];
extern uint16_t D0white[];
extern uint16_t D1red[];
extern uint16_t D1green[];
extern uint16_t D1yellow[];
extern uint16_t D1blue[];
extern uint16_t D1white[];
extern uint16_t D2red[];
extern uint16_t D2green[];
extern uint16_t D2yellow[];
extern uint16_t D2blue[];
extern uint16_t D2white[];
extern uint16_t D3red[];
extern uint16_t D3green[];
extern uint16_t D3yellow[];
extern uint16_t D3blue[];
extern uint16_t D3white[];
extern uint16_t D4red[];
extern uint16_t D4green[];
extern uint16_t D4yellow[];
extern uint16_t D4blue[];
extern uint16_t D4white[];
extern uint16_t D5red[];
extern uint16_t D5green[];
extern uint16_t D5yellow[];
extern uint16_t D5blue[];
extern uint16_t D5white[];
extern uint16_t D6red[];
extern uint16_t D6green[];
extern uint16_t D6yellow[];
extern uint16_t D6blue[];
extern uint16_t D6white[];
extern uint16_t D7red[];
extern uint16_t D7green[];
extern uint16_t D7yellow[];
extern uint16_t D7blue[];
extern uint16_t D7white[];
extern uint16_t D8red[];
extern uint16_t D8green[];
extern uint16_t D8yellow[];
extern uint16_t D8blue[];
extern uint16_t D8white[];
extern uint16_t D9red[];
extern uint16_t D9green[];
extern uint16_t D9yellow[];
extern uint16_t D9blue[];
extern uint16_t D9white[];
extern uint16_t RedOFF[];

#define  DIGIT_WIDTH   40
#define  DIGIT_HEIGHT  72
#define  DIGIT_SIZE    2880
#define  BUTTONS_WIDTH   96
#define  BUTTONS_HEIGHT  96
#define  BUTTONS_SIZE  9216
#define  ICONS_WIDTH   48
#define  ICONS_HEIGHT  48
#define  ICONS_SIZE  2304
#define  LOGO_WIDTH   240
#define  LOGO_HEIGHT  42
#define  LOGO_SIZE  10080
#define  BACK_WIDTH   130
#define  BACK_HEIGHT  64
#define  BACK_SIZE  8320

extern   uint16_t DigitOFF[DIGIT_SIZE];
extern   uint16_t SyntMateLogo[LOGO_SIZE];
extern	 uint16_t Digit[10][DIGIT_SIZE];

extern   uint16_t Increase[];
extern   uint16_t Decrease[];
extern   uint16_t Increase_disabled[];
extern   uint16_t Decrease_disabled[];
extern   uint16_t Home[];
extern   uint16_t Settings[];
extern   uint16_t Home_disabled[];
extern   uint16_t Settings_disabled[];
extern   uint16_t play[];
extern   uint16_t stop[];
extern   uint16_t BackGround[];
extern	 uint16_t Plus[];
extern	 uint16_t Minus[];
extern	 uint16_t Back[];

extern	 uint16_t HomingRed[];
extern	 uint16_t HomingBlue[];
extern	 uint16_t HomingGreen[];
extern	 uint16_t HomingBlack[];

#endif /*INC_SINTMATE_DIGIT_H_ */
