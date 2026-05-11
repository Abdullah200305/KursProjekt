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
    GAME_STATE_GAME_OVER,
    GAME_STATE_CHOOSE_HOST_JOIN,
    GAME_STATE_HOST_SETUP,
    GAME_STATE_CLIENT_SETUP,
    GAME_STATE_COUNTDOWN
} GameState;

#define Max_Players 4

typedef struct {
    GameState state;
    int running;
    int countdownValue;
    Map map;
    Player players[Max_Players];
    AbilitySystem *abilitySystem;
    int numPlayers;
    int playerActive[Max_Players];
    Bomb bomb;
    SoundSystem sound;
} Game;
#endif // GAME_STATE_H












