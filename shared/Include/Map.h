#ifndef MAP_H
#define MAP_H
#include <stdio.h>
#include <stdlib.h>
#include "Player.h"

#define WIDTH 1280
#define HEIGHT 720
#define TILE_SIZE 16
#define TILE_COUNT_X (WIDTH / TILE_SIZE)
#define TILE_COUNT_Y (HEIGHT / TILE_SIZE)




typedef struct Map_type *Map;

//Olika kartor
typedef enum {
    MAP_DESERT = 1,
    MAP_FOREST = 2,
    MAP_ICELAND = 3,
    MAP_GRAVEYARD = 4,
    MAP_LAVA = 5,
    MAP_PIRATE = 6
} MapId;

// geter and setter
int getTileSize(Map map);
int getHeight(Map map);
int getWidth(Map map);
int getMapBufferItems(Map map,int x,int y);
 


Map Map_create(int width, int height);
void Map_destroy(Map map);


// Random map
MapId getRandomMapId(void);
const char *getMapImagePath(MapId mapId);

int Player_collisionWithOtherPlayer( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

int Collision_Map(Map map, float x, float y, float width, float height);
int checkCollision(Map map, int x, int y);
void resolveCollision(Map map, Player player);
void resolveCollisionRate(Map map, Player player, int miliseconds);


#endif // MAP_H





