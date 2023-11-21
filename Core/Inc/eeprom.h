#ifndef __EEprom__
#define __EEprom__
#include "stdint.h"

#define WREN_CMD           0x06        /* Set Write Enable Latch */ 
#define WRDI_CMD           0x04        /* Reset Write Enable Latch */ 
#define RDSR_CMD           0x05        /* Read Status Register */ 
#define WRSR_CMD           0x01        /* Write Status Register */ 
#define READ_CMD           0x03        /* Read Data from Memory Array */ 
#define WRITE_CMD          0x02        /* Write Data to Memory Array */ 
 
#define REG_STATUS             	0x00        /* Status Register */ 
#define STATUS_MASK_READY  			0x01        /* Status Register ready/busy mask */ 
#define BUSY_Bit               	0x1         /* RDY bit of SR = 0x1 */ 
#define READY_Bit              	0x0         /* RDY bit of SR = 0x0 */ 
 
#define STATUS_MASK_WREN   			0x02        /* Status Register WREN mask */ 
#define WREN_Bit               	0x2         /* WEN of SR = 0x1 */ 


#define PAGESIZE  64

  void EE_Init(void);
  void EE_write_enable(void);
	void EE_write_disable(void);
  char EE_read_status_register(void);
  void EE_write_status_register(char w);
  void write_byte(uint16_t  address, uint8_t  data);
  void write_page( uint16_t  address, uint8_t  *data );
	void write_float( uint16_t  address, float *data);
  uint8_t read_byte(uint16_t  address);
  int read_page(uint16_t  address, uint8_t * data);
	float read_float(uint16_t  address);

#endif
