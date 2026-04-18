#include "ability.h"

// Intern struct
typedef struct {
    AbilityType type;
    float x, y;
    int width, height;
    bool active;
} AbilityItem;

struct AbilitySystem
{
    AbilityItem items[MAX_ABILITIES];
};



AbilitySystem* AbilitySystem_create()
{
    AbilitySystem *system = malloc(sizeof(AbilitySystem));
    return system;
}

void AbilitySystem_destroy(AbilitySystem *system)
{
    free(system);
}


void AbilitySystem_init (AbilitySystem *system)
{
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        system->items[i].active = false;
        system->items[i].type = ABILITY_NONE;
    }
}

void AbilitySystem_spawn(AbilitySystem *system, Map map)
{
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        if (!system->items[i].active)
        {
            int x, y;
            int tile;

            do {
                x = rand() % TIlE_COUNT_X;
                y = rand() % TILE_COUNT_Y;

                tile = getMapBufferItems(map, x, y);

            } while (!(tile == 0 || tile == 3));

            int tileSize = getTileSize(map);

            system->items[i].active = true;
            system->items[i].type = 1; // initalized to always spawn the speed boost for now, when more abilties created do rand() % 6 

            system->items[i].width = 32;
            system->items[i].height = 32;

            system->items[i].x = x * tileSize;
            system->items[i].y = y * tileSize;

            break;
        }
    }
}

void AbilitySystem_render(const AbilitySystem *system, Renderer *r)
{
    SDL_Rect rect;

    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        if (!system->items[i].active)
            continue;

        rect.x = (int)system->items[i].x;
        rect.y = (int)system->items[i].y;
        rect.w = system->items[i].width;
        rect.h = system->items[i].height;

        SDL_Texture *tex = r->abilityTextures[system->items[i].type];

        if (tex)
            SDL_RenderCopy(r->sdlRenderer, tex, NULL, &rect);
    }
}

void AbilitySystem_checkPickup(AbilitySystem *system, Player player)
{
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        AbilityItem *a = &system->items[i];

        if (!a->active)
            continue;

        if (Player_collisionWithOtherPlayer(
                getPlayerX(player),
                getPlayerY(player),
                a->x,
                a->y))
        {
            // remove ability
            a->active = false;

            // apply effect
            if (a->type == ABILITY_SPEED)
            {
                setPlayerSpeedYX(player, 10, 10);
                setPlayerSpeedTimer(player, 300);
            }
        }
    }
}