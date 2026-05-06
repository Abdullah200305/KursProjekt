#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#define MAX_PLAYERS 4
#define MAP_ID_ISLAND 1
#define MAX_ABILITIES 10


#include <SDL2/SDL_stdinc.h>
typedef enum {
    PACKET_JOIN_REQUEST = 1,
    PACKET_JOIN_ACCEPT = 2,
    PACKET_GAME_INIT = 3,
    PACKET_DISCONNECT = 4,
    PACKET_INPUT = 5,
    PACKET_GAME_STATE = 6,
    PACKET_START_GAME = 7
} PacketType;

typedef struct {
    Uint8 type;
} JoinRequestPacket;

typedef struct {
    Uint8 type;
} StartGamePacket;

typedef struct {
    Uint8 type;
    int clientId;
} JoinAcceptPacket;

typedef struct {
    Uint8 type;
    int clientId;
} DisconnectPacket;


typedef struct {
    Uint8 type;
} HeaderPacket;


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
    float freezeTimer;  
}PlayerInitData;



typedef struct {
    float x;
    float y;
    int active;
    int exploding;
    int bombCarrier;
    int timer;
} BombInitData;

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
    Uint8 type;
    GameInitData data;
} GameInitPacket;


typedef struct {
    int   active;
    int   type;      
    float x;
    float y;
    int   width;
    int   height;
} AbilityNetData;

typedef struct {
    int    numAbilities;         
    AbilityNetData items[10];
} AbilityStateData;


typedef struct {
    int numPlayers;
    PlayerInitData players[MAX_PLAYERS];
    BombInitData bomb;
    AbilityStateData abilities;
} GameStateData;

typedef struct {
    Uint8 type;
    GameStateData data;
} GameStatePacket;

typedef struct {
    Uint8 type;
    int clientId;
    int up;
    int down;
    int left;
    int right;
} InputPacket;


#endif








