#include "tim_services.h"
#include "main.h"
#include "tim.h"
#include "leds.h"
#include "relays.h"


void tim_init(void)
{
	__HAL_TIM_ENABLE( &htim2);
	HAL_TIM_Base_Start_IT(&htim2);
	
}

void tim_engine(void)
{



}

void tim_setperiod( uint32_t period)
{
	htim2.Instance->ARR = period * 10;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM2)
		{
			
			tim_engine();

		}
}
