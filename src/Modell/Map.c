#include "Map.h"

Map *Map_create(int width, int height)
{
    Map *map = (Map *)malloc(sizeof(Map));
    FILE *ReadMap;
    map->running = 1;
    map->width = width;
    map->height = height;
     
    map->tileSize = 16;
    ReadMap = fopen("link/Test.txt", "r");
    if (ReadMap)
    {
        int item;
        for (int y = 0; y < TILE_COUNT_Y; y++)
        {
            for (int x = 0; x < TIlE_COUNT_X; x++)
            {
                if (fscanf(ReadMap, "%d%*c", &item) == 1)
                {
                    map->mapBuffer[y][x] = item;
                }
            }
        }
        fclose(ReadMap);
    }
    else
    {
        fprintf(stderr, "Failed to open map file.\n");
        free(map);
        return NULL;
    }

    return map;
}


void Map_destroy(Map *map)
{
    free(map);
}



int Player_collisionWithOtherPlayer(int x1, int y1, int x2, int y2) {
    // Simple AABB 
  return (
        x1 < x2 + 32 &&
        x1 + 32 > x2 &&
        y1 < y2 + 32 &&
        y1 + 32 > y2
    );
}


int Collision_Map(Map *map, float x, float y)
{
    float left   = x;
    float right  = x + 32 - 1; // update
    float top    = y;
    float bottom = y + 32 - 1; // updatw

    int tileLeft   = (int)(left / map->tileSize);
    int tileRight  = (int)(right / map->tileSize);
    int tileTop    = (int)(top / map->tileSize);
    int tileBottom = (int)(bottom / map->tileSize);
    
    int A1 = checkCollision(map, tileLeft, tileTop);
    int A2 = checkCollision(map, tileRight, tileTop);
    int B3 = checkCollision(map, tileLeft, tileBottom);
    int B4 = checkCollision(map, tileRight, tileBottom);

    // Priority: STOP > SLOW > NONE
    if (A1 == 1 || A2 == 1 || B3 == 1 || B4 == 1)
        return 1;

    if (A1 == 3 || A2 == 3 || B3 == 3 || B4 == 3)
        return 3;

    return 0;
}

int checkCollision(Map *map, int x, int y) {
    switch (map->mapBuffer[y][x])
    {
    case 1:    
    case 2:
        return 1;
    case 3:
        return 3;
    default:
        return 0;
    }
   
    // if (map->mapBuffer[y][x] == 2) {
    //     printf("Collision with tile type 2 at (%d, %d)\n", x, y);
    //     return 2; 
    // }

    // if (map->mapBuffer[y][x] == 3) {
    //     printf("Collision with tile type 3 at (%d, %d)\n", x, y);
    //     return 1; 
    // }
    // return 0;
}