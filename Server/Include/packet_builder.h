#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H


#include "network_protocol.h"
#include "Game_state.h"



// void Packet_BuildGameStart(Game *game,int id,Packet *packet);
// void Packet_BuildGameAccept(int id,Packet *packet);

//### Abid version 


void Packet_BuildGameStart(Game *game,int id,GameInitPacket *packet);
void Packet_BuildGameAccept(int id,JoinAcceptPacket *packet);
void Packet_BuildDisconnect(int Server_id,DisconnectPacket *packet);

#endif //PACKET_BUILDER_H