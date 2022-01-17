/*
 * SpiFlash.h
 *
 *  Created on: Feb 4, 2021
 *      Author: fil
 */

#ifndef INC_SINTMATE_SPIFLASH_H_
#define INC_SINTMATE_SPIFLASH_H_

extern	void 		flash_WriteDisable(void);
extern	uint32_t	flash_SectorToAddress(uint32_t	Sector);
extern	uint32_t	flash_AddressToSector(uint32_t	Address);
extern	void 		flash_EraseSector(uint32_t Sector);
extern	void 		flash_ReadBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size);
extern	void 		flash_WriteBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size);
extern	uint32_t 	flash_ReadID(void);
extern	uint32_t 	SetupFlash(void);
extern	void 		flash_ChipErase(void);
extern	uint8_t 	flash_PollChipErase(void);


#define		F128			0x18
#define		F256			0x19
#define		PAGE_SIZE		256
#define		SECTOR_SIZE		4096

#endif /* INC_SINTMATE_SPIFLASH_H_ */
