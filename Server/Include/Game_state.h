#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "Map.h"
#include "Player.h"

#define MAX_PLAYERS 4

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
} GameState;

typedef struct {
    GameState state;
    Map map;
    Player players[MAX_PLAYERS];
    int numPlayers;
    //Bomb bomb;
} Game;

// void game_state_init(Game *game);
// void game_state_update(Game *game);
// void game_state_change(Game *game, GameState new_state);

#endif // GAME_STATE_H