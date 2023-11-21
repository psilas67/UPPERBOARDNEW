#ifndef __sysconf__
#define __sysconf__
#include "stdint.h"

typedef struct
{
	uint32_t preamble;
	int32_t fmult_weight;
	int32_t fdiv_weight;
	int32_t max_offset_weight;
	int32_t min_offset_weight;
	int32_t fmult_torque;
	int32_t fdiv_torque;
	int32_t max_offset_torque;
	int32_t min_offset_torque;
	int32_t fmult_susp;
	int32_t fdiv_susp;
	int32_t max_offset_susp;
	int32_t min_offset_susp;
	uint32_t logic_neg;
	uint32_t speed_stop;
	uint32_t timepack_max;
	char customer_name[32];
	char serial_number[16];
} config_data_t;


extern config_data_t * cfg;
extern config_data_t   cfgx;
	
uint32_t sysconf_checkdata( config_data_t * a);

void sysconf_read( config_data_t * b);

void sysconf_write( config_data_t * c);

void sysconf_write_defaults( config_data_t * d);


#endif
