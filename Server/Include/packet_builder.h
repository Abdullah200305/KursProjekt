#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H
#include "protocol.h"
#include "Game_state.h"

void Packet_BuildGameStart(Game *game,int id,Packet *packet);
void Packet_BuildGameAccept(int id,Packet *packet);

#endif //PACKET_BUILDER_H