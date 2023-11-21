#ifndef __protocol__
#define __protocol__
#include "stdint.h"

void protocol_Engine(void);
void parse_engine( uint8_t * rxmsg, uint32_t len);
void Reset_System(void);
extern uint32_t memory_save(void);
extern uint32_t kdiag;
extern uint32_t kpacket;
extern uint32_t ksposl;
extern uint32_t ksposr;
extern uint32_t ksspdl;
extern uint32_t ksspdr;
extern uint32_t klogicneg;
extern uint32_t kdiag;
extern volatile uint16_t flagtx;
#endif

