#ifndef GAME_H
#define GAME_H
#include "server_net.h"
#include "Game_state.h"
#include "Player_manager.h"
#include "server_handlar.h"
#include "packet_builder.h"
void run(Server server,Game game);
void Game_Init(Server server,Game *game);
void Game_InitSendToClients(Server server,Game *game);


void Server_Broadcast(Server server,Packet *packet);
#endif //GAME_H