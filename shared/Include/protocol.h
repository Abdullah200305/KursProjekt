// #ifndef PROTOCOL_H
// #define PROTOCOL_H

// #include <stdint.h>

// #include "Input.h"
// typedef enum
// {
//     // server->client
//     PACKET_GAME_INIT,
//     packet_gameuppdate,
  


//    // client->server
//     PACKET_JOIN_REQUEST,
//     PACKET_CONNECT,

//     // server->client
//     PACKET_JOIN_ACCEPT,



//     // client->server
//     PACKET_INPUT,



//     // out 
//     PACKET_DISCONNECT
// } PacketType;
// typedef struct {
//     char name[32];
//     } JoinRequest;

// typedef struct {
//     int assignedId;
//     } JoinAccept;


// typedef struct 
// {
//     int MapID;
//     int height;
//     int width;
// }DataMap;


// typedef struct
// {
//     float x;
//     float y;
//     float vx;   
//     float vy;   
//     float width;
//     float height;
//     int lives;  
//     int alive;  
//     int id;
// }DataPlayer;

// typedef struct 
// {
    
// }DataBomb;

// typedef struct{
// DataMap dataMap;
// DataPlayer dataPlayer[4];
// }DataGame;




// typedef struct {
//     PacketType type;
//     // add antal player
//     int playerId;
//     union {
//         JoinAccept joinAccept;
//         JoinRequest joinRequest;
//         DataGame Game;
//         InputState input;
//     } data;
// } Packet;

// #endif


