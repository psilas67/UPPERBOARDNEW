#include "data_exchange.h"
#include "usart.h"
#include "dma.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "leds.h"
#include "relays.h"
#include "network.h"
#include "sys_conf.h"
#include "protocol.h"
#define zero_max_limit 10000


struct TDATAEX1 datarex;

uint32_t tx_flag = 0;
uint8_t rxdatabuf[(sizeof(datarex)*2)]={0};

int32_t zero[6];

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern CRC_HandleTypeDef hcrc;

char * cpointer;
char * beginarg;
char * endarg;
char MainBuf[256];
char RxBuf[26];
char MainBuf2[1024];
char RxBuf2[256];
char TxBuf2[256];
uint32_t oldPos;
uint32_t newPos;
uint32_t rx2len;
uint32_t flaq_recebido;
uint32_t flaq_recebido2;

uint32_t txPackFlag;
uint32_t txPackPeriod;
uint32_t PackTimer = 0;
uint32_t kTimePack = 0;
extern uint32_t kpacket;
volatile uint32_t TimePack = 0;
volatile uint32_t TimePackPeriod = 1000;
uint32_t cntPack = 0;
uint32_t comm_timer;

struct TDATAEX1 * dtpack;
    // 1 - Ali
		// 2 - wheight L
		// 3 - wheight R
		// 4 - torque L
		// 5 - torque R
		// 6 - susp L
		// 7 - susp R
		// 8 - status
		// 9 -  spd1
		// 10 - spd2
		// 11 - pknum

void data_engine_init(void)
{
	clear_zeros();
  oldPos = 0;
	newPos = 0;
  flaq_recebido = 0;
	flaq_recebido2 = 0;
	txPackFlag = 0;
	txPackPeriod = 1000;
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) RxBuf, sizeof(RxBuf));
	HAL_UARTEx_ReceiveToIdle_DMA (&huart2, (uint8_t *) RxBuf2 , sizeof(RxBuf2));
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	HAL_GPIO_WritePin( LED3_GPIO_Port , LED3_Pin, GPIO_PIN_SET);
}

void data_engine(void)
{
char ttt,tty,j,
	st = 0,
	strbuf[128]={0},
	strtemp[32]={0};
	int16_t temp;
			if( txPackFlag == 1 || (flaq_recebido == 1 && kpacket == 1) || kTimePack == 1)
			{
					kTimePack = 0;
				  TimePack = 0;
					if( txPackFlag == 1 )
					{
						datarex.Alinhamento = 0;
						datarex.Peso_Esquerdo = 0;
						datarex.Peso_Direito = 0;
						datarex.Torque_Esquerdo = 0;
						datarex.Torque_Direito = 0;
						datarex.Suspencao_Esquerda = 0;
						datarex.Suspencao_Direita = 0;
						datarex.Spd1 = 0;
						datarex.Spd2 = 0;
						datarex.Status = 0;
					}
				  HAL_GPIO_TogglePin( LED4_GPIO_Port, LED4_Pin);
					if( flaq_recebido == 1) flaq_recebido = 0;
				  txPackFlag = 0;
					HAL_GPIO_WritePin( LED2_GPIO_Port , LED2_Pin, GPIO_PIN_SET);
	 // 1: Align
					sprintf( strbuf,"[%-d;", datarex.Alinhamento );
   //2-3:WBrk
						memset( strtemp, 0x00, 32);
					  temp = datarex.Peso_Esquerdo  * cfgx.fmult_weight - zero[0];
						sprintf( strtemp, "%-d;", temp );
						strcat( strbuf, strtemp );

						memset( strtemp, 0x00, 32);
					  temp = datarex.Peso_Direito * cfgx.fmult_weight - zero[1];
						sprintf( strtemp, "%-d;", temp );
						strcat( strbuf, strtemp );
	 //4-5:TBrk
						memset( strtemp, 0x00, 32);
						temp = datarex.Torque_Esquerdo * cfgx.fmult_torque - zero[2];
						sprintf( strtemp, "%-d;", temp );
						strcat( strbuf, strtemp );

						memset( strtemp, 0x00, 32);
						temp = datarex.Torque_Direito * cfgx.fmult_torque - zero[3];
						sprintf( strtemp, "%-d;", temp );
						strcat( strbuf, strtemp );
	 //6-7:Susp
						memset( strtemp, 0x00, 32);
						temp = datarex.Suspencao_Esquerda * cfgx.fmult_susp  - zero[4];
						sprintf( strtemp, "%-d;", temp );
						strcat( strbuf, strtemp );

						memset( strtemp, 0x00, 32);
						temp = datarex.Suspencao_Direita * cfgx.fmult_susp - zero[5];
						sprintf( strtemp, "%-d;", temp );
						strcat( strbuf, strtemp );
		//8:spd1
						memset( strtemp, 0x00, 32);
						sprintf( strtemp, "%05u;", datarex.Spd1 & 0x0000FFFF);
						strcat( strbuf, strtemp );
		//9:spd2
						memset( strtemp, 0x00, 32);
						sprintf( strtemp, "%05u;", datarex.Spd2 & 0x0000FFFF);
						strcat( strbuf, strtemp );
		//10:spd1
						memset( strtemp, 0x00, 32);
						sprintf( strtemp, "%05u;", datarex.Spd1 & 0x0000FFFF);
						strcat( strbuf, strtemp );
		//11:spd2
						memset( strtemp, 0x00, 32);
						sprintf( strtemp, "%05u;", datarex.Spd2 & 0x0000FFFF);
						strcat( strbuf, strtemp );
		//12: Alist
					memset( strtemp, 0x00, 32);
					sprintf( strtemp, "%03u;", datarex.Status  & 0x0000FFFF);
					strcat( strbuf, strtemp );

		//13:status
				dtpack = (void *) &datarex;
				if(klogicneg == 0)
				{
					if( dtpack->Status & ST_POS1)
						{
						HAL_GPIO_WritePin(POS1_GPIO_Port,POS1_Pin , GPIO_PIN_RESET);
						st += 0x01;
						}
						else
						HAL_GPIO_WritePin(POS1_GPIO_Port,POS1_Pin , GPIO_PIN_SET);
		
					if( dtpack->Status & ST_POS2)
					{
						HAL_GPIO_WritePin(POS2_GPIO_Port,POS2_Pin , GPIO_PIN_RESET);
						st += 0x10;
					}
					else
						HAL_GPIO_WritePin(POS2_GPIO_Port,POS2_Pin , GPIO_PIN_SET);
				}
				else
				{
					if( !(dtpack->Status & ST_POS1))
						{
						HAL_GPIO_WritePin(POS1_GPIO_Port,POS1_Pin , GPIO_PIN_RESET);
						st += 0x01;
						}
						else
						HAL_GPIO_WritePin(POS1_GPIO_Port,POS1_Pin , GPIO_PIN_SET);
		
					if( !(dtpack->Status & ST_POS2))
					{
						HAL_GPIO_WritePin(POS2_GPIO_Port,POS2_Pin , GPIO_PIN_RESET);
						st += 0x10;
					}
					else
						HAL_GPIO_WritePin(POS2_GPIO_Port,POS2_Pin , GPIO_PIN_SET);
				}

				if( (uint32_t) dtpack->Spd1 >= cfgx.speed_stop)
				{
						HAL_GPIO_WritePin(SPD1_GPIO_Port,SPD1_Pin , GPIO_PIN_RESET);
					  st += 0x20;
				}
				else
						HAL_GPIO_WritePin(SPD1_GPIO_Port,SPD1_Pin , GPIO_PIN_SET);

				if( (uint32_t) dtpack->Spd2 >= cfgx.speed_stop)
				{
						HAL_GPIO_WritePin(SPD2_GPIO_Port,SPD2_Pin , GPIO_PIN_RESET);
						st += 0x02;
				}
				else
						HAL_GPIO_WritePin(SPD2_GPIO_Port,SPD2_Pin , GPIO_PIN_SET);
					
				if(HAL_GPIO_ReadPin(RL1_GPIO_Port, RL1_Pin ) == GPIO_PIN_SET) st += 0x04;
				if(HAL_GPIO_ReadPin(RL3_GPIO_Port, RL3_Pin ) == GPIO_PIN_SET) st += 0x40;
				if(HAL_GPIO_ReadPin(RL6_GPIO_Port, RL6_Pin ) == GPIO_PIN_SET) st += 0x08;
				if(HAL_GPIO_ReadPin(RL7_GPIO_Port, RL7_Pin ) == GPIO_PIN_SET) st += 0x80;
				memset( strtemp, 0x00, 32);
		//14:pknumber
				sprintf( strtemp, "%04u;%04u]\r\n", st, cntPack );
				strcat( strbuf, strtemp );
				HAL_UART_Transmit_DMA( &huart2, (uint8_t *) strbuf , strlen( strbuf)); 
				net_SendString((uint8_t *) strbuf, strlen(strbuf));
			}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

   if( huart == &huart1)
			HAL_GPIO_WritePin( TXEN_GPIO_Port , TXEN_Pin, GPIO_PIN_RESET);
	 if (huart == &huart2)
			HAL_GPIO_WritePin( LED2_GPIO_Port , LED2_Pin, GPIO_PIN_RESET);

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1){
		// FRAMING, overrun or HIGH noise ERROR
		// Restart USART and continue communicating
		__HAL_UART_CLEAR_PEFLAG(&huart1);
		__HAL_UART_CLEAR_OREFLAG(&huart1);
		__HAL_UART_CLEAR_FEFLAG(&huart1);
		__HAL_UART_CLEAR_NEFLAG(&huart1);
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		HAL_UART_DMAStop(&huart1);
		HAL_UART_MspDeInit(&huart1);
		HAL_UART_MspInit(&huart1);
	  if ( HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) RxBuf, sizeof(RxBuf)) != HAL_OK)
		{
			Error_Handler();
		}
	}
	if (huart == &huart2){
		// FRAMING, overrun or HIGH noise ERROR
		// Restart USART and continue communicating
		HAL_UART_MspDeInit(&huart2);
		__HAL_UART_CLEAR_PEFLAG(&huart2);
		__HAL_UART_CLEAR_OREFLAG(&huart2);
		__HAL_UART_CLEAR_FEFLAG(&huart2);
		__HAL_UART_CLEAR_NEFLAG(&huart2);
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		HAL_UART_DMAStop(&huart2);
		HAL_UART_DeInit(&huart2);
		HAL_UART_MspInit(&huart2);
		if (HAL_UARTEx_ReceiveToIdle_DMA (&huart2, (uint8_t *) RxBuf2 , sizeof(RxBuf2)) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
	{
		comm_timer = comm_timer_period;
		flaq_recebido = 1;
		cntPack++;
		if (cntPack > 9999) cntPack = 0;
		PackTimer = 0;
		HAL_GPIO_TogglePin( LED3_GPIO_Port, LED3_Pin);
		
		oldPos = newPos;  // Update the last position before copying new data

		/* If the data in large and it is about to exceed the buffer size, we have to route it to the start of the buffer
		 * This is to maintain the circular buffer
		 * The old data in the main buffer will be overlapped
		 */
    memcpy ( (void *) &datarex, RxBuf, sizeof(datarex));
		__HAL_CRC_DR_RESET( &hcrc );
		uint32_t ccrc =  HAL_CRC_Accumulate( &hcrc, (uint32_t *) &datarex, 9);
		if( ccrc == datarex.crc )
				ccrc=0;
		if (oldPos + Size > sizeof(MainBuf))  // If the current position + new data size is greater than the main buffer
		{
			uint16_t datatocopy = sizeof(MainBuf) - oldPos;  // find out how much space is left in the main buffer
			memcpy ( (uint8_t *) MainBuf + oldPos, RxBuf, datatocopy);  // copy data in that remaining space
			oldPos = 0;  // point to the start of the buffer
			memcpy ((uint8_t *) MainBuf, (uint8_t *) RxBuf + datatocopy, (Size - datatocopy));  // copy the remaining data
			newPos = (Size - datatocopy);  // update the position
		}

		/* if the current position + new data size is less than the main buffer
		 * we will simply copy the data into the buffer and update the position
		 */
		else
		{
			memcpy ((uint8_t *) MainBuf + oldPos, RxBuf, Size);
			newPos = Size + oldPos;
		}
		flaq_recebido = 1;
		flagtx = 1;
		PackTimer = 0;
		/* start the DMA again */
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) RxBuf, sizeof(RxBuf));
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	}
	
	if (huart->Instance == USART2)
	{
		HAL_GPIO_TogglePin( LED2_GPIO_Port, LED2_Pin);
		HAL_Delay(1);
		memcpy ((uint8_t *)MainBuf2, (uint8_t *)RxBuf2, Size);
		memset(RxBuf2, 0x00, Size);
		rx2len = Size + 1;
		flaq_recebido2= 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) RxBuf2, sizeof(RxBuf2));
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
}

uint32_t zeros(void)
{
	uint32_t err=0;

	zero[0] = datarex.Peso_Esquerdo * cfgx.fmult_weight;
	zero[1] = datarex.Peso_Direito * cfgx.fmult_weight;
	zero[2] = datarex.Torque_Esquerdo * cfgx.fmult_torque;
	zero[3] = datarex.Torque_Direito * cfgx.fmult_torque;
	zero[4] = datarex.Suspencao_Esquerda * cfgx.fmult_susp;
	zero[5] = datarex.Suspencao_Direita * cfgx.fmult_susp;
	
	return err;
}

void clear_zeros(void)
{
			for( uint32_t i=0; i<6 ; i++)
			{
				zero[i] = 0;
			}
}


