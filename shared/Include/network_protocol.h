#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#define MAX_PLAYERS 4
#define MAP_ID_ISLAND 1

typedef enum {
    PACKET_JOIN_REQUEST = 1,
    PACKET_JOIN_ACCEPT = 2,
    PACKET_GAME_INIT = 3,
    PACKET_DISCONNECT = 4
} PacketType;

typedef struct {
    int type;
} JoinRequestPacket;

typedef struct {
    int type;
    int clientId;
} JoinAcceptPacket;

typedef struct {
    int type;
    int clientId;
} DisconnectPacket;


typedef struct {
    int type;
} HeaderPacket;


// abid old version
// typedef struct {
//     float x;
//     float y;
//     int lives;
//     int alive;
// } PlayerInitData;


// add by abody
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
}PlayerInitData;



typedef struct {
    float x;
    float y;
    int active;
    int exploding;
    int bombCarrier;
    int timer;
} BombInitData;

// add by abody
typedef struct 
{
    int mapId;
    int height;
    int width;
}MapInitData;

typedef struct {
    int numPlayers;
    int yourClientId;
    PlayerInitData players[MAX_PLAYERS];
    BombInitData bomb;
    MapInitData map;
} GameInitData;

typedef struct {
    int type;
    GameInitData data;
} GameInitPacket;

#endif