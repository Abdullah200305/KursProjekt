#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <SDL2/SDL_net.h>

#define CLIENT_PACKET_SIZE 512

typedef struct ClientNet {
    int connected;
    int clientId;
    UDPsocket socket;
    IPaddress serverAddress;
    UDPpacket *sendPacket;
    UDPpacket *recvPacket;
} ClientNet;

typedef enum {
    PACKET_JOIN_REQUEST = 1,
    PACKET_JOIN_ACCEPT = 2
} PacketType;

typedef struct {
    int type;
} JoinRequestPacket;

typedef struct {
    int type;
    int clientId;
} JoinAcceptPacket;

int ClientNet_Init(ClientNet *client, const char *serverIP, Uint16 port);
int ClientNet_SendJoinRequest(ClientNet *client);
int ClientNet_TryReceive(ClientNet *client);
void ClientNet_Destroy(ClientNet *client);


#endif