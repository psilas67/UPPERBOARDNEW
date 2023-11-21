#ifndef __relays__
#define __relays__
#include "gpio.h"

typedef struct{
	  GPIO_TypeDef * gpio;
		uint16_t pin;
} trelay;


void relays_init(void);
void relays_test(void);
uint32_t RL_ON(uint32_t nrelay);
uint32_t RL_OFF(uint32_t nrelay);
void RL_ALL_OFF(void);
void	relay( uint32_t arg1, uint32_t arg2);
uint32_t motor1_on(void);
uint32_t motor2_on(uint32_t mode);
uint32_t motorx1_on(void);
uint32_t motorx2_on(uint32_t mode);
void motor3_on(void);
void motor4_on(void);
void motor1_off(void);
void motor2_off(void);
void motor3_off(void);
void motor4_off(void);
void relays_engine(void);



#endif

