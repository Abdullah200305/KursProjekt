#ifndef GAME_STATE_H
#define GAME_STATE_H



#include "Map.h"
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
} GameState;


typedef struct {
    GameState state;
    Map* map;
} Game;


#endif // GAME_STATE_H