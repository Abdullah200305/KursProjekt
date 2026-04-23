// #include "ability.h"
// #include <stdlib.h>

// // Intern struct
// typedef struct {
//     AbilityType type;
//     float x, y;
//     int width, height;
//     bool active;
// } AbilityItem;

// struct AbilitySystem
// {
//     AbilityItem items [MAX_ABILITIES];
// };



// AbilitySystem* AbilitySystem_create()
// {
//     AbilitySystem *system = malloc(sizeof(AbilitySystem));
//     return system;
// }

// void AbilitySystem_destroy(AbilitySystem *system)
// {
//     free(system);
// }


// void AbilitySystem_init (AbilitySystem *system)
// {
//     for (int i = 0; i < MAX_ABILITIES; i++)
//     {
//         system->items[i].active = false;
//         system->items[i].type = ABILITY_NONE;
//     }
// }

// void AbilitySystem_spawn(AbilitySystem *system, int mapWidth, int mapHeight)
// {
//     for (int i = 0; i < MAX_ABILITIES; i++)
//     {
//         if (!system->items[i].active)
//         {
//             system->items[i].active = true;
//             system->items[i].type = (rand() % 5) + 1;
//             system->items[i].width = 32;
//             system->items[i].height = 32;
//             system->items[i].x = rand() % (mapWidth - system->items[i].width);
//             system->items[i].y = rand() % (mapHeight - system->items[i].height);
            
//             break;
//         }
//     }
// }

// void AbilitySystem_render(const AbilitySystem *system, SDL_Renderer *renderer)
// {
//     SDL_Rect rect;

//     for (int i = 0; i < MAX_ABILITIES; i++)
//     {
//         if (system->items[i].active)
//         {
//             rect.x = (int)system->items[i].x;
//             rect.y = (int)system->items[i].y;
//             rect.w = system->items[i].width;
//             rect.h = system->items[i].height;

//             SDL_RenderFillRect(renderer, &rect);
//         }
//     }
// }