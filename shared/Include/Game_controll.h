#ifndef GAME_CONTROLL_H
#define GAME_CONTROLL_H

#include "Game_state.h"
#include "Renderer.h"
#include "Input.h"
#include "client_net.h"



void game_init(Game *game, Renderer *renderer,ClientNet clientNet);
void game_update(Game* game, Renderer* renderer);
void game_loop(Game* game, Renderer* renderer,InputState event);
void game_cleanup(Game *game, Renderer *renderer,InputState event);


// input handling for player movement and actions
////void  Player_handleInput(Game *game,InputState *input);

// logic for the game will be here, such as collision, player movement, etc.
//void movePlayerWithOther(Player p, Player players[], int count, Bomb *bomb);

/////void movePlayerWithOther(Player player, int p_index, Player players[], int count, Bomb bomb);
/////void movePlayer(Map map, Player p);

#endif // GAME_CONTROLL_H