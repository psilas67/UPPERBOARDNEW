#include "protocol.h"
#include "main.h"
#include "leds.h"
#include "relays.h"
#include "tim_services.h"
#include "data_exchange.h"
#include "network.h"
#include "socket.h"
#include "stdlib.h" 
#include "tim.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "ctype.h"
#include "crc.h"
#include "sys_conf.h"

uint32_t lenx;
uint32_t sw=0;
extern uint32_t INTERFACE;
uint32_t reset_cause;
uint32_t sys_error;
uint32_t kdiag;
uint32_t kpacket;
uint32_t ksposl;
uint32_t ksposr;
uint32_t ksspdl;
uint32_t ksspdr;
uint32_t klogicneg;
uint32_t susp_delay;
uint32_t susp_samples;
uint32_t susp_cnt_samples;
uint32_t pack_time;


static uint8_t suspmsg[20];

static uint8_t rxmsg[256];
static char prtmsg[256];
static char tmpmsg[64];
uint32_t keepalive = 0;
uint32_t keepaliveperiod = 5000;
uint32_t err = 0;
uint32_t x = 0;
uint32_t l = 0;
int32_t rxlen;
uint32_t crcm;
int32_t arg1, arg2;
char * ptrx, * strini;
char tmp[16]={0};
__IO uint32_t ux;
uint8_t * rxxmsg;
volatile uint16_t flagtx = 0;

extern char MainBuf2[1024];
extern char RxBuf2[256];
extern char TxBuf2[256];
extern char rxpk[256];
extern uint32_t rx2len;
extern uint32_t oldPos2;
extern uint32_t newPos2;
extern uint32_t flaq_recebido2;

const uint8_t rstPOR[] = "reseted POR\r\n";
const uint8_t rstSFT[] = "reseted SFT\r\n";
const uint8_t rstIWDT[] = "reseted IWDT\r\n";
const uint8_t rstWWDT[] = "reseted WWDT\r\n";
const uint8_t rstBOR[] = "reseted BOR\r\n";
const uint8_t rstLPW[] = "reseted LPW\r\n";
const uint8_t rstPIN[] = "reseted PIN\r\n";
const uint8_t rstUNK[] = "reseted UNK\r\n";

const uint8_t str0[] = "kp\r\n";
const uint8_t str1[] = "@reset OK\r\n";
const uint8_t str1r[] = "@reset SystemOK\r\n";
const uint8_t str2[] = "@MOTOR1_ON=OK\r\n";
const uint8_t str3[] = "@MOTOR1_OFF=OK\r\n";
const uint8_t str4[] = "@MOTOR1=ERROR\r\n";
const uint8_t str5[] = "@MOTOR2_ON=OK\r\n";
const uint8_t str6[] = "@MOTOR2_OFF=OK\r\n";
const uint8_t str7[] = "@MOTOR2=ERROR\r\n";
const uint8_t str8[] = "@MOTOR3_ON=OK\r\n";
const uint8_t str9[] = "@MOTOR3_OFF=OK\r\n";
const uint8_t str10[] = "@MOTOR3=ERROR\r\n";
const uint8_t str11[] = "@MOTOR4_ON=OK\r\n";
const uint8_t str12[] = "@MOTOR4_OFF=OK\r\n";
const uint8_t str13[] = "@MOTOR4=ERROR\r\n";
const uint8_t str14[] = "@RELAY=OK\r\n";
const uint8_t str15[] = "@RELAY=ERROR\r\n";
const uint8_t str16[] = "@MOTROR1_ON=OK\r\n";
const uint8_t str17[] = "@MOTROR1_ON=ERROR\r\n";
const uint8_t str18[] = "@MOTROR2_ON=OK\r\n";
const uint8_t str19[] = "@MOTROR2_ON=ERROR\r\n";
const uint8_t str20[] = "@BREAK_N_ON=OK\r\n";
const uint8_t str21[] = "@BREAK_N_OFF=OK\r\n";
const uint8_t str160[] = "@MOTROR1_ON=OK\r\n";
const uint8_t str170[] = "@MOTROR1_ON=ERROR\r\n";
const uint8_t str180[] = "@MOTROR2_I_ON=OK\r\n";
const uint8_t str190[] = "@MOTROR2_I_ON=ERROR\r\n";
const uint8_t str200[] = "@BREAK_I_ON=OK\r\n";
const uint8_t str210[] = "@BREAK_I_OFF=OK\r\n";
const uint8_t str220[] = "@BREAK_OFF=OK\r\n";
const uint8_t str22[] = "@BREAK_L=OK\r\n";
const uint8_t str23[] = "@BREAK_L=ERROR\r\n";
const uint8_t str24[] = "@BREAK_L=OFF\r\n";
const uint8_t str25[] = "@BREAK_R=OK\r\n";
const uint8_t str26[] = "@BREAK_R=ERROR\r\n";
const uint8_t str27[] = "@BREAK_R=OFF\r\n";

const uint8_t str28[] = "@BREAK_EXN_M1ON=OK\r\n";
const uint8_t str29[] = "@BREAK_EXN_M1ON=ERROR\r\n";
const uint8_t str30[] = "@BREAK_EXN_M2ON=OK\r\n";
const uint8_t str31[] = "@BREAK_EXN_M2ON=ERROR\r\n";
const uint8_t str32[] = "@BREAK_EXN_OFF=OK\r\n";

const uint8_t str33[] = "@BREAK_EXI_M1ON=OK\r\n";
const uint8_t str34[] = "@BREAK_EXI_M1ON=ERROR\r\n";
const uint8_t str35[] = "@BREAK_EXI_M2ON=OK\r\n";
const uint8_t str36[] = "@BREAK_EXI_M2ON=ERROR\r\n";
const uint8_t str37[] = "@BREAK_EXI_OFF=OK\r\n";

const uint8_t str38[] = "@SUSP_CYCLE_L_START\r\n";
const uint8_t str39[] = "@SUSP_MOTOR_L=ON\r\n";
const uint8_t str40[] = "@SUSP_MOTOR_L=OFF\r\n";
const uint8_t str41[] = "@BEGIN_SAMPLE_L_BURST\r\n";
const uint8_t str42[] = "@END_SAMPLE_L_BURST\r\n";
const uint8_t str43[] = "@SUSP_CYCLE_R_START\r\n";
const uint8_t str44[] = "@SUSP_MOTOR_R=ON\r\n";
const uint8_t str45[] = "@SUSP_MOTOR_R=OFF\r\n";
const uint8_t str46[] = "@BEGIN_SAMPLE_R_BURST\r\n";
const uint8_t str47[] = "@END_SAMPLE_R_BURST\r\n";
const uint8_t str48[] = "@SUSP_TIME=OK\r\n";
const uint8_t str49[] = "@SUSP_TIME=ERROR\r\n";
const uint8_t str50[] = "@SUSP_SAMPLES=OK\r\n";
const uint8_t str51[] = "@SUSP_SAMPLES=ERROR\r\n";
const uint8_t str52[] = "@PACK_TIME=OK\r\n";
const uint8_t str53[] = "@PACK_TIME=ERROR\r\n";

const uint8_t str54[] = "@SEND_PACK_ON=OK\r\n";
const uint8_t str55[] = "@SEND_PACK_OFF=OK\r\n";
const uint8_t str56[] = "@SEND_PACK=ERROR\r\n";

const uint8_t str57[] = "@DIAG_ON=OK\r\n";
const uint8_t str58[] = "@DIAG_OFF=OK\r\n";
const uint8_t str59[] = "@DIAG=ERROR\r\n";

const uint8_t str60[] = "@ZEROS_ON=OK\r\n";
const uint8_t str61[] = "@ZEROS_OFF=OK\r\n";
const uint8_t str62[] = "@ZEROS=ERROR\r\n";

const uint8_t str63[] = "@LOGIC_NEG=1\r\n";
const uint8_t str64[] = "@LOGIC_NEG=0\r\n";
const uint8_t str65[] = "@LOGIC_NEG=ERROR\r\n";

const uint8_t str66[] = "@SPOSL=1\r\n";
const uint8_t str67[] = "@SPOSL=0\r\n";
const uint8_t str68[] = "@SPOSL=ERROR\r\n";

const uint8_t str69[] = "@SPOSR=1\r\n";
const uint8_t str70[] = "@SPOSR=0\r\n";
const uint8_t str71[] = "@SPOSR=ERROR\r\n";

const uint8_t str72[] = "@SSPDL=1\r\n";
const uint8_t str73[] = "@SSPDL=0\r\n";
const uint8_t str74[] = "@SSPDL=ERROR\r\n";

const uint8_t str75[] = "@SSPDR=1\r\n";
const uint8_t str76[] = "@SSPDR=0\r\n";
const uint8_t str77[] = "@SSPDR=ERROR\r\n";

const uint8_t str78[] = "@MSAVE=OK\r\n";
const uint8_t str79[] = "@MSAVE=ERROR\r\n";


const uint8_t str100[] = "@SET_KEEPALIVE=OK\r\n";
const uint8_t str110[] = "@SET_KEEPALIVE=ERROR\r\n";

const uint8_t str250[] = "@INVALID COMMAND\r\n";
const uint8_t str251[] = "@FTMW=ERROR\r\n";
const uint8_t str252[] = "@FTDW=ERROR\r\n";
const uint8_t str253[] = "@FTMT=ERROR\r\n";
const uint8_t str254[] = "@FTDT=ERROR\r\n";
const uint8_t str255[] = "@FTMS=ERROR\r\n";
const uint8_t str256[] = "@FTDS=ERROR\r\n";
const uint8_t str257[] = "@CUSTOMER=ERROR\r\n";
const uint8_t str258[] = "@SERIAL=ERROR\r\n";
const uint8_t strVER[] = "Mectronix Tecnologia\r\nN e w L i n e\r\n0";
const uint8_t strVersion[] = "@V:23.11.15  Nov/2023\r\n";

void	show_configs(void)
{
	char strb[64] = {0};
	    net_SendString((uint8_t *) "Mectronix Tecnologia\r\nNewLine V22.11.15 Nov/2023\r\n\0", strlen((const char *) strVER) );
	    HAL_Delay(1);
			net_SendString((uint8_t *) strVersion, strlen((const char *)strVersion));
	    HAL_Delay(1);
		  memset( prtmsg, 0x00, sizeof( prtmsg ));
	    sprintf( prtmsg , "Customer: %s\r\n", cfg->customer_name );
		  net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
	    HAL_Delay(2);
		  memset( prtmsg, 0x00, sizeof( prtmsg ));
	    sprintf( prtmsg, "Serial: %s\r\n", cfg->serial_number );
		  net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
	    HAL_Delay(1);
			sprintf( strb, "Speed Limit = %u       Logic Neg = %u\r\nPak Time = %u \r\n", cfgx.speed_stop, klogicneg, pack_time);
	    net_SendString((uint8_t *) strb, strlen( strb));
	    HAL_Delay(1);
			memset( prtmsg, 0x00, sizeof( prtmsg ));
    	sprintf( prtmsg, "@FTMW = %d ,FTDW = %d\r\n", cfgx.fmult_weight, cfgx.fdiv_weight);
		  net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
	    HAL_Delay(1);
    	sprintf( prtmsg, "@FTMT = %d ,FTDT = %d\r\n", cfgx.fmult_torque, cfgx.fdiv_torque);
		  net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
	    HAL_Delay(1);
    	sprintf( prtmsg, "@FTMS = %d ,FTDS = %d\r\n", cfgx.fmult_susp, cfgx.fdiv_susp);
		  net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
}

void protocol_init(void)
{
		INTERFACE = COMM_ETHERNET;
		susp_delay = 2000;
		susp_samples = 400;
		susp_cnt_samples = 0;
		//pack_time = 500;
		kdiag = 0;
		kpacket = 0;
		ksposl = 1;
		ksposr = 1;
		ksspdl = 1;
		ksspdr = 1;
		sys_error = 0;
		//klogicneg = 0; // get config
}

void Tx2Lower( uint8_t * txt, uint32_t len );

void protocol_Engine(void){
uint32_t i = 0;
// Keep Alive
				if((keepaliveperiod > 0) && (HAL_GetTick() > keepalive))
				{
						keepalive = HAL_GetTick() + keepaliveperiod;
//
					net_SendString( (uint8_t *) str0, strlen((const char *)str0));
				}	 
// SUSP SAMPLES
				if(susp_cnt_samples > 0)
				{
						net_SendString( (uint8_t *) suspmsg, sizeof(suspmsg));
				}	 
// get string if received
				for(ux=0;ux < MAX_SOCKETS; ux++)
				{
					rxxmsg = (uint8_t *) &rxmsg;
					memset( rxxmsg, 0x00, 256);
					rxlen = 0;
					if( net_state[ux] == ST_CONNECTED)
							rxlen = net_GetString(ux, rxxmsg);
					if( rxlen > 0)
						parse_engine( rxxmsg, rxlen);
				}
					if( flaq_recebido2 )
					{
						rxxmsg = (uint8_t *) &MainBuf2;
						parse_engine( rxxmsg, rx2len);
						flaq_recebido2 = 0;
						memset( MainBuf2, 0x00, rx2len);
						rx2len = 0;
					}
}


void	Tx2Lower( uint8_t * txt, uint32_t len)
{
	uint16_t timeout;
		timeout = 100;
		flagtx = 0;
		do
		{
			HAL_Delay(1);
			timeout--;
		} while (flagtx == 0 && timeout > 0);
		HAL_GPIO_WritePin( TXEN_GPIO_Port, TXEN_Pin,GPIO_PIN_SET);
		HAL_Delay(2);
		HAL_UART_Transmit( &huart1, txt, len, 5 );
		HAL_Delay(2);
		HAL_GPIO_WritePin( TXEN_GPIO_Port, TXEN_Pin,GPIO_PIN_RESET);
}
// ---
char recv_buffer[128] = {0}; // recv_buffer
char cmd_buffer[128] = {0}; // sanitized command to be executed

uint32_t recv_buffer_len = 0;
// Receive buffer waits for \n character before parse message
// Incoming chars are copied to recv_buffer
// When a '\n' char is received the sanitized command is copied to cmd_buffer
// recv_buffer_processing() return 1 when cmd_buffer has a parseable command
// and return 0 when it is not ready yet

uint8_t recv_buffer_processing(uint8_t * rxmsg, uint32_t len) {
	uint8_t is_ready_to_parse = 0;
	
	for (uint32_t i=0; i < len; i++) 
	{
// Full buffer, copy to cmd_buffer[] anyways
		if (recv_buffer_len >= sizeof(recv_buffer)) 
		{
			recv_buffer[sizeof(recv_buffer)-1] = '\0';
			strncpy(cmd_buffer, recv_buffer, sizeof(cmd_buffer));
			recv_buffer_len = 0;
			return 1;
		}

		char ch = rxmsg[i];
		uint32_t rx_ix = recv_buffer_len;
		recv_buffer[rx_ix] = ch;
		recv_buffer_len++;
// full reset		
		if (ch == '!')
			HAL_NVIC_SystemReset();
// partial reset
		if (ch == '$')
		{	
			Tx2Lower( (uint8_t *) "##", 2);
			memset(cmd_buffer, 0x00, sizeof(cmd_buffer));
			recv_buffer_len = 0;
			Reset_System();
			return 0;
		}
// all motors off
		if (ch == '#')
		{	
			memset(cmd_buffer, 0x00, sizeof(cmd_buffer));
			recv_buffer_len = 0;
			motor1_off();
			motor2_off();
			motor3_off();
			motor4_off();
			HAL_Delay(10);
			return 0;
		}
// Completed message, copy to cmd_buffer[]
		if (ch == '\n' || ch == '\r' || ch == '\0') 
		{
			recv_buffer[rx_ix] = '\0';
			strncpy(cmd_buffer, recv_buffer, sizeof(cmd_buffer));
			recv_buffer_len = 0;
			return 1;
		}
		if (ch == 'v') 
		{
			recv_buffer[rx_ix] = 'v';
			cmd_buffer[0] = 'v';
			cmd_buffer[1] = 0x00;
			recv_buffer_len = 0;
			return 1;
		}

	}
	// Keep waiting for remaining chars
	return 0;
}
// reset w/o network
void Reset_System(void)
{
	Tx2Lower( (uint8_t *) "##", 2);
	protocol_init();
	relays_init();
	data_engine_init();
	leds_init();
	show_configs();
}

uint32_t memory_save(void)
{
	sysconf_write( &cfgx );
	return 0;
}

// parse old commands
void parse_engine( uint8_t * rxmsg, uint32_t len)
{
	uint8_t is_ready_to_parse = recv_buffer_processing(rxmsg, len);
	if (!is_ready_to_parse) return;
	
	rxmsg = (uint8_t*) cmd_buffer;
	uint32_t i;

// $reset			
					if( (strstr((char *) rxmsg ,"!") != NULL) )
					{
							net_SendString( (uint8_t *) str1, strlen((const char *)str1));
							Tx2Lower( (uint8_t *) "##", 2);	
							NVIC_SystemReset();	
							return;
					}

// partial reset			
					if( (strstr((char *) rxmsg ,"$") != NULL))
					{
							net_SendString( (uint8_t *) str1r, strlen((const char *)str1r));
							Tx2Lower( (uint8_t *) "##", 2);	
							Reset_System();
							return;
					}
		
					
// MOTOR1		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "M1" ); 
					if(ptrx != NULL )
					{
							err = motor1_on();
							if(err == 0)
									net_SendString((uint8_t *) str2, strlen((const char *)str2));
							else
									net_SendString((uint8_t *) str4, strlen((const char *)str4));
							return;
					}

// MOTOR2		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "M2"  ); 
					if(ptrx != NULL )
					{
							err = motor2_on(0);
							if(err == 0)
									net_SendString((uint8_t *) str5, strlen((const char *)str5));
							else
									net_SendString((uint8_t *) str7, strlen((const char *)str7));
							return;
					}
					
// MOTOR3		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "M3" ); 
					if(ptrx != NULL )
					{
							motor3_on();
							net_SendString((uint8_t *) str8, strlen((const char *)str8));
							return;
					}
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SL" ); 
					if(ptrx != NULL && strlen( (char *) rxmsg) == 2)
					{
							motor3_on();
							net_SendString((uint8_t *) str8, strlen((const char *)str8));
							return;
					}
// MOTOR4		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "M4" ); 
					if(ptrx != NULL )
					{
								motor4_on();
								net_SendString((uint8_t *) str11, strlen((const char *)str11));
							return;
					}
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SR" ); 
					if(ptrx != NULL && strlen( (char *) rxmsg) == 2)
					{
								motor4_on();
								net_SendString((uint8_t *) str11, strlen((const char *)str11));
							return;
					}

// MOTOR0		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "M0" ); 
					if(ptrx != NULL && strlen( (char *) rxmsg) == 2)
					{
												motor1_off();
												net_SendString((uint8_t *) str3, strlen((const char *)str3));
												motor2_off();
												net_SendString((uint8_t *) str6, strlen((const char *)str6));
												motor3_off();
												net_SendString((uint8_t *) str9, strlen((const char *)str9));
												motor4_off();
												net_SendString((uint8_t *) str12, strlen((const char *)str12));
					}
										ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "#" ); 
					if(ptrx != NULL)
					{
												motor1_off();
												net_SendString((uint8_t *) str3, strlen((const char *)str3));
												motor2_off();
												net_SendString((uint8_t *) str6, strlen((const char *)str6));
												motor3_off();
												net_SendString((uint8_t *) str9, strlen((const char *)str9));
												motor4_off();
												net_SendString((uint8_t *) str12, strlen((const char *)str12));
					}
// BREAK_N=x				
					if( strstr((char *) rxmsg, "FN") != NULL )
					{
								err = motor1_on();
								if(err == 0)
									net_SendString((uint8_t *) str16, strlen((const char *)str16));
								else
									net_SendString((uint8_t *) str17, strlen((const char *)str17));
								err = motor2_on(0);
								if(err == 0)
								{
									net_SendString((uint8_t *) str18, strlen((const char *)str18));
									net_SendString((uint8_t *) str20, strlen((const char *)str20));
									return;
								}
								else
									net_SendString((uint8_t *) str19, strlen((const char *)str19));
								return;
					}

// BREAK_I=x				
					if( strstr((char *) rxmsg, "FI") != NULL )
					{
								err = motor1_on();
								if(err == 0)
									net_SendString((uint8_t *) str160, strlen((const char *)str160));
								else
								{
									net_SendString((uint8_t *) str170, strlen((const char *)str170));
									
								}
								err = motor2_on(1);
								if(err == 0)
								{
									net_SendString((uint8_t *) str180, strlen((const char *)str180));
									net_SendString((uint8_t *) str200, strlen((const char *)str200));
								}
								else
									net_SendString((uint8_t *) str190, strlen((const char *)str190));
								return;
					}

// BREAK_OFF				
					if( strstr((char *) rxmsg, "BREAK=OFF") != NULL)
					{
								motor1_off();
								motor2_off();
								net_SendString((uint8_t *) str220, strlen((const char *)str220));
								return;
					}


// BREAK_L=x				
					if( strstr((char *) rxmsg, "BREAK_L=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motor1_on();
								if(err == 0)
									net_SendString((uint8_t *) str22, strlen((const char *)str22));
								else
								{
									net_SendString((uint8_t *) str23, strlen((const char *)str23));
								}
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									net_SendString((uint8_t *) str24, strlen((const char *)str24));
									return;
								}
					}

// BREAK_R=x				
					if( strstr((char *) rxmsg, "BREAK_R=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motor2_on(1);
								if(err == 0)
									net_SendString((uint8_t *) str25, strlen((const char *)str25));
								else
								{
									net_SendString((uint8_t *) str26, strlen((const char *)str26));
								}
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									net_SendString((uint8_t *) str27, strlen((const char *)str27));
									return;
								}
					}

// BREAK_EXN=x				
					if( strstr((char *) rxmsg, "BREAK_EXN=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motorx1_on();
								if(err == 0)
									net_SendString((uint8_t *) str28, strlen((const char *)str28));
								else
									net_SendString((uint8_t *) str29, strlen((const char *)str29));
								err = motorx2_on(0);
								if(err == 0)
									net_SendString((uint8_t *) str30, strlen((const char *)str30));
								else
									net_SendString((uint8_t *) str31, strlen((const char *)str31));
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									motor2_off();
									net_SendString((uint8_t *) str32, strlen((const char *)str32));
									return;
								}
					}

// BREAK_EXI=x				
					if( strstr((char *) rxmsg, "BREAK_EXI=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motorx1_on();
								if(err == 0)
									net_SendString((uint8_t *) str33, strlen((const char *)str33));
								else
								{
									net_SendString((uint8_t *) str34, strlen((const char *)str34));
									
								}
								err = motorx2_on(1);
								if(err == 0)
									net_SendString((uint8_t *) str35, strlen((const char *)str35));
								else
									net_SendString((uint8_t *) str36, strlen((const char *)str36));
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									motor2_off();
									net_SendString((uint8_t *) str37, strlen((const char *)str37));
									return;
								}
					}

					

// SUSP CYCLE Left				

					if( strstr((char *) rxmsg, "SUSP_CYCLE_L") != NULL)
					{
							net_SendString((uint8_t *) str38, strlen((const char *)str38));
							net_SendString((uint8_t *) str39, strlen((const char *)str39));
							motor3_on();
							HAL_Delay( susp_delay );
							motor3_off();
							net_SendString((uint8_t *) str40, strlen((const char *)str40));
							susp_cnt_samples = susp_cnt_samples;
							net_SendString((uint8_t *) str41, strlen((const char *)str41));
							memcpy( suspmsg, str42, 20);
						return;
					}
					

// SUSP CYCLE Right				

					if( strstr((char *) rxmsg, "SUSP_CYCLE_R") != NULL)
					{
							net_SendString((uint8_t *) str43, strlen((const char *)str43));
							net_SendString((uint8_t *) str44, strlen((const char *)str44));
							motor4_on();
							HAL_Delay( susp_delay );
							motor4_off();
							net_SendString((uint8_t *) str45, strlen((const char *)str45));
							susp_cnt_samples = susp_cnt_samples;
							net_SendString((uint8_t *) str46, strlen((const char *)str46));
							memcpy( suspmsg, str47, 20);
						return;
					}
					
// SUSP_TIME = 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SUSP_TIME=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 < 500) && (arg2  > 10000)) err = 1;
							if( err == 0)
							{
									susp_delay = arg2;
									net_SendString((uint8_t *) str48, strlen((const char *)str48));
						}
						else
						{
									net_SendString((uint8_t *) str49, strlen((const char *)str49));
						}
						return;
					}
					
// SUSP_SAMPLES = 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SUSP_SAMPLES=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 < 50) && (arg2  > 1000)) err = 1;
							if( err == 0)
							{
									susp_samples = arg2;
									net_SendString((uint8_t *) str50, strlen((const char *)str50));
						}
						else
						{
									net_SendString((uint8_t *) str51, strlen((const char *)str51));
						}
						return;
					}
// Dnnnn ;		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "D" ); 
					if(ptrx != NULL && rxmsg[0] == 'D')
					{
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( err == 0)
							{
									cfgx.speed_stop = arg2;
									memset( tmpmsg, 0x00, 64);
									sprintf( tmpmsg, "Speed Stop = %03d\r\n", arg2);
									Tx2Lower( (uint8_t *) tmpmsg, strlen( tmpmsg ) );
//									send command to slave
									net_SendString((uint8_t *) str52, strlen((const char *)str52));
						}
						else
						{
									net_SendString((uint8_t *) str53, strlen((const char *)str53));
						}
						return;
					}

// Tnnnn		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "T" ); 
					if(ptrx != NULL && rxmsg[0] == 'T')
					{
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 < 15) && (arg2  > 1000)) err = 1;
							if( err == 0)
							{
									pack_time = arg2;
								  txPackPeriod = pack_time;
								  if( pack_time == 500 || pack_time == 1000) TimePackPeriod = pack_time;
									memset( tmpmsg, 0x00, 64);
									sprintf( tmpmsg, "T%03d\r\n", pack_time);
									Tx2Lower( (uint8_t *) tmpmsg, strlen( tmpmsg ) );
//									send command to slave
									net_SendString((uint8_t *) str52, strlen((const char *)str52));
						}
						else
						{
									net_SendString((uint8_t *) str53, strlen((const char *)str53));
						}
						return;
					}
					
					
// P0;P1		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "P"); 
					if(ptrx != NULL && rxmsg[0] == 'P')
					{
							err = 0;
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												kpacket = 1;
												net_SendString((uint8_t *) str54, strlen((const char *)str54));
												HAL_Delay(1);
											  return;
										}else
										{
												kpacket = 0;
											  TimePackPeriod = 1000;
 											  net_SendString((uint8_t *) str55, strlen((const char *)str55));
											  return;
										}
							}
							else
							{
										net_SendString((uint8_t *) str56, strlen((const char *)str56));
							}
						return;
					}
				
// G0;G1		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "G" ); 
					if(ptrx != NULL && rxmsg[0] == 'G')
					{
						  err = 0;
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												kdiag = 1;
												net_SendString((uint8_t *) str57, strlen((const char *)str57));
											  return;
										}else
										{
												kdiag = 0;
												net_SendString((uint8_t *) str58, strlen((const char *)str58));
											  return;
										}
							}
							else
							{
										net_SendString((uint8_t *) str59, strlen((const char *)str59));
							}
						return;
					}
					
// Z0;Z1;Z?		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "Z" ); 
					if(ptrx != NULL && rxmsg[0] == 'Z')
					{
						  err = 0;
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( (tmp[0] != '0') && (tmp[0] != '1') && (tmp[0] != '?') ) err = 1;
							if( err == 0 )
							{
										if(tmp[0] == '?') 
										{
											memset( prtmsg, 0x00, sizeof( prtmsg ));
											sprintf( prtmsg, "@Zeros=OK:\r\n");
											for(x=0; x<6; x++)
											{
												memset( tmpmsg, 0x00, 64);
												sprintf( tmpmsg, "@Zero[%d]=%04d\r\n", x, zero[x]);
												strcat( prtmsg, tmpmsg);
											}
											net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
											return;
										}
										else
										{
											arg1 = (uint32_t) atoi(tmp);
											if( arg1 == 1) 
											{
													zeros();
													net_SendString((uint8_t *) str60, strlen((const char *)str60));
											}
											else
											{
													clear_zeros();
													net_SendString((uint8_t *) str61, strlen((const char *)str61));
											}
										}
							}
							else
							{
										net_SendString((uint8_t *) str62, strlen((const char *)str62));
							}
							return;
					}

// LOGIC_NEG=x		
					if((strstr((char *) rxmsg, "LN") != NULL) || (strstr((char *) rxmsg, "LP") != NULL))
					{
							ptrx = strstr((char *) rxmsg, "LN" ); 
							if(ptrx != NULL)
								klogicneg = 1;
							ptrx = strstr((char *) rxmsg, "LP" ); 
							if(ptrx != NULL)
								klogicneg = 0;
							cfgx.logic_neg = klogicneg;
							memset( prtmsg, 0x00, sizeof( prtmsg ));
							sprintf( prtmsg, "@LOGIC_NEG=%d\r\n", klogicneg );
							net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
							return;
					}
					
// SPOSL=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SPOSL=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksposl = 1;
												net_SendString((uint8_t *) str66, strlen((const char *)str66));
										}else
										{
												ksposl = 0;
												net_SendString((uint8_t *) str66, strlen((const char *)str66));
										}
							}
							else
							{
										net_SendString((uint8_t *) str67, strlen((const char *)str67));
							}
							return;
					}
					
// SPOSR=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SPOSR=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksposr = 1;
												net_SendString((uint8_t *) str68, strlen((const char *)str68));
										}else
										{
												ksposr = 0;
												net_SendString((uint8_t *) str69, strlen((const char *)str69));
										}
							}
							else
							{
										net_SendString((uint8_t *) str70, strlen((const char *)str70));
							}
							return;
					}
					
// SSPDL=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SSPDL=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksspdl = 1;
												net_SendString((uint8_t *) str71, strlen((const char *)str72));
										}else
										{
												ksspdl = 0;
												net_SendString((uint8_t *) str73, strlen((const char *)str73));
										}
							}
							else
							{
										net_SendString((uint8_t *) str74, strlen((const char *)str74));
							}
							return;
					}
					
// SSPDR=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SSPDR=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksspdr = 1;
												net_SendString((uint8_t *) str75, strlen((const char *)str75));
										}else
										{
												ksspdr = 0;
												net_SendString((uint8_t *) str76, strlen((const char *)str76));
										}
							}
							else
							{
										net_SendString((uint8_t *) str77, strlen((const char *)str77));
							}
							return;
					}
					
// MSAVE				

					if( strstr((char *) rxmsg, "v") != NULL)
					{
							if( memory_save() == 0)
							{
								net_SendString((uint8_t *) str78, strlen((const char *)str78));
							}
							else
							{
								net_SendString((uint8_t *) str37, strlen((const char *)str37));
							}
							return;
					}		
	
// FTMW=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "FTMW=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if( (isdigit(strini[i])==0) && (strini[i] != '-') ) err = 1;
							}
							arg2 = (int32_t) atoi( strini );
							if( (arg2 == 0) ) err = 1;
							if( err == 0)
							{
									cfgx.fmult_weight = arg2;
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@FTMW = %d\r\n", cfgx.fmult_weight );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
							}
							else
							{
									net_SendString((uint8_t *) str251, strlen((const char *)str251));
							}
							return;
					}

// FTDW=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "FTDW=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if( (isdigit(strini[i])==0) && (strini[i] != '-') ) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( arg2 == 0) err = 1;
							if( err == 0)
							{
									cfgx.fdiv_weight = arg2;
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@FTDW = %d\r\n", cfgx.fdiv_weight );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
						}
						else
						{
									net_SendString((uint8_t *) str252, strlen((const char *)str252));
						}
						return;
					}

// FTMT=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "FTMT=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if( (isdigit(strini[i])==0) && (strini[i] != '-') ) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( arg2 == 0) err = 1;
							if( err == 0)
							{
									cfgx.fmult_torque = arg2;
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@FTMT = %d\r\n", cfgx.fmult_torque );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
						}
						else
						{
									net_SendString((uint8_t *) str253, strlen((const char *)str253));
						}
						return;
					}

// FTDT=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "FTDT=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if( (isdigit(strini[i])==0) && (strini[i] != '-') ) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( arg2 == 0) err = 1;
							if( err == 0)
							{
									cfgx.fdiv_torque = arg2;
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@FTDT = %d\r\n", cfgx.fdiv_torque );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
						}
						else
						{
									net_SendString((uint8_t *) str254, strlen((const char *)str254));
						}
						return;
					}

// FTMS=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "FTMS=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if( (isdigit(strini[i])==0) && (strini[i] != '-') ) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( arg2 == 0) err = 1;
							if( err == 0)
							{
									cfgx.fmult_susp = arg2;
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@FTMS = %d\r\n", cfgx.fmult_susp );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
						}
						else
						{
									net_SendString((uint8_t *) str255, strlen((const char *)str255));
						}
						return;
					}

// FTDS=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "FTDS=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if( (isdigit(strini[i])==0) && (strini[i] != '-') ) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( arg2 == 0) err = 1;
							if( err == 0)
							{
									cfgx.fdiv_susp = arg2;
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@FTDS = %d\r\n", cfgx.fdiv_susp );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
						}
						else
						{
									net_SendString((uint8_t *) str256, strlen((const char *)str256));
						}
						return;
					}

// CUSTOM=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "CUSTOM=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							if( err == 0)
							{
									strcpy( cfgx.customer_name, strini);
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@CUSTOM = %s\r\n", cfgx.customer_name );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
						}
						else
						{
									net_SendString((uint8_t *) str257, strlen((const char *)str256));
						}
						return;
					}

// SERIAL=xxxxx 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SERIAL=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							if( err == 0)
							{
									strcpy(cfgx.serial_number, strini);
									memset( prtmsg, 0x00, sizeof( prtmsg ));
    							sprintf( prtmsg, "@SERIAL = %s\r\n", cfgx.serial_number );
		    					net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
							}
							else
							{
									net_SendString((uint8_t *) str258, strlen((const char *)str256));
							}
							return;
					}

					
// get-version			
					if( (strstr((char *) rxmsg ,"GET-VERSION") != NULL) && (strlen((char *) rxmsg) < 12))
					{
							
							net_SendString((uint8_t *) strVersion, strlen((const char *)strVersion));
							return;
					}

// set keep alive
				// sintaxe : set-keepalive xxxx\n
				// xxxx -> miliseconds 0 to 99999
					if( strstr((char *) rxmsg, "SET_KEEPALIVE=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 > 0 && arg2  < 100) || (arg2 > 120000)) err = 1;
							if( err == 0)
							{
									keepaliveperiod = arg2;
									net_SendString((uint8_t *) str100, strlen((const char *)str100));
						}
						else
						{
									net_SendString((uint8_t *) str110, strlen((const char *)str110));
						}
						return;
					}

// GET_CONFIG

				if( (strstr((char *) rxmsg, "GET_CONFIG") != NULL) && (strlen((const char *)rxmsg) == 10))
				{	
					show_configs();	
					return;
				}

// invalid command
				net_SendString((uint8_t *) str250, strlen((const char *)str250));
}





