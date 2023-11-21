#include "network.h"
#include "stdio.h"
#include "socket.h"
#include "dma.h"
#include "spi.h"
#include "socket.h"
#include "gpio.h"
#include "usart.h"
#include "string.h"
#include "protocol.h"
#include "cmsis_armclang.h"

__IO uint32_t spi_free=0;
uint8_t stat;
uint8_t reqnr;
uint8_t data_buf[DATA_BUF_SIZE];
uint32_t INTERFACE;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
uint8_t bfRX[256];



extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;
uint32_t net_state[MAX_SOCKETS] = {0};

uint8_t databuffer[DATA_BUF_SIZE];

wiz_NetInfo gWIZNETINFO = { .mac = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
														.ip = { 169, 254, 0, 100},
														.sn = { 255, 255, 255, 0},
														.gw = { 0, 0, 0, 1},
														.dns = { 0, 0, 0, 0},
														.dhcp = NETINFO_STATIC
													};

													
// chip select activate W5500
void W5500_Select(void)
	{
			HAL_GPIO_WritePin(  ETH_CS_GPIO_Port,  ETH_CS_Pin, GPIO_PIN_RESET);
			do {
				__ASM ("NOP");
			} while (HAL_GPIO_ReadPin(  ETH_CS_GPIO_Port,  ETH_CS_Pin) == GPIO_PIN_SET);
	}

// chip select desactivate W5500
void W5500_Unselect(void)
	{
			HAL_GPIO_WritePin(  ETH_CS_GPIO_Port,  ETH_CS_Pin, GPIO_PIN_SET);
			do {
					__ASM ("NOP");
			} while (HAL_GPIO_ReadPin(  ETH_CS_GPIO_Port,  ETH_CS_Pin) == GPIO_PIN_RESET);

	}

// read buffer via DMA
void W5500_ReadBuff( uint8_t * buff, uint16_t len)
	{
			spi_free=0;
			HAL_SPI_Receive_DMA( &hspi1, buff, len);
			while(!spi_free) {					__ASM ("NOP"); };
	}

// write bufer via DMA
void W5500_WriteBuff( uint8_t * buff, uint16_t len)
	{
			//while( hdma_spi_tx.Instance->NDTR > 0) { };
			spi_free=0;
			HAL_SPI_Transmit_DMA( &hspi1, buff, len);
			while(!spi_free) {					__ASM ("NOP");};
	}

// W5500 read byte
uint8_t W5500_ReadByte(void)
	{
		uint8_t buff;
				spi_free=0;
			//while (hdma_spi1_tx.Instance->NDTR != 0) {};
		    HAL_SPI_Receive( &hspi1, &buff, 1, 10);
				return buff;
	}

// W5500 write byte
void W5500_WriteByte(uint8_t byte)
	{
			spi_free=0;
			HAL_SPI_Transmit( &hspi1, &byte, 1, 0);
	}

// SPI Transmit complete callback
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
	{
			spi_free=1;
	}

	// SPI Receive complete callback
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
			spi_free=1;
}

void W5500_CRIS_ENTER(void)
	{

	}

void W5500_CRIS_EXIT(void){
}
													
													
void net_init(void)
{
	uint32_t sw;
	uint8_t v;
	INTERFACE = COMM_ETHERNET;
    for(v=0; v< MAX_SOCKETS;v++) net_state[v] = ST_IDLE;
// Hardware reset W5500
	  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
	  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
// initialize W5500 functions...
		reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);

		reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);

		reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);

		reg_wizchip_cris_cbfunc(W5500_CRIS_ENTER, W5500_CRIS_EXIT);

		uint8_t rx_tx_buff_sizes[] = { 2, 2, 2, 2, 2, 2, 2, 2};
		
		wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
		
// initialize network
		
// select IP according SW1 and SW2 positions
		
					wizchip_setnetinfo( &gWIZNETINFO);
					ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
// end select IP

}													

__IO uint8_t u;
void net_engine (void)
{
// LEDCTRL = ON if any TCP connected	
		if(net_state[0]==ST_CONNECTED || net_state[1]==ST_CONNECTED || net_state[2]==ST_CONNECTED || net_state[3]==ST_CONNECTED)
		{
			HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		}
// Net state machine
	for	(u=0;u<MAX_SOCKETS;u++) 
	{
		switch( net_state[u] )
		{
			case ST_IDLE:
// check - init
//					net_init();
						
						net_state[u] = ST_SOCKET;
			break;
			case ST_SOCKET:
// create socket
//					printf("Creating socket...\r\n");
			     //stat = socket(u, Sn_MR_UDP, PORT_LISTEN_TCP, 0);
							
			stat = socket(u, Sn_MR_TCP, PORT_LISTEN_TCP, SF_TCP_NODELAY);
					if(stat != u)
					{
//							printf("socket() failed, code = %d\r\n", stat);
							net_state[u] = ST_IDLE;
							break;
					}
					else 
					{
//							printf("Socket created, connecting...\r\n");
							net_state[u] = ST_LISTEN;
					}
			break;
			case ST_LISTEN:
					stat = listen(u);
					if(stat != SOCK_OK)
					{
//						printf("listen() failed, code = %d\r\n", stat);
						net_state[u] = ST_IDLE;
						break;
					}
					else
					{
//						printf("listen() OK\r\n");
						net_state[u] = ST_CLIENT;
					}
					break;
			case ST_CLIENT:
// Wait for Client Request		
					if(getSn_SR(u) == SOCK_ESTABLISHED )
					{
//						printf("Input connection\r\n");
						net_state[u] = ST_CONNECTED;
					}
					break;
// End Wait Client
			case ST_CONNECTED:
// test if connection establiched		
					if(getSn_SR(u) != SOCK_ESTABLISHED)
					{
//						printf("Error socket status\r\n");
						net_state[u] = ST_DISCONNECT;
						break;
					}
					else 
					{
//								while(getSn_SR(SOCKET_0) != SOCK_ESTABLISHED)
//								{
//									osDelay(1);
//								}								
					}
				break;
				case ST_DISCONNECT:
						disconnect(u);
//				printf("Closing socket.\r\n");
						close(u);
						net_state[u] = ST_IDLE;
				break;
		default:
				net_state[u] = ST_IDLE;
				break;
	}
}
}
__IO uint8_t ix;
int32_t net_SendString( uint8_t * str, uint16_t len)
{
	INTERFACE = COMM_ETHERNET;
		// send to serial port
		while( hdma_usart1_tx.Instance->CNDTR > 0)
		{ 
			HAL_Delay(1);
		}
		HAL_UART_Transmit_DMA( &huart2, (uint8_t *) str, len);

	
	// return transmitted length or SOCK_ERROR 
	for( ix = 0; ix < MAX_SOCKETS; ix++)
	{
		if(INTERFACE == COMM_ETHERNET)
		{
			if(net_state[ix] == ST_CONNECTED) 
					send(ix, str, len);
		}
		else
		{
		}
	}

	return (int32_t)len;
}

int32_t net_GetString( uint8_t socket, uint8_t * str){
uint16_t RSR_len;
int32_t rxlen = 0;
	if(INTERFACE == COMM_ETHERNET)
	{
		if(net_state[socket] == ST_CONNECTED )
		{	
			RSR_len = getSn_RX_RSR( socket);
			if( RSR_len > 0)
			{
				rxlen = recv( socket, str, RSR_len);
			}
		}
	}
	return rxlen;

}


