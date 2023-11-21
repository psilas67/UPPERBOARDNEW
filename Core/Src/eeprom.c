#include "eeprom.h"
#include "spi.h"
#include "string.h"

#define EE_CS_PORT GPIOA
#define EE_CS_PIN  GPIO_PIN_4

void EE_Init(void) 
{
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
}

uint8_t SPI_transmit( uint8_t data )
{
uint8_t rxdata;
	 HAL_SPI_TransmitReceive( &hspi1, &data, &rxdata, 1, 0xFFFF);
	 return rxdata; 
}

void EE_write_enable(void)
{
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
  SPI_transmit(WREN_CMD);    //write enable instruction
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};

}
void EE_write_disable(void)
{
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
  SPI_transmit(WRDI_CMD);    //write disable instruction
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};

}
char EE_read_status_register(void)
{
  uint8_t  data = 0;
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
 	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
	data = SPI_transmit(RDSR_CMD);    //read status registry
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};
  return data;
}

void write_status_register(uint8_t data)
{
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
  SPI_transmit(WRSR_CMD);    //write instruction
  SPI_transmit(data);
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};
}


void write_byte( uint16_t  address, uint8_t  data )
{
	EE_write_enable();
	HAL_Delay(10);
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
  SPI_transmit(WRITE_CMD); //write instruction

  //Send next page of data
  SPI_transmit((address & 0x0FF00) >> 8);   //send MSByte address first
  SPI_transmit(address & 0x0FF);      //send LSByte address
  SPI_transmit(data);
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};
	EE_write_disable();
}

void write_page( uint16_t  address, uint8_t  *data )
{
  uint16_t  i;
  uint8_t  *ptr = data;
	EE_write_enable();
	HAL_Delay(10);
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
  SPI_transmit(WRITE_CMD); //write instruction
  //Send next page of data
  SPI_transmit((address & 0x0FF00) >> 8);   //send MSByte address first
  SPI_transmit(address & 0x0FF);      //send LSByte address
  for (i=0;(i<PAGESIZE);i++)
    SPI_transmit(*ptr++);
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};

}
void write_float( uint16_t  address, float *data){
	uint16_t i;
  uint8_t  *ptr = (uint8_t *) (void *) data;
	EE_write_enable();
	HAL_Delay(10);
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
  SPI_transmit(WRITE_CMD); //write instruction
  //Send next page of data
  SPI_transmit((address & 0x0FF00) >> 8);   //send MSByte address first
  SPI_transmit(address & 0x0FF);      //send LSByte address
  for (i=0;(i<sizeof(data));i++){
   SPI_transmit(*ptr++);
	}
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};

}

uint8_t  read_byte( uint16_t  address)
{
	uint8_t  data;
 	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};
	SPI_transmit(READ_CMD);
  SPI_transmit((address & 0x0FF00) >> 8);  
  SPI_transmit(address & 0x0FF);  
  data = SPI_transmit(0xFF);  
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};
  return data;
}

int read_page(uint16_t  address, uint8_t * data)
{
  int i;

	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};

  SPI_transmit(READ_CMD); //read instruction

  //Send next page of data
  SPI_transmit((address & 0x0FF00) >> 8);   //send MSByte address first
  SPI_transmit(address & 0x0FF);      //send LSByte address
  
  for (i=0;(i<PAGESIZE);i++)
    *data++ = SPI_transmit(0xFF);

	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};
  
  return i;
}
float read_float(uint16_t  address)
{
  int i;
	float data = 0.0;
	uint8_t *ptr = (uint8_t *) (void *) &data;
	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_RESET){};

  SPI_transmit(READ_CMD); //read instruction

  //Send next page of data
  SPI_transmit((address & 0x0FF00) >> 8);   //send MSByte address first
  SPI_transmit(address & 0x0FF);      //send LSByte address
  
  for (i=0;(i<sizeof(float));i++)
		*ptr++ = SPI_transmit(0xFF);

	HAL_GPIO_WritePin( EE_CS_PORT , EE_CS_PIN, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(EE_CS_PORT , EE_CS_PIN)!=GPIO_PIN_SET){};
  
  return data;
}

