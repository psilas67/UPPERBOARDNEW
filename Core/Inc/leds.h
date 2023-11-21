#ifndef __leds__
#define __leds__
#include "gpio.h"

typedef struct{
	  GPIO_TypeDef * gpio;
		uint16_t pin;
} tled;


void leds_init(void);
void leds_test(void);
uint32_t LED_ON(uint32_t nrelay);
uint32_t LED_OFF(uint32_t nrelay);
void LED_ALL_OFF(void);

#endif

