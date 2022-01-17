/*
 * SpiFlash.c
 *
 *  Created on: Dec 17, 2020
 *      Author: fil
 */

#include "main.h"


#define 	W25Q_DUMMY_BYTE 		0xA5
#define		READ_COMMAND			0x03
#define		FAST_READ_COMMAND		0x0B
#define		WRITE_COMMAND			0x02
#define		WRITE_DISABLE_COMMAND	0x04
#define		WRITE_ENABLE_COMMAND	0x06
#define		READ_ID_COMMAND			0x9f
#define		GETSTATUS_COMMAND		0x05
#define		SECTOR_ERASE_COMMAND	0x20
#define		CHIP_ERASE_COMMAND		0xc7


static uint8_t	flash_SpiTX(uint8_t	Data)
{
uint8_t	ret;
	HAL_SPI_TransmitReceive(&FlashSPIport,&Data,&ret,1,100);
	return ret;
}

static void flash_WaitForWriteEnd(void)
{
uint8_t		StatusRegister1;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	flash_SpiTX(GETSTATUS_COMMAND);
	do
	{
		StatusRegister1 = flash_SpiTX(W25Q_DUMMY_BYTE);
		HAL_Delay(1);
	}
	while ((StatusRegister1 & 0x01) == 0x01);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

static void flash_WriteEnable(void)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	flash_SpiTX(WRITE_ENABLE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

void flash_WriteDisable(void)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	flash_SpiTX(WRITE_DISABLE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

uint32_t	flash_SectorToAddress(uint32_t	Sector)
{
	return Sector * SECTOR_SIZE;
}

uint32_t	flash_AddressToSector(uint32_t	Address)
{
	return Address / SECTOR_SIZE;
}

static uint32_t send_cmd_addr(uint8_t cmd,uint32_t Address)
{
uint32_t	ret = 0;
	flash_SpiTX(cmd);
	if ( SystemParameters.flash_capacity == F256 )
		ret = flash_SpiTX((Address & 0xFF000000) >> 24) << 24;
	ret |= flash_SpiTX((Address & 0xFF0000) >> 16) << 16;
	ret |= flash_SpiTX((Address & 0xFF00) >> 8) << 8;
	ret |= flash_SpiTX(Address & 0xFF);
	return ret;
}

void flash_EraseSector(uint32_t Sector)
{
uint32_t	SectorAddr;
	SectorAddr = Sector * SECTOR_SIZE;
	flash_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(SECTOR_ERASE_COMMAND,SectorAddr);
	HAL_Delay(1);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(50);
	flash_WaitForWriteEnd();
}

void flash_ReadBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(READ_COMMAND,Address);
	//flash_SpiTX(0);
	HAL_SPI_Receive(&FlashSPIport,pBuffer,size,2000);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

static void flash_WritePage(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	flash_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(WRITE_COMMAND,Address);
	HAL_SPI_Transmit(&FlashSPIport,pBuffer,size,100);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	flash_WaitForWriteEnd();
}

void flash_WriteBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	while(size > PAGE_SIZE)
	{
		flash_WritePage(pBuffer, Address, PAGE_SIZE);
		size -= PAGE_SIZE;
		Address += PAGE_SIZE;
		pBuffer += PAGE_SIZE;
	}
	if ( size > 0 )
		flash_WritePage(pBuffer, Address, size);
}

uint32_t flash_ReadID(void)
{
uint32_t fake_addr, ret_val;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	flash_SpiTX(READ_ID_COMMAND);
	fake_addr = (W25Q_DUMMY_BYTE << 24 )|(W25Q_DUMMY_BYTE << 16 )|(W25Q_DUMMY_BYTE << 8 )|(W25Q_DUMMY_BYTE << 0 );
	ret_val = send_cmd_addr(READ_ID_COMMAND,fake_addr) >> 8;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	return ret_val;
}

uint8_t flash_PollChipErase(void)
{
uint8_t		StatusRegister1; // 1 if operation in progress, 0 if finished
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	flash_SpiTX(GETSTATUS_COMMAND);
	StatusRegister1 = flash_SpiTX(W25Q_DUMMY_BYTE);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	return StatusRegister1;
//	while ((StatusRegister1 & 0x01) == 0x01);
}

void flash_ChipErase(void)
{
	flash_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	flash_SpiTX(CHIP_ERASE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(1);
}

