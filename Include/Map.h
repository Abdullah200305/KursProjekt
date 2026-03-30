#ifndef GAME_H
#define GAME_H
#include <stdio.h>
#include <stdlib.h>


#define WIDTH 1280
#define HEIGHT 600
// #define WIDTH 1280
// #define HEIGHT 720
#define TILE_SIZE 32
#define TIlE_COUNT_X (WIDTH / TILE_SIZE)
#define TILE_COUNT_Y (HEIGHT / TILE_SIZE)





typedef struct{
    int running;
    int width;
    int height;
    int tileSize;
    int mapBuffer[TILE_COUNT_Y][TIlE_COUNT_X];
} Map;

Map* Map_create();
void Map_destroy(Map* map);




#endif // GAME_H