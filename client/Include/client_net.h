#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <SDL2/SDL_net.h>
//#include "network_protocol.h"
#include "protocol.h"

#define CLIENT_PACKET_SIZE 512

typedef struct ClientNet_type *ClientNet;


ClientNet ClientNet_Init(const char *serverIP, Uint16 port);
int ClientNet_SendJoinRequest(ClientNet client);
int ClientNet_TryReceive(ClientNet client);
void ClientNet_Destroy(ClientNet client);


#endif



