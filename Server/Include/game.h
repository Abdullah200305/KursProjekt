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
void Game_Update(Server server, Game *game);
void server_disconnet(Server server);
void Server_Broadcast(Server server,void *packet,size_t packetSize);

void movePlayerWithOther(Player player, int p_index, Player players[], int count, Bomb bomb);
void movePlayer(Map map, Player p);
#endif //GAME_H