#ifndef CLIENT_NET_H
#define CLIENT_NET_H
#include <SDL2/SDL_net.h>

#include "Input.h"
#include "Game_state.h"


typedef struct {
    int id;
    int connected;

    UDPsocket socket;
    IPaddress serverIP;

    UDPpacket *sendPacket;
    UDPpacket *recvPacket;

    Player *player;
    InputState *input;

} Client;



Client* client_net_init();
int ClientConnection(Client *client, const char *serverIP, int port);


void Destroy_Client(Client *client);
void Client_Send(Client *client, void *data, int size);
//void Send(Player* Player);
// int Recv(Player* player);
#endif // CLIENT_NET_H