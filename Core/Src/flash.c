#include "flash.h"

//************************************************************************************************
// endereco_inicial_pagina = 0x0801FC00 no STM32F103C8 (128k) Pagina=1k
//************************************************************************************************
uint32_t flash_erase_sec(uint32_t init_address)
{
   FLASH_EraseInitTypeDef er = {
      .TypeErase = FLASH_TYPEERASE_PAGES,
      .PageAddress = init_address,
      .NbPages = 1,
   };
   uint32_t fault_sec = 0;
   HAL_FLASH_Unlock();
   HAL_StatusTypeDef res = HAL_FLASHEx_Erase(&er, &fault_sec);
   HAL_FLASH_Lock();
   return res == HAL_OK ? 0 : -1;
}
//************************************************************************************************
// 
//************************************************************************************************
uint32_t flash_write(uint32_t addr, void const* data, uint32_t sz)
{
   HAL_StatusTypeDef res = HAL_OK;
   HAL_FLASH_Unlock();
   for (; sz >= 4; sz -= 4, addr += 4) {
      res = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *(uint32_t const*)data);
      data = (uint32_t const*)data + 1;
      if (res != HAL_OK)
      {
         goto done;
      }
   }
done:
   HAL_FLASH_Lock();
   return res == HAL_OK ? 0 : -1;
}
