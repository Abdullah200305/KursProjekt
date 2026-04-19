#include "client_net.h"
#include <stdio.h>
#include <string.h>

struct ClientNet_type{
    int connected;
    int clientId;
    UDPsocket socket;
    IPaddress serverAddress;
    UDPpacket *sendPacket;
    UDPpacket *recvPacket;
};

ClientNet ClientNet_Init(const char *serverIP, int  port)
{
    ClientNet client = malloc(sizeof(struct ClientNet_type));
    if (client == NULL) {
        return NULL;
    }

    client->connected = 0;
    client->clientId = -1;
    client->socket = NULL;
    client->sendPacket = NULL;
    client->recvPacket = NULL;

    if (SDLNet_Init() < 0) {
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        free(client);
        return NULL;
    }

    client->socket = SDLNet_UDP_Open(0);
    if (client->socket == NULL) {
        printf("SDLNet_UDP_Open failed: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        free(client);
        return NULL;
    }

    if (SDLNet_ResolveHost(&client->serverAddress, serverIP, port) < 0) {
        printf("SDLNet_ResolveHost failed: %s\n", SDLNet_GetError());
        SDLNet_UDP_Close(client->socket);
        client->socket = NULL;
        SDLNet_Quit();
        free(client);
        return NULL;
    }

    client->sendPacket = SDLNet_AllocPacket(CLIENT_PACKET_SIZE);
    client->recvPacket = SDLNet_AllocPacket(CLIENT_PACKET_SIZE);

    if (client->sendPacket == NULL || client->recvPacket == NULL) {
        printf("SDLNet_AllocPacket failed: %s\n", SDLNet_GetError());

        if (client->sendPacket != NULL) {
            SDLNet_FreePacket(client->sendPacket);
            client->sendPacket = NULL;
        }

        if (client->recvPacket != NULL) {
            SDLNet_FreePacket(client->recvPacket);
            client->recvPacket = NULL;
        }

        SDLNet_UDP_Close(client->socket);
        client->socket = NULL;
        SDLNet_Quit();
        free(client);
        return NULL;
    }

    client->connected = 1;
    return client;
}



int ClientNet_SendJoinRequest(ClientNet client)
{
    Packet packet;

    if (client == NULL || client->socket == NULL || client->sendPacket == NULL) {
        return -1;
    }

    packet.type = PACKET_JOIN_REQUEST;

   
    memcpy(client->sendPacket->data, &packet, sizeof(Packet));
    client->sendPacket->len = sizeof(Packet);

   
    client->sendPacket->address = client->serverAddress;

    if (SDLNet_UDP_Send(client->socket, -1, client->sendPacket) == 0) {
        printf("ClientNet_SendJoinRequest failed: %s\n", SDLNet_GetError());
        return -1;
    }

    return 0;
}

/// old 
// int ClientNet_SendJoinRequest(ClientNet client)
// {
//     Packet packet;

//     if (client == NULL || client->socket == NULL || client->sendPacket == NULL) {
//         return -1;
//     }
  
//     packet.type = PACKET_JOIN_REQUEST;
   
//     client->sendPacket->address = client->serverAddress;
//     memcpy(client->sendPacket->data, &packet, sizeof(packet));
//     client->sendPacket->len = sizeof(packet);

//     if (SDLNet_UDP_Send(client->socket, -1, client->sendPacket) == 0) {
//         printf("ClientNet_SendJoinRequest failed: %s\n", SDLNet_GetError());
//         return -1;
//     }
//     return 0;
// }

// int ClientNet_TryReceive(ClientNet client)
// {
//     int packetType;

//     if (client == NULL || client->socket == NULL || client->recvPacket == NULL) {
//         return -1;
//     }

//     if (SDLNet_UDP_Recv(client->socket, client->recvPacket) == 0) {
//         return 0;
//     }

//     if (client->recvPacket->len < (int)sizeof(int)) {
//         printf("[CLIENT] Received packet too small\n");
//         return 1;
//     }

//     memcpy(&packetType, client->recvPacket->data, sizeof(int));

//     if (packetType == PACKET_JOIN_ACCEPT) {
//         JoinAcceptPacket packet;

//         if (client->recvPacket->len < (int)sizeof(JoinAcceptPacket)) {
//             printf("[CLIENT] JOIN_ACCEPT packet too small\n");
//             return 1;
//         }

//         memcpy(&packet, client->recvPacket->data, sizeof(JoinAcceptPacket));
//         client->clientId = packet.clientId;

//         printf("[CLIENT] JOIN_ACCEPT received, clientId = %d\n", client->clientId);
//         return 1;
//     }

//     printf("[CLIENT] Received unknown packet type: %d\n", packetType);
//     return 1;
// }




int ClientNet_TryReceive(ClientNet client,Packet *packet)
{
    
    if (client == NULL || client->socket == NULL || client->recvPacket == NULL) {
        return -1;
    }
    int result = SDLNet_UDP_Recv(client->socket, client->recvPacket);
    if (result <= 0) {
        return 0;
    }
    if (client->recvPacket->len < sizeof(Packet)) {
        printf("[CLIENT] Packet too small\n");
        return 0;
    }

    memcpy(packet, client->recvPacket->data, sizeof(Packet));

    if (packet->type == PACKET_JOIN_ACCEPT) {
        client->clientId = packet->playerId;
        printf("[CLIENT] JOIN_ACCEPT received, clientId = %d /// willcome %d\n", client->clientId,packet->data.joinAccept);
        return 1;
    }

    return 0;
}





























void ClientNet_Destroy(ClientNet client)
{
    if (client == NULL) {
        return;
    }

    if (client->sendPacket != NULL) {
        SDLNet_FreePacket(client->sendPacket);
        client->sendPacket = NULL;
    }

    if (client->recvPacket != NULL) {
        SDLNet_FreePacket(client->recvPacket);
        client->recvPacket = NULL;
    }

    if (client->socket != NULL) {
        SDLNet_UDP_Close(client->socket);
        client->socket = NULL;
    }

    client->connected = 0;
    SDLNet_Quit();
}