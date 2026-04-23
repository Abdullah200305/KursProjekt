#ifndef GAME_CONTROLL_H
#define GAME_CONTROLL_H
// maybe will update 
#include "Game_state.h"
#include "client_net.h"
#include "Renderer.h"
#include "Input.h"
#include "bombRelated.h"
#include "client_net.h"
#include <stdio.h>
#include <string.h>

void game_loop(Game *game, Renderer *renderer, ClientNet clientNet);
void game_init(Game* game,Renderer* renderer,ClientNet clientNet);
void game_update(Game* game, Renderer* renderer);
void game_cleanup(Game* game, Renderer* renderer);



// network for depacket stuff 
int game_apply_network_init(Game *game, ClientNet clientNet);
void game_apply_network_state(Game *game, ClientNet clientNet);



#endif // GAME_CONTROLL_H