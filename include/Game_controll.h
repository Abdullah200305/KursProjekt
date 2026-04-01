#ifndef GAME_CONTROLL_H
#define GAME_CONTROLL_H

#include "Game_state.h"
#include "Renderer.h"

void game_init(Game* game,Renderer* renderer);
void game_update(Game* game, Renderer* renderer);
void game_loop(Game* game, Renderer* renderer);
void game_cleanup(Game* game, Renderer* renderer);



// logic for the game will be here, such as collision, player movement, etc.
void movePlayerWithOther(Player* p, Player* players, int numPlayers);
void movePlayer(Map *map, Player *p);

#endif // GAME_CONTROLL_H