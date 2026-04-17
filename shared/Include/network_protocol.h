#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

typedef enum {
    PACKET_JOIN_REQUEST = 1,
    PACKET_JOIN_ACCEPT = 2
} PacketType;

typedef struct {
    int type;
} JoinRequestPacket;

typedef struct {
    int type;
    int clientId;
} JoinAcceptPacket;

#endif