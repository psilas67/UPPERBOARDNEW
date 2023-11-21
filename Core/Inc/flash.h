#ifndef __pflash__

#define __pflash__

/*
Library:				STM32F10x Internal FLASH read/write
Written by:			Mohamed Yaqoob (MYaqoobEmbedded YouTube Channel)
Last modified:	15/03/2019
Description:
							MY_FLASH library implements the following basic functionalities
								- Set sectos address
								- Flash Sector Erase
								- Flash Write
								- Flash Read
								
* Copyright (C) 2019 - M. Yaqoob
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
	
   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.								
*/

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "stdint.h"

//Typedefs
//1. data size
typedef enum
{
	DATA_TYPE_8=0,
	DATA_TYPE_16,
	DATA_TYPE_32,
}DataTypeDef;
#define LAST_PAGE_END 0x0801FC00
#define SAVE_ADDRESS 0x0801FC00
#define SAVE_SECTOR  127

//functions prototypes
//1. Write Flash
uint32_t flash_write(uint32_t addr, void const* data, uint32_t sz);
//2. Read Flash
void flash_read(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType);
uint32_t flash_erase_sec(uint32_t init_address);

#endif