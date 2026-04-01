#ifndef GAME_CONTROLL_H
#define GAME_CONTROLL_H

#include "Game_state.h"
#include "Renderer.h"

void game_init(Game* game,Renderer* renderer);
void game_update(Game* game, Renderer* renderer);
void game_loop(Game* game, Renderer* renderer);
void game_cleanup(Game* game, Renderer* renderer);



#endif // GAME_CONTROLL_H