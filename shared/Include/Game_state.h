#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "Map.h"
#include "Player.h"
#include "bombRelated.h"
#include "Ability.h"



typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
} GameState;

#define Max_Players 4

typedef struct {
    GameState state;
    Map map;
    Player players[Max_Players];
    AbilitySystem *abilitySystem;
    int numPlayers;
    Bomb bomb;
} Game;
#endif // GAME_STATE_H