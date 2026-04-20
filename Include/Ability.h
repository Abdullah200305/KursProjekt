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
    ABILITY_SLOW,
    ABILITY_SHIELD,
    ABILITY_SWAP,
    
} AbilityType;

typedef struct AbilitySystem AbilitySystem;

AbilitySystem* AbilitySystem_create();
void AbilitySystem_destroy(AbilitySystem *system);

void AbilitySystem_init(AbilitySystem *system);
void AbilitySystem_spawn(AbilitySystem *system, Map map, AbilityType type);
void AbilitySystem_render(const AbilitySystem *system, Renderer *r);
void AbilitySystem_checkPickup(AbilitySystem *system, Player player);
void AbilitySystem_checkFreezePickup(AbilitySystem *system, Player picker, Player players[], int count);

#endif



