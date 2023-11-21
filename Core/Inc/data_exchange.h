#ifndef __dataexch__
#define __dataexch__
#include "stdint.h"

#define stmask_pos_1 0x0001
#define stmask_pos_2 0x0010
#define stmask_spd_1 0x0002
#define stmask_spd_2 0x0020
#define stmask_M1_1  0x0004
#define stmask_M2_2  0x0040
#define stmask_M3_1  0x0008
#define stmask_M4_2  0x0080
#define stmask_HXP_1 0x0100
#define stmask_HXP_2 0x1000
#define stmask_HXP_3 0x0200
#define stmask_HXP_4 0x2000
#define stmask_HXP_5 0x0400
#define stmask_HXP_6 0x4000

extern uint32_t txPackFlag;
extern uint32_t txPackPeriod;
//typedef struct 
//{
//	  // 0 - preamble;
//		// 1 - Ali
//		// 2 - wheight L
//		// 3 - wheight R
//		// 4 - torque L
//		// 5 - torque R
//		// 6 - susp L
//		// 7 - susp R
//		// 8 - status
//		// 9 -  spd1
//		// 10 - spd2
//		// 11 - pknum
//		// 12 - CRC
//		int16_t chn_read[13];
//} TDATAEX;

struct TDATAEX1
{
  uint16_t Preamble;
   int16_t Alinhamento;
   int16_t Peso_Esquerdo;
   int16_t Peso_Direito;
   int16_t Torque_Esquerdo;
   int16_t Torque_Direito;
   int16_t Suspencao_Esquerda;
   int16_t Suspencao_Direita;
   int16_t Status;
   int16_t Spd1;
   int16_t Spd2;
   uint16_t Pkt_Numero;
   uint16_t crc;
} ;

#define ST_POS1 0x0001
#define ST_POS2 0x0010
#define ST_VEL1 0x0002
#define ST_VEL2 0x0020
#define ST_MFR1 0x0004
#define ST_MFR2 0x0040
#define ST_MBS1 0x0008
#define ST_MBS2 0x0080


extern struct TDATAEX1 datarex;
extern int32_t zero[6];
extern uint32_t comm_timer;
extern uint32_t klogicneg;
extern volatile uint32_t TimePackPeriod;
#define comm_timer_period 1000

void data_engine_init(void);
void data_engine(void);
void txdata(uint8_t * txd, uint16_t txlen);
uint32_t rxdata(uint8_t * rxpointer);
uint32_t zeros(void);
void clear_zeros(void);


#endif