#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

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


typedef struct 
{
    int height;
    int width;
}DataMap;


typedef struct
{
    float x;
    float y;
    float vx;   
    float vy;   
    float width;
    float height;
    int lives;  
    int alive;  
    int id;
}DataPlayer;







typedef struct {
    PacketType type;
    int playerId;
    union {
        JoinAccept joinAccept;
        JoinRequest joinRequest;
        DataPlayer player[4];
        DataMap map;
        //Bomb bomb;
        InputState input;
    } data;
} Packet;

#endif


