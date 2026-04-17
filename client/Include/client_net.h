#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <SDL2/SDL_net.h>

typedef struckt{
    int connected;
    UPDsocket socket;
    IPaddress serverAddress;
    UPDpacket *sendPacket;
    UPDpacket *recvPacket;


} ClientNet;

int ClientNet_Init(ClientNet *client, const char *serverIP, Uint16 port);
void ClientNet_Destroy(ClientNet *client);


#endif