#ifndef ABILITY_H
#define ABILITY_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define MAX_ABILITIES 10

typedef enum
{
    ABILITY_NONE,
    ABILITY_SPEED,
    ABILITY_FREEZE,
    ABILITY_SLOW,
    ABILITY_SHIELD,
    ABILITY_SWAP,
} AbilityType;

typedef struct AbilitySystem AbilitySystem;

AbilitySystem* AbilitySystem_create();
void AbilitySystem_destroy(AbilitySystem *system);

void AbilitySystem_init(AbilitySystem *system);
void AbilitySystem_spawn(AbilitySystem *system, int mapWidth, int mapHeight);
void AbilitySystem_render(const AbilitySystem *system, SDL_Renderer *renderer);

#endif



