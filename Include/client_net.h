#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <SDL2/SDL_net.h>

#define CLIENT_PACKET_SIZE 512

typedef struct ClientNet {
    int connected;
    UDPsocket socket;
    IPaddress serverAddress;
    UDPpacket *sendPacket;
    UDPpacket *recvPacket;
} ClientNet;

int ClientNet_Init(ClientNet *client, const char *serverIP, Uint16 port);
void ClientNet_Destroy(ClientNet *client);

#endif