/*
 * ImageUploader.c
 *
 *  Created on: Feb 15, 2021
 *      Author: fil
 */

#include "main.h"

#define	BKG_BUF_SIZE_IN_BYTES	32768
uint8_t		usb_uploader_tx_buf[64];
uint8_t		image_buffer[BKG_BUF_SIZE_IN_BYTES];
uint32_t	usb_rxed_byte_count , usb_byte_count, usb_rx_index , usb_bytes_to_receive,restart_flag , stored_image;
uint32_t	error=0;

extern	uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

void default_case(void)
{
	SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
	bzero(SystemVar.usb_packet,USB_BUFLEN);
	SystemVar.usb_byte_count = SystemVar.usb_rx_index = SystemVar.usb_rxed_byte_count = 0;
	SystemVar.usb_do_cp=0;
	usb_rxed_byte_count = usb_byte_count = usb_rx_index = 0;
}

void usbusb_error(void)
{
	error++;
}

uint32_t	analyze_usb_buf(void)
{
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit0") == 0 )
			return DIGIT0_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit1") == 0 )
			return DIGIT1_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit2") == 0 )
			return DIGIT2_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit3") == 0 )
			return DIGIT3_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit4") == 0 )
			return DIGIT4_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit5") == 0 )
			return DIGIT5_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit6") == 0 )
			return DIGIT6_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit7") == 0 )
			return DIGIT7_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit8") == 0 )
			return DIGIT8_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "digit9") == 0 )
			return DIGIT9_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "settings") == 0 )
			return SETTINGS_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "settings_disabled") == 0 )
			return SETTINGS_DISABLED_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "home") == 0 )
			return HOME_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "home_disabled") == 0 )
			return HOME_DISABLED_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "increase") == 0 )
			return INCREASE_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "increase_disabled") == 0 )
			return INCREASE_DISABLED_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease") == 0 )
			return DECREASE_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease_disabled") == 0 )
			return DECREASE_DISABLED_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "SyntMateLogo") == 0 )
			return LOGO_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "DigitOff") == 0 )
			return DIGITOFF_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "play") == 0 )
			return PLAY_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "stop") == 0 )
			return STOP_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "minus") == 0 )
			return MINUS_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "plus") == 0 )
			return PLUS_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "back") == 0 )
			return BACK2NORMAL_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "back") == 0 )
			return BACK2NORMAL_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "homing_green") == 0 )
			return HOMING_GREEN_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "homing_red") == 0 )
			return HOMING_RED_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "homing_blue") == 0 )
			return HOMING_BLUE_BASE;
		if ( strcmp ( (char *)SystemVar.usb_image_name , "homing_black") == 0 )
			return HOMING_BLACK_BASE;
		return 0;
}

typedef struct _SUploaderPtrs
{
	char					usb_image_name[32];
	uint32_t				array_ptr;
	uint32_t 				size;
}SUploaderPtrs;

const SUploaderPtrs UploaderPtrs[] =
{
		{
				.usb_image_name  = "SyntMateLogo",
				.array_ptr = (uint32_t	)&SyntMateLogo,
				.size = LOGO_SIZE
		},
		{
				.usb_image_name  = "settings",
				.array_ptr = (uint32_t	)&Settings,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "settings_disabled",
				.array_ptr = (uint32_t	)&Settings_disabled,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "home",
				.array_ptr = (uint32_t	)&Home,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "home_disabled",
				.array_ptr = (uint32_t	)&Home_disabled,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "increase",
				.array_ptr = (uint32_t	)&Increase,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "increase_disabled",
				.array_ptr = (uint32_t	)&Increase_disabled,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "decrease",
				.array_ptr = (uint32_t	)&Decrease,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "decrease_disabled",
				.array_ptr = (uint32_t	)&Decrease_disabled,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "plus",
				.array_ptr = (uint32_t	)&Plus,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "minus",
				.array_ptr = (uint32_t	)&Minus,
				.size = ICONS_SIZE
		},
		{
				.usb_image_name  = "play",
				.array_ptr = (uint32_t	)&play,
				.size = BUTTONS_SIZE
		},
		{
				.usb_image_name  = "back",
				.array_ptr = (uint32_t	)&Back,
				.size = BACK_SIZE
		},
		{
				.usb_image_name  = "stop",
				.array_ptr = (uint32_t	)&stop,
				.size = BUTTONS_SIZE
		},
		{
				.usb_image_name  = "DigitOff",
				.array_ptr = (uint32_t	)&DigitOFF,
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit0",
				.array_ptr = (uint32_t	)&Digit[0],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit1",
				.array_ptr = (uint32_t	)&Digit[1],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit2",
				.array_ptr = (uint32_t	)&Digit[2],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit3",
				.array_ptr = (uint32_t	)&Digit[3],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit4",
				.array_ptr = (uint32_t	)&Digit[4],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit5",
				.array_ptr = (uint32_t	)&Digit[5],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit6",
				.array_ptr = (uint32_t	)&Digit[6],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit7",
				.array_ptr = (uint32_t	)&Digit[7],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit8",
				.array_ptr = (uint32_t	)&Digit[8],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "digit9",
				.array_ptr = (uint32_t	)&Digit[9],
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "homing_green",
				.array_ptr = (uint32_t	)&HomingGreen,
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "homing_red",
				.array_ptr = (uint32_t	)&HomingRed,
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "homing_blue",
				.array_ptr = (uint32_t	)&HomingBlue,
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "homing_black",
				.array_ptr = (uint32_t	)&HomingBlack,
				.size = DIGIT_SIZE
		},
		{
				.usb_image_name  = "endflag",
				.array_ptr = 0,
				.size = 0
		},
};

static uint32_t lookup(char *name, uint32_t *array_ptr)
{
uint32_t	i=0;
	while( UploaderPtrs[i].size != 0 )
	{
		if ( strcmp(name,UploaderPtrs[i].usb_image_name) == 0 )
		{
			*array_ptr = (uint32_t )UploaderPtrs[i].array_ptr;
			return UploaderPtrs[i].size;
		}
		i++;
	}
	return 0;
}

void ReceiveUSBPacket(void)
{
uint32_t	i,j,size,array_ptr32;
uint16_t	*array_ptr;

	for(i=0;i<SystemVar.usb_rxed_byte_count;i+=2)
	{
		SystemVar.usb_image_number = 0;
		image_buffer[usb_rx_index] =  ((SystemVar.usb_rxed_packet[i] & 0x0f) << 4)|(SystemVar.usb_rxed_packet[i+1] & 0x0f);
		usb_rx_index ++;
		if ( usb_rx_index > usb_bytes_to_receive-1)
		{
			usb_rx_index = 0;
			if (( size = lookup(SystemVar.usb_image_name,&array_ptr32)) != 0 )
			{
				array_ptr = (uint16_t	*)array_ptr32;
				for(j=0;j<usb_bytes_to_receive;j+=2)
				{
					*array_ptr = (image_buffer[j]<<8) | image_buffer[j+1];
					usb_rx_index++;
					array_ptr ++;
				}
				SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
				SystemVar.usb_rx_index  = 0;
				bzero(usb_uploader_tx_buf,64);
				sprintf((char *)usb_uploader_tx_buf,"RECEIVED %d %d",(int )SystemVar.usb_image_number,(int )usb_bytes_to_receive);
				CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
				return;
			}
			else
			{
				SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
				SystemVar.usb_rx_index  = 0;
				bzero(usb_uploader_tx_buf,64);
				sprintf((char *)usb_uploader_tx_buf,"UNKNOWN_IMAGE %d %d",(int )SystemVar.usb_image_number,(int )usb_bytes_to_receive);
				CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
				return;
			}
		}
	}
}

static void flash_erase_base(uint32_t base,uint32_t num)
{
uint32_t j;
	for(j=0;j<num;j++)
		flash_EraseSector(base+j);
}

static void store_image(void)
{
uint32_t	base=0,error=0;

	base = analyze_usb_buf();
	if ( base == 0 )
	{
		error = 1;
	}
	else
	{
		switch ( base )
		{
		case	DIGIT0_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[0] ,flash_SectorToAddress(DIGIT0_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT1_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[1] ,flash_SectorToAddress(DIGIT1_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT2_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[2] ,flash_SectorToAddress(DIGIT2_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT3_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[3] ,flash_SectorToAddress(DIGIT3_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT4_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[4] ,flash_SectorToAddress(DIGIT4_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT5_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[5] ,flash_SectorToAddress(DIGIT5_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT6_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[6] ,flash_SectorToAddress(DIGIT6_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT7_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[7] ,flash_SectorToAddress(DIGIT7_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT8_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[8] ,flash_SectorToAddress(DIGIT8_BASE),DIGIT_SIZE*2);
			break;
		case	DIGIT9_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Digit[9] ,flash_SectorToAddress(DIGIT9_BASE),DIGIT_SIZE*2);
			break;
		case	DIGITOFF_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)DigitOFF ,flash_SectorToAddress(DIGITOFF_BASE),DIGIT_SIZE*2);
			break;
		case	LOGO_BASE	:
			flash_erase_base(base,8);
			flash_WriteBytes((uint8_t *)SyntMateLogo ,flash_SectorToAddress(LOGO_BASE),LOGO_SIZE*2);
			break;
		case	INCREASE_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Increase 			,flash_SectorToAddress(INCREASE_BASE),ICONS_SIZE*2);
			break;
		case	INCREASE_DISABLED_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Increase_disabled	,flash_SectorToAddress(INCREASE_DISABLED_BASE),ICONS_SIZE*2);
			break;
		case	DECREASE_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Decrease 			,flash_SectorToAddress(DECREASE_BASE),ICONS_SIZE*2);
			break;
		case	DECREASE_DISABLED_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Decrease_disabled	,flash_SectorToAddress(DECREASE_DISABLED_BASE),ICONS_SIZE*2);
			break;
		case	SETTINGS_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Settings 			,flash_SectorToAddress(SETTINGS_BASE),ICONS_SIZE*2);
			break;
		case	SETTINGS_DISABLED_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Settings_disabled	,flash_SectorToAddress(SETTINGS_DISABLED_BASE),ICONS_SIZE*2);
			break;
		case	HOME_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Home	 			,flash_SectorToAddress(HOME_BASE),ICONS_SIZE*2);
			break;
		case	HOME_DISABLED_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Home_disabled		,flash_SectorToAddress(HOME_DISABLED_BASE),ICONS_SIZE*2);
			break;
		case	PLUS_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Plus	 			,flash_SectorToAddress(PLUS_BASE),ICONS_SIZE*2);
			break;
		case	MINUS_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)Minus 				,flash_SectorToAddress(MINUS_BASE),ICONS_SIZE*2);
			break;
		case	BACK2NORMAL_BASE	:
			flash_erase_base(base,16);
			flash_WriteBytes((uint8_t *)Back 				,flash_SectorToAddress(BACK2NORMAL_BASE),BACK_SIZE*2);
			break;
		case	PLAY_BASE	:
			flash_erase_base(base,16);
			flash_WriteBytes((uint8_t *)play ,flash_SectorToAddress(PLAY_BASE),BUTTONS_SIZE*2);
			break;
		case	STOP_BASE	:
			flash_erase_base(base,16);
			flash_WriteBytes((uint8_t *)stop ,flash_SectorToAddress(STOP_BASE),BUTTONS_SIZE*2);
			break;
		case	HOMING_GREEN_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)HomingGreen ,flash_SectorToAddress(HOMING_GREEN_BASE),ICONS_SIZE*2);
			break;
		case	HOMING_RED_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)HomingRed ,flash_SectorToAddress(HOMING_RED_BASE),ICONS_SIZE*2);
			break;
		case	HOMING_BLUE_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)HomingBlue ,flash_SectorToAddress(HOMING_BLUE_BASE),ICONS_SIZE*2);
			break;
		case	HOMING_BLACK_BASE	:
			flash_erase_base(base,2);
			flash_WriteBytes((uint8_t *)HomingBlack ,flash_SectorToAddress(HOMING_BLACK_BASE),ICONS_SIZE*2);
			break;
		default	:
			error=1;
			break;
		}
	}

	bzero(usb_uploader_tx_buf,64);
	if ( error == 0 )
		sprintf((char *)usb_uploader_tx_buf,"STORED %s\n",SystemVar.usb_image_name);
	else
		sprintf((char *)usb_uploader_tx_buf,"ERROR %s\n",SystemVar.usb_image_name);
	CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));

	default_case();
}

static void erase_flash(void)
{
uint32_t	secs = 0 , status = 0;
char		msg[32];

	ILI9341_FillScreen(ILI9341_BLACK);
	sprintf(msg,"Erasing ...");
	ILI9341_WriteString(20, 100, msg, Font_16x26, ILI9341_RED, ILI9341_BLACK);
	HAL_Delay(1000);
	flash_ChipErase();
	while( status == 0 )
	{
		sprintf(msg,"Erasing, %d elapsed",(int )secs);
		CDC_Transmit_FS((uint8_t *)msg,strlen(msg));
		HAL_Delay(1000);
		secs += 1;
		status = flash_PollChipErase();
	}
	HAL_Delay(100);
	sprintf(msg,"Flash Erased, %d elapsed\n",(int )secs);
	CDC_Transmit_FS((uint8_t *)msg,strlen(msg));
	sprintf(msg,"Erase done  ");
	ILI9341_WriteString(20, 100, msg, Font_16x26, ILI9341_BLUE, ILI9341_BLACK);
	HAL_Delay(1000);
}

uint32_t ParseUSBPacket(void)
{
uint32_t	pnum;
char		item[32], size_label[32], usb_image_name[32];

	bzero(SystemVar.usb_image_name,sizeof(SystemVar.usb_image_name));
	bzero(usb_image_name,32);

	pnum = sscanf((char *)SystemVar.usb_packet,"%s %s %s %d %d",
		item,usb_image_name,size_label,(int *)&SystemVar.usb_byte_count,(int *)&SystemVar.usb_image_number);

	sprintf(SystemVar.usb_image_name,"%s",usb_image_name);
	if ( pnum == 2 )
	{
		if ( strcmp(item,"STORE") == 0 )
			store_image();
		if (( strcmp(item,"ERASE") == 0 ) && ( strcmp(usb_image_name,"ALL") == 0 ))
			erase_flash();
		return 0;
	}

	if ( pnum == 4 )
	{
		bzero(usb_uploader_tx_buf,64);
		if ( strcmp(item,"IMAGE") == 0 )
		{
			sprintf((char *)usb_uploader_tx_buf,"%s %s %d OK",item,SystemVar.usb_image_name,(int )SystemVar.usb_byte_count);
		}

		CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
		SystemVar.usb_rx_index  = 0;
		SystemVar.usb_pkt_found = USB_RECEIVING_PACKET;
		usb_rxed_byte_count = usb_byte_count = usb_rx_index = 0;
		usb_bytes_to_receive = SystemVar.usb_byte_count/2;
		return 0;
	}

	bzero(usb_uploader_tx_buf,64);
	sprintf((char *)usb_uploader_tx_buf,"NOK pnum %d\n",(int )pnum );
	CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
	SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
	return 1;
}

uint32_t AnalyzeUSBHeader(void)
{
uint16_t	i;
	for(i=0;i<SystemVar.usb_rxed_byte_count;i++)
	{
		if ( SystemVar.usb_rxed_packet[i] == '<' )
		{
			bzero(SystemVar.usb_packet,USB_BUFLEN);
			SystemVar.usb_byte_count = SystemVar.usb_rx_index = 0;
			SystemVar.usb_do_cp=1;
		}
		else
		{
			if ( SystemVar.usb_do_cp == 1 )
			{
				if ( SystemVar.usb_rxed_packet[i] == '>' )
				{
					SystemVar.usb_do_cp=0;
					bzero(SystemVar.usb_rxed_packet,USB_BUFLEN);
					SystemVar.usb_rx_index = 0;
					return 0;
				}
				else
				{
					SystemVar.usb_packet[SystemVar.usb_rx_index] = SystemVar.usb_rxed_packet[i];
					SystemVar.usb_byte_count ++;
					SystemVar.usb_rx_index++;
					if ( SystemVar.usb_rx_index >= USB_BUFLEN)
						SystemVar.usb_rx_index = 0;
				}
			}
		}
	}
	return 1;
}



uint32_t USB_ImageUploader(void)
{
	switch ( SystemVar.usb_pkt_found )
	{
	case	USB_WAIT_FOR_HEADER 	:   if ( AnalyzeUSBHeader() == 0 )
										{
											ParseUSBPacket();
										}
										break;
	case	USB_RECEIVING_PACKET 	: ReceiveUSBPacket(); break;
	default 						: default_case();break;
	}
	return SystemVar.usb_pkt_found;
}



