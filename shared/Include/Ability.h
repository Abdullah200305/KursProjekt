#ifndef ABILITY_H
#define ABILITY_H

#include <stdbool.h>
#include "map.h"

#define MAX_ABILITIES 10

typedef enum
{
    ABILITY_NONE,
    ABILITY_SPEED,
    ABILITY_FREEZE,
    ABILITY_SWAP,
    ABILITY_SIZEUP,
    ABILITY_SHIELD

} AbilityType;

typedef struct AbilitySystem_type AbilitySystem;

AbilitySystem* AbilitySystem_create();
void AbilitySystem_destroy(AbilitySystem *system);

void AbilitySystem_init(AbilitySystem *system);
void abilitySpawnRate(AbilitySystem *system, Map map, int miliseconds);
void AbilitySystem_spawn(AbilitySystem *system, Map map);

void AbilitySystem_checkPickup(AbilitySystem *system, Player player, Player players[], int totalPlayers);


void SetAbilityItemState(AbilitySystem *system, int index, AbilityType type, float x, float y, int width, int height, bool active);

AbilityType AbilityItem_getType(AbilitySystem *system, int index);   
float       AbilityItem_getX(AbilitySystem *system, int index);      
float       AbilityItem_getY(AbilitySystem *system, int index);      
int         AbilityItem_getWidth(AbilitySystem *system, int index);  
int         AbilityItem_getHeight(AbilitySystem *system, int index); 
bool        AbilityItem_isActive(AbilitySystem *system, int index);  

#endif //ABILITY_H






