#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef enum
{
    PACKET_CONNECT,
    PACKET_JOIN_ACCEPT,
    PACKET_INPUT,
    PACKET_DISCONNECT
} PacketType;

typedef struct
{
    PacketType type;
    int playerId;
} Packet;

#endif



