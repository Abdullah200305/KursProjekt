#ifndef ABILITY_H
#define ABILITY_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "map.h"

#define MAX_ABILITIES 10

typedef enum
{
    ABILITY_NONE,
    ABILITY_SPEED,
    ABILITY_FREEZE,
    ABILITY_SWAP,
    ABILITY_SIZEUP,
    ABILITY_SHIELD,
    
    
} AbilityType;

typedef struct AbilitySystem AbilitySystem;

AbilitySystem* AbilitySystem_create();
void AbilitySystem_destroy(AbilitySystem *system);

void AbilitySystem_init(AbilitySystem *system);
void abilitySpawnRate(AbilitySystem *system, Map map, int miliseconds);
void AbilitySystem_spawn(AbilitySystem *system, Map map);
void AbilitySystem_render(const AbilitySystem *system, Renderer *r);
void AbilitySystem_checkPickup(AbilitySystem *system, Player player, Player players[], int totalPlayers);
#endif



