#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <SDL2/SDL_stdinc.h>
#include "network_protocol.h"

typedef struct ClientNet_type *ClientNet;

ClientNet ClientNet_Init(const char *serverIP, Uint16 port);
void ClientNet_Destroy(ClientNet client);

int ClientNet_SendJoinRequest(ClientNet client);
int ClientNet_SendDisconnect(ClientNet client);
int ClientNet_TryReceive(ClientNet client);

int ClientNet_HasGameInit(ClientNet client);
GameInitPacket ClientNet_GetGameInitPacket(ClientNet client);
void ClientNet_ClearGameInit(ClientNet client);

int ClientNet_GetClientId(ClientNet client);
void ClientNet_SetClientId(ClientNet client, int clientId);

#endif