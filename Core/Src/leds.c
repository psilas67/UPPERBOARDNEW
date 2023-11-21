#include "leds.h"
//#include "cmsis_os.h"

const tled leds[8] = {
	{LED1_GPIO_Port , LED1_Pin}, 	// LED1
	{LED2_GPIO_Port , LED2_Pin}, 	// LED2
	{LED3_GPIO_Port , LED3_Pin}, 	// LED3
	{LED4_GPIO_Port , LED4_Pin}, 	// LED4
};

void leds_init(void){
	LED_ALL_OFF();
}
void leds_test(void){
uint32_t n;
	for(n=1;n<=4;n++){
			LED_ON(n);
			HAL_Delay(10);
	}
	for(n=1;n<=4;n++){
			LED_OFF(n);
			HAL_Delay(10);
	}

}

uint32_t LED_ON(uint32_t nled){
	nled--;
	if(nled < 4){
		HAL_GPIO_WritePin(leds[nled].gpio, leds[nled].pin, GPIO_PIN_RESET );
		return (uint32_t) leds[nled].pin;
	}else 
	return 0;
 
}
uint32_t LED_OFF(uint32_t nled){
	nled--;
	if(nled < 4){
	HAL_GPIO_WritePin(leds[nled].gpio, leds[nled].pin, GPIO_PIN_SET );
		return (uint32_t) leds[nled].pin;
	}else 
	return 0;
}

void LED_ALL_OFF(void){
uint32_t n;
	for(n=1;n<=4;n++) LED_OFF(n);

}
