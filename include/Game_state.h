#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "Map.h"
#include "Player.h"
#include "bombRelated.h"

typedef struct AbilitySystem AbilitySystem;

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










// will update

// typedef enum GameState_type *GameState; 
// typedef struct Game_type *Game;
// enum GameState_type{
//     GAME_STATE_MENU,
//     GAME_STATE_PLAYING,
//     GAME_STATE_PAUSED,
//     GAME_STATE_GAME_OVER
// };
// #define Max_Players 4

// struct Game_type{
//     GameState state;
//     Map map;
//     Player players[Max_Players];
//     int numPlayers;
//     Bomb bomb;
// };





#endif // GAME_STATE_H