#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum {
    ITEM_NONE,
    ITEM_SPEEDRUN,
    ITEM_FREEZE,
    ITEM_DANCE
} ItemType;

typedef struct {
    float x;
    float y;
    int width;
    int height;
    ItemType type;
    bool active;
    Uint32 effectEndTime;
} Item;

#endif