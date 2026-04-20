#include "client_net.h"
#include <stdio.h>
#include <string.h>

int ClientNet_Init(ClientNet *client, const char *serverIP, Uint16 port)
{
    if (client == NULL) {
        return -1;
    }

    client->connected = 0;
    client->clientId = -1;
    client->hasGameInit = 0;
    client->socket = NULL;
    client->sendPacket = NULL;
    client->recvPacket = NULL;

    if (SDLNet_Init() < 0) {
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        return -1;
    }

    client->socket = SDLNet_UDP_Open(0);
    if (client->socket == NULL) {
        printf("SDLNet_UDP_Open failed: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return -1;
    }

    if (SDLNet_ResolveHost(&client->serverAddress, serverIP, port) < 0) {
        printf("SDLNet_ResolveHost failed: %s\n", SDLNet_GetError());
        SDLNet_UDP_Close(client->socket);
        client->socket = NULL;
        SDLNet_Quit();
        return -1;
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
        return -1;
    }

    client->connected = 1;
    return 0;
}

int ClientNet_SendJoinRequest(ClientNet *client)
{
    JoinRequestPacket packet;

    if (client == NULL || client->socket == NULL || client->sendPacket == NULL) {
        return -1;
    }

    packet.type = PACKET_JOIN_REQUEST;

    client->sendPacket->address = client->serverAddress;
    memcpy(client->sendPacket->data, &packet, sizeof(packet));
    client->sendPacket->len = sizeof(packet);

    if (SDLNet_UDP_Send(client->socket, -1, client->sendPacket) == 0) {
        printf("ClientNet_SendJoinRequest failed: %s\n", SDLNet_GetError());
        return -1;
    }

    return 0;
}

int ClientNet_SendDisconnect(ClientNet *client)
{
    DisconnectPacket packet;

    if (client == NULL || client->socket == NULL || client->sendPacket == NULL) {
        return -1;
    }

    packet.type = PACKET_DISCONNECT;
    packet.clientId = client->clientId;

    client->sendPacket->address = client->serverAddress;
    memcpy(client->sendPacket->data, &packet, sizeof(packet));
    client->sendPacket->len = sizeof(packet);

    if (SDLNet_UDP_Send(client->socket, -1, client->sendPacket) == 0) {
        printf("ClientNet_SendDisconnect failed: %s\n", SDLNet_GetError());
        return -1;
    }

    printf("[CLIENT] DISCONNECT sent\n");
    return 0;
}


int ClientNet_TryReceive(ClientNet *client)
{
    int packetType;

    if (client == NULL || client->socket == NULL || client->recvPacket == NULL) {
        return -1;
    }

    if (SDLNet_UDP_Recv(client->socket, client->recvPacket) == 0) {
        return 0;
    }

    if (client->recvPacket->len < (int)sizeof(int)) {
        printf("[CLIENT] Received packet too small\n");
        return 1;
    }

    memcpy(&packetType, client->recvPacket->data, sizeof(int));



    if (packetType == PACKET_JOIN_ACCEPT) {
        JoinAcceptPacket packet;

        if (client->recvPacket->len < (int)sizeof(JoinAcceptPacket)) {
            printf("[CLIENT] JOIN_ACCEPT packet too small\n");
            return 1;
        }

        memcpy(&packet, client->recvPacket->data, sizeof(JoinAcceptPacket));
        client->clientId = packet.clientId;

        printf("[CLIENT] JOIN_ACCEPT received, clientId = %d\n", client->clientId);
        return 1;
    }

    if (packetType == PACKET_GAME_INIT) {
        GameInitPacket packet;

        if (client->recvPacket->len < (int)sizeof(GameInitPacket)) {
            printf("[CLIENT] GAME_INIT packet too small\n");
            return 1;
        }

        memcpy(&packet, client->recvPacket->data, sizeof(GameInitPacket));

        client->gameInitPacket = packet;
        client->hasGameInit = 1;

        printf("[CLIENT] GAME_INIT received\n");
       
        printf("[CLIENT] mapId = %d\n",  packet.data.map.mapId);
        printf("[CLIENT] numPlayers = %d\n", packet.data.numPlayers);
        printf("[CLIENT] yourClientId = %d\n", packet.data.yourClientId);
        printf("[CLIENT] bombCarrier = %d\n", packet.data.bomb.bombCarrier);

        for (int i = 0; i < packet.data.numPlayers && i < MAX_PLAYERS; i++) {
            printf("[CLIENT] player %d -> x=%.1f y=%.1f lives=%d alive=%d\n",
                   i,
                   packet.data.players[i].x,
                   packet.data.players[i].y,
                   packet.data.players[i].lives,
                   packet.data.players[i].alive);
        }

        return 1;
    }

    printf("[CLIENT] Received unknown packet type: %d\n", packetType);
    return 1;
}
void ClientNet_Destroy(ClientNet *client)
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
    client->clientId = -1;
    SDLNet_Quit();
}
