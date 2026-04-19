#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include "Game_state.h"
#include "Input.h"
typedef enum
{
    // server->client
    PACKET_MAP_INIT,
    PACKET_PLAYER_INIT,
    PACKET_BOMB_INIT,


   // client->server
    PACKET_JOIN_REQUEST,
    PACKET_CONNECT,

    // server->client
    PACKET_JOIN_ACCEPT,



    // client->server
    PACKET_INPUT,



    // out 
    PACKET_DISCONNECT
} PacketType;
typedef struct {
    char name[32];
    } JoinRequest;

typedef struct {
    int assignedId;
    } JoinAccept;









typedef struct {
    PacketType type;
    int playerId;
    union {
        JoinAccept joinAccept;
        JoinRequest joinRequest;
        Player player;
        Map *map;
        Bomb bomb;
        InputState input;
    } data;
} Packet;

#endif


