#include "relays.h"
#include "protocol.h"
#include "data_exchange.h"
#include "sys_conf.h"
#include "data_exchange.h"

#define POS1_STATE (!(HAL_GPIO_ReadPin( POS1_GPIO_Port, POS1_Pin) == GPIO_PIN_SET))
#define POS2_STATE (!(HAL_GPIO_ReadPin( POS2_GPIO_Port, POS2_Pin) == GPIO_PIN_SET))

uint32_t delay_motor_time;
uint32_t delay_motor_value;

const trelay relays[8] = {
	{RL1_GPIO_Port , RL1_Pin}, 		// RL1
	{RL2_GPIO_Port , RL2_Pin}, 		// RL2
	{RL3_GPIO_Port , RL3_Pin}, 		// RL3
	{RL4_GPIO_Port , RL4_Pin}, 		// RL4
	{RL5_GPIO_Port , RL5_Pin}, 		// RL5
	{RL6_GPIO_Port , RL6_Pin}, 		// RL6
	{RL7_GPIO_Port , RL7_Pin},		// RL7
	{RL8_GPIO_Port , RL8_Pin} 		// RL8
};

void relays_init(void){
	RL_ALL_OFF();
	delay_motor_time = HAL_GetTick();
	delay_motor_value = 500;
}
void relays_test(void){
uint32_t n;
	for(n=1;n<=8;n++){
			RL_ON(n);
			HAL_Delay(100);
	}
	for(n=1;n<=8;n++){
			RL_OFF(n);
			HAL_Delay(100);
	}

}

uint32_t RL_ON(uint32_t nrelay){
	nrelay--;
	if(nrelay < 8){
		HAL_GPIO_WritePin(relays[nrelay].gpio, relays[nrelay].pin, GPIO_PIN_SET );
		return (uint32_t) relays[nrelay].pin;
	}else 
	return 0;
 
}
uint32_t RL_OFF(uint32_t nrelay){
	nrelay--;
	if(nrelay < 8){
	HAL_GPIO_WritePin(relays[nrelay].gpio, relays[nrelay].pin, GPIO_PIN_RESET );
		return (uint32_t) relays[nrelay].pin;
	}else 
	return 0;
}

void RL_ALL_OFF(void){
uint32_t n;
	for(n=1;n<=8;n++) RL_OFF(n);

}

void	relay( uint32_t arg1, uint32_t arg2)
{
	if(arg2 == 1)
		RL_ON( arg1 );
	else
		RL_OFF( arg1 );
}
 
uint32_t motor1_on(void)
{
	uint32_t err = 0;
		if( POS1_STATE || kdiag == 1)
		{
			RL_ON(8);
			HAL_Delay(1000);
			RL_OFF(8);
			HAL_Delay(1000);
			RL_ON(1);
			HAL_Delay(1000);
			RL_ON(2);
			HAL_Delay(1000);
		}else
			err = 1;
	delay_motor_time = HAL_GetTick();
	delay_motor_value = delay_motor_time + 500;
	return err;
}
uint32_t motor2_on(uint32_t mode)
{
	uint32_t err = 0;
		if( POS2_STATE || kdiag == 1)
		{
			if(mode == 1)
			{
				RL_ON(5);
				HAL_Delay( 500 ); 
			}
			RL_ON(3);
			HAL_Delay(1000);
			RL_ON(4);
			HAL_Delay(1000);
		}else
			err = 1;
	delay_motor_time = HAL_GetTick();
	delay_motor_value = delay_motor_time + 500;
	return err;
}

uint32_t motorx1_on(void)
{
	uint32_t err = 0;
		if( POS1_STATE || kdiag == 1 )
		{
			RL_ON(8);
			HAL_Delay(1000);
			RL_OFF(8);
			HAL_Delay(1000);
			RL_ON(1);
			HAL_Delay(1000);
			RL_ON(2);
			HAL_Delay(1000);
		}else
			err = 1;
	delay_motor_time = HAL_GetTick();
	delay_motor_value = delay_motor_time + 2500;
	return err;
}
uint32_t motorx2_on(uint32_t mode)
{
	uint32_t err = 0;
		if( POS2_STATE || kdiag == 1)
		{
			if(mode == 1)
			{
				RL_ON(5);
				HAL_Delay( 500 ); 
			}
			RL_ON(3);
			HAL_Delay(1000);
			RL_ON(4);
			HAL_Delay(1000);
		}else
			err = 1;
	delay_motor_time = HAL_GetTick();
	delay_motor_value = delay_motor_time + 2500;
	return err;
}

void motor3_on(void)
{
		RL_ON(6);
}

void motor4_on(void)
{
		RL_ON(7);
}

void motor1_off(void)
{
		RL_OFF(1);
		RL_OFF(2);
}

void motor2_off(void)
{
		RL_OFF(3);
		RL_OFF(4);
		RL_OFF(5);
}

void motor3_off(void)
{
		RL_OFF(6);
}

void motor4_off(void)
{
		RL_OFF(7);
}

void relays_engine(void)
{
	struct TDATAEX1 * dt;
	dt = (struct TDATAEX1 *) &datarex;
	if((HAL_GetTick() > delay_motor_value) && (kdiag == 0))
	{
	// desligar por sensor de presença - indicar a causa
		if (cfgx.logic_neg == 0)
		{
				if( !(dt->Status & ST_POS1) || !(dt->Status & ST_POS2) )
				{
					motor1_off();
					motor2_off();
					delay_motor_time = 0;
				}
		}
		else
		{
				if( !(~dt->Status & ST_POS1) || !(~dt->Status & ST_POS2))
				{
					motor1_off();
					motor2_off();
					delay_motor_time = 0;
				}
		}
	// desligar por sensor de rotação - indicar a causa
		if( (uint32_t) dt->Spd1 < cfgx.speed_stop || (uint32_t) dt->Spd2 < cfgx.speed_stop )
			{
			motor1_off();
			motor2_off();
			delay_motor_time = 0;
		}		
// desligar por falta de comunicação
		if( comm_timer == 0 )
		{
			motor1_off();
			motor2_off();
			delay_motor_time = 0;
		}		

	}
}
