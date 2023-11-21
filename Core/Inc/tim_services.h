#ifndef __timservice__
#define __timservice__
#include "stdint.h"

void tim_init(void);
void tim_engine(void);
void tim_setperiod( uint32_t period);


#endif
