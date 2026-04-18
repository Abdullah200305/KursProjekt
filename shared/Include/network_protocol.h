#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H
#define MAX_PLAYERS 4


typedef enum {
    PACKET_JOIN_REQUEST = 1,
    PACKET_JOIN_ACCEPT = 2
    PACKET_GAME_INIT = 3
} PacketType;

typedef struct {
    int type;
} JoinRequestPacket;

typedef struct {
    int type;
    int clientId;
} JoinAcceptPacket;

typedef struct{
    float x;
    float y;
    int active
    int exploding;
    int bombCarrier;
    int timer;
} BombNetData;

typedef struct{
    float x;
    float y;
    int lives;
    int alive;

} PlayerNetData

typedef struct {
    int width;
    int height;
} MapNetData;

typedef struct {
    int type;
    int numPlayers;
    int clientId;
    MapNetData map;
    PlayerNetData players[MAX_PLAYERS];
    BombNetData bomb;
} GameInitPacket;


#endif