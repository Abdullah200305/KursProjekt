#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <SDL2/SDL_net.h>
#include "network_protocol.h"

#define CLIENT_PACKET_SIZE 512

typedef struct ClientNet {
    int connected;
    int clientId;
    int hasGameInit;
    GameInitPacket gameInitPacket;
    UDPsocket socket;
    IPaddress serverAddress;
    UDPpacket *sendPacket;
    UDPpacket *recvPacket;
} ClientNet;


int ClientNet_Init(ClientNet *client, const char *serverIP, Uint16 port);
int ClientNet_SendJoinRequest(ClientNet *client);
int ClientNet_TryReceive(ClientNet *client);
void ClientNet_Destroy(ClientNet *client);


#endif