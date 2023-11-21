#ifndef __network__
#define __network__
#include "stdint.h"


#define SOCKET_0		0
#define SOCKET_1		1
#define SOCKET_2		2
#define SOCKET_3		3
#define MAX_SOCKETS	4

#define SOCK_TCPS			0
#define PORT_LISTEN_TCP			1001

#define	DATA_BUF_SIZE	2048

enum{
		ST_IDLE = 0,
		ST_SOCKET,
		ST_LISTEN,
		ST_CLIENT,
		ST_CONNECTED,
		ST_DISCONNECT
};
enum {
COMM_ETHERNET,
COMM_UART
};
extern uint32_t net_state[MAX_SOCKETS];

void net_init(void);
void net_engine (void);
int32_t net_SendString( uint8_t * str, uint16_t len);
int32_t net_GetString( uint8_t socket, uint8_t * str);

#endif

