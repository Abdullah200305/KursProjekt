#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <SDL2/SDL_stdinc.h>
#include "network_protocol.h"

typedef struct ClientNet_type *ClientNet;

ClientNet ClientNet_Init(const char *serverIP, Uint16 port);
void ClientNet_Destroy(ClientNet client);

int ClientNet_SendJoinRequest(ClientNet client);
int ClientNet_SendDisconnect(ClientNet client);
int ClientNet_SendInput(ClientNet client, const InputPacket *packet);
int ClientNet_TryReceive(ClientNet client);

int ClientNet_HasGameInit(ClientNet client);
GameInitPacket ClientNet_GetGameInitPacket(ClientNet client);
void ClientNet_ClearGameInit(ClientNet client);

int ClientNet_HasGameState(ClientNet client);
GameStatePacket ClientNet_GetGameStatePacket(ClientNet client);
void ClientNet_ClearGameState(ClientNet client);

int ClientNet_GetDisconnectedPlayerId(ClientNet client);
void ClientNet_ClearDisconnectedPlayerId(ClientNet client);


int ClientNet_HasGameStart(ClientNet client);
int ClientNet_hasCountdown(ClientNet client);



int ClientNet_getConutDown(ClientNet client);

int ClientNet_GetClientId(ClientNet client);
void ClientNet_SetClientId(ClientNet client, int clientId);


void get_local_ip(char *buffer);
int ClientNet_SendStartGame(ClientNet client);

#endif