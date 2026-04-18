#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#define MAX_PLAYERS 4
#define MAP_ID_ISLAND 1

typedef enum {
    PACKET_JOIN_REQUEST = 1,
    PACKET_JOIN_ACCEPT = 2,
    PACKET_GAME_INIT = 3
} PacketType;

typedef struct {
    int type;
} JoinRequestPacket;

typedef struct {
    int type;
    int clientId;
} JoinAcceptPacket;

typedef struct {
    float x;
    float y;
    int lives;
    int alive;
} PlayerInitData;

typedef struct {
    float x;
    float y;
    int active;
    int exploding;
    int bombCarrier;
    int timer;
} BombInitData;

typedef struct {
    int mapId;
    int numPlayers;
    int yourClientId;
    PlayerInitData players[MAX_PLAYERS];
    BombInitData bomb;
} GameInitData;

typedef struct {
    int type;
    GameInitData data;
} GameInitPacket;

#endif