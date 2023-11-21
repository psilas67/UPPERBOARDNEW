#include "flash.h"
#include "sys_conf.h"
#include "string.h"

config_data_t * cfg;
config_data_t   cfgx;

union {
		uint8_t bytes[sizeof(config_data_t)];
		config_data_t cfgd;
	} datac;

uint32_t sysconf_checkdata( config_data_t * a)
{
		sysconf_read ( a );
		if( a->preamble != 0x00ffaa55)
			return 0;

		return 1;
}

void sysconf_read( config_data_t * b)
{
   uint32_t check;
   uint32_t x;
   uint8_t y;
   uint8_t *p_font;
   uint8_t *p_dest;
   
   p_dest = (uint8_t*) b;
   p_font = (uint8_t*) LAST_PAGE_END;
   
   check = 0;
   for (x = 0; x < sizeof(cfgx); x++)
   {
      y = *p_font;
      *p_dest = y;
      check = check + y;        
      p_font++;
      p_dest++;        
   }    

}

void sysconf_write( config_data_t * c)
{
   uint32_t check;
   uint32_t x;
   uint8_t y;
   uint8_t *p;
   
   p = (uint8_t*) &c;
   check = 0;
   for (x = 0; x < sizeof(cfgx); x++)
   {
      y = *p;
      check = check + y;
      p++;        
   }    
   //uso os ultimos 1024 bytes da flash
   flash_erase_sec(LAST_PAGE_END);    
   //a estrutura deve ser multiplo de 4 bytes
   flash_write(LAST_PAGE_END, &cfgx, sizeof(cfgx)); 

}

void sysconf_write_defaults( config_data_t * d)
{
	  cfg = d;
		cfg->fmult_susp   = 1L;
		cfg->fdiv_susp    = 1L;
		cfg->fmult_weight = 1L;
		cfg->fdiv_weight  = 1L;
		cfg->fmult_torque = 1L;
		cfg->fdiv_torque  = 1L;
		cfg->logic_neg    = 0;
		cfg->speed_stop = 1;
		memcpy( cfg->serial_number, (void *) "0000000000000000\0", 16);
		memcpy( cfg->customer_name, (void *) "CUSTOMER MECTRONIX\0", 19);
		cfg->max_offset_susp    = 0xffffffff;
		cfg->max_offset_torque  = 0xffffffff;
		cfg->max_offset_weight  = 0xffffffff;
		cfg->min_offset_susp    = 0x80000000;
		cfg->min_offset_torque  = 0x80000000;
		cfg->min_offset_weight  = 0x80000000;
		cfg->preamble = 0x00ffaa55;
		sysconf_write ( d );
}

