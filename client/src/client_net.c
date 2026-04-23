#include "client_net.h"
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIENT_PACKET_SIZE ((int)sizeof(GameInitPacket))

struct ClientNet_type {
    int connected;
    int clientId;
    int hasGameInit;
    GameInitPacket gameInitPacket;
    int hasGameState;
    GameStatePacket gameStatePacket;
    UDPsocket socket;
    IPaddress serverAddress;
    UDPpacket *sendPacket;
    UDPpacket *recvPacket;
};

ClientNet ClientNet_Init(const char *serverIP, Uint16 port)
{
    ClientNet client = malloc(sizeof(struct ClientNet_type));
    if (client == NULL) {
        return NULL;
    }

    client->connected = 0;
    client->clientId = -1;
    client->hasGameInit = 0;
    memset(&client->gameInitPacket, 0, sizeof(GameInitPacket));
    client->hasGameState = 0;
    memset(&client->gameStatePacket, 0, sizeof(GameStatePacket));
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
        }

        if (client->recvPacket != NULL) {
            SDLNet_FreePacket(client->recvPacket);
        }

        SDLNet_UDP_Close(client->socket);
        SDLNet_Quit();
        free(client);
        return NULL;
    }

    client->connected = 1;
    return client;
}

int ClientNet_SendJoinRequest(ClientNet client)
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

int ClientNet_SendDisconnect(ClientNet client)
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

int ClientNet_SendInput(ClientNet client, const InputPacket *packet)
{
    if (client == NULL || packet == NULL || client->socket == NULL || client->sendPacket == NULL) {
        return -1;
    }

    client->sendPacket->address = client->serverAddress;
    memcpy(client->sendPacket->data, packet, sizeof(InputPacket));
    client->sendPacket->len = sizeof(InputPacket);

    if (SDLNet_UDP_Send(client->socket, -1, client->sendPacket) == 0) {
        printf("ClientNet_SendInput failed: %s\n", SDLNet_GetError());
        return -1;
    }

    return 0;
}

int ClientNet_TryReceive(ClientNet client)
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
        printf("[CLIENT] mapId = %d\n", packet.data.map.mapId);
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
    if (packetType == PACKET_GAME_STATE) {
        GameStatePacket packet;

        if (client->recvPacket->len < (int)sizeof(GameStatePacket)) {
            printf("[CLIENT] GAME_STATE packet too small\n");
            return 1;
        }

        memcpy(&packet, client->recvPacket->data, sizeof(GameStatePacket));

        client->gameStatePacket = packet;
        client->hasGameState = 1;



        // abody stop 
       // printf("[CLIENT] GAME_STATE received\n");
       // printf("[CLIENT] numPlayers = %d\n", packet.data.numPlayers);

        return 1;
    }

    printf("[CLIENT] Received unknown packet type: %d\n", packetType);
    return 1;
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
    client->clientId = -1;
    SDLNet_Quit();
    free(client);
}

int ClientNet_HasGameInit(ClientNet client)
{
    if (client == NULL) {
        return 0;
    }

    return client->hasGameInit;
}

GameInitPacket ClientNet_GetGameInitPacket(ClientNet client)
{
    GameInitPacket packet;
    memset(&packet, 0, sizeof(GameInitPacket));

    if (client == NULL) {
        return packet;
    }

    return client->gameInitPacket;
}

void ClientNet_ClearGameInit(ClientNet client)
{
    if (client == NULL) {
        return;
    }

    client->hasGameInit = 0;
}

int ClientNet_GetClientId(ClientNet client)
{
    if (client == NULL) {
        return -1;
    }

    return client->clientId;
}

void ClientNet_SetClientId(ClientNet client, int clientId)
{
    if (client == NULL) {
        return;
    }

    client->clientId = clientId;
}


int ClientNet_HasGameState(ClientNet client)
{
    if (client == NULL) {
        return 0;
    }

    return client->hasGameState;
}

GameStatePacket ClientNet_GetGameStatePacket(ClientNet client)
{
    GameStatePacket packet;
    memset(&packet, 0, sizeof(GameStatePacket));

    if (client == NULL) {
        return packet;
    }

    return client->gameStatePacket;
}

void ClientNet_ClearGameState(ClientNet client)
{
    if (client == NULL) {
        return;
    }

    client->hasGameState = 0;
}





