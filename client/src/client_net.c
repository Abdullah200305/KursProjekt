#include "client_net.h"



Client* client_net_init()
{
    Client *client = (Client*)malloc(sizeof(Client));
    if (!client) {
        fprintf(stderr, "Failed to allocate memory for client\n");
        return NULL;
    }
    client->id = -1;
    client->connected = 0;
    client->socket = NULL;
    client->serverIP = (IPaddress){0};
    client->sendPacket = NULL;
    client->recvPacket = NULL;
    client->player = NULL;
    client->input = NULL;

    return client;
}

int ClientConnection(Client *client, const char *serverIP, int port) {
    if (SDLNet_Init() != 0) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        return -1;
    }
    client->socket = SDLNet_UDP_Open(0); 
    if (!client->socket) {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return -1;
    }

    if (SDLNet_ResolveHost(&client->serverIP, serverIP, port) != 0) {
        printf("SDLNet_ResolveHost error: %s\n", SDLNet_GetError());
        return -1;
    }

    client->recvPacket = SDLNet_AllocPacket(512);
    client->sendPacket = SDLNet_AllocPacket(512);

    return 0;
}





void Client_Send(Client *client, void *data, int size)
{
    memcpy(client->sendPacket->data, data, size);
    client->sendPacket->len = size;
    client->sendPacket->address = client->serverIP;
    SDLNet_UDP_Send(client->socket, -1, client->sendPacket);
}

int Server_Receive(Client *client) {
    if (SDLNet_UDP_Recv(client->socket, client->recvPacket)) {
        if (client->recvPacket->len >= sizeof(Packet)) {
            memcpy(&client->packet, client->recvPacket->data, sizeof(Packet));
            return 1;
        }
    }
    return 0;
}



void Destroy_Client(Client *client){
    SDLNet_FreePacket(client->sendPacket);
    SDLNet_FreePacket(client->recvPacket);
    SDLNet_UDP_Close(client->socket);
    free(client);
}