// #include "Map.h"


// struct Map_type{
//     int running;
//     int width;
//     int height;
//     int tileSize;
//     int mapBuffer[TILE_COUNT_Y][TILE_COUNT_X];
// };


// int getTileSize(Map map) {
//     return map->tileSize;
// }

// int getHeight(Map map) {
//     return map->height;
// }

// int getWidth(Map map) {
//     return map->width;
// }

// int getMapBufferItems(Map map, int x, int y) {
//     return map->mapBuffer[y][x];
// }

// Map Map_create(int width, int height)
// {
//     Map map = malloc(sizeof(struct Map_type));
//     if (!map) return NULL;

//     map->running = 1;
//     map->width = width;
//     map->height = height;
//     map->tileSize = TILE_SIZE;

//     FILE *ReadMap = fopen("link/Test.txt", "r");
//     if (!ReadMap) {
//         fprintf(stderr, "Failed to open map file.\n");
//         free(map);
//         return NULL;
//     }

//     int item;
//     for (int y = 0; y < TILE_COUNT_Y; y++) {
//         for (int x = 0; x < TILE_COUNT_X; x++) {
//             if (fscanf(ReadMap, "%d%*c", &item) == 1) {
//                 map->mapBuffer[y][x] = item;
//             }
//         }
//     }
//     fclose(ReadMap);
//     return map;
// }

// void Map_destroy(Map map)
// {
//     free(map);
// }




// int Player_collisionWithOtherPlayer(int x1, int y1, int x2, int y2) {
//     // Simple AABB 
//   return (
//         x1 < x2 + 32 &&
//         x1 + 32 > x2 &&
//         y1 < y2 + 32 &&
//         y1 + 32 > y2
//     );
// }

// int Collision_Map(Map map, float x, float y)
// {
//     float left   = x;
//     float right  = x + 32 - 1; // update
//     float top    = y;
//     float bottom = y + 32 - 1; // updatw

//     int tileLeft   = (int)(left / map->tileSize);
//     int tileRight  = (int)(right / map->tileSize);
//     int tileTop    = (int)(top / map->tileSize);
//     int tileBottom = (int)(bottom / map->tileSize);
    
//     int A1 = checkCollision(map, tileLeft, tileTop);
//     int A2 = checkCollision(map, tileRight, tileTop);
//     int B3 = checkCollision(map, tileLeft, tileBottom);
//     int B4 = checkCollision(map, tileRight, tileBottom);

//     // Priority: STOP > SLOW > NONE
//     if (A1 == 1 || A2 == 1 || B3 == 1 || B4 == 1)
//         return 1;

//     if (A1 == 3 || A2 == 3 || B3 == 3 || B4 == 3)
//         return 3;

//     return 0;
// }

// int checkCollision(Map map, int x, int y) {
//     switch (map->mapBuffer[y][x])
//     {
//     case 1:    
//     case 2:
//         return 1;
//     case 3:
//         return 3;
//     default:
//         return 0;
//     }
// }















#include "Map.h"
#include "Player.h"

struct Map_type{
    int running;
    int width;
    int height;
    int tileSize;
    int mapBuffer[TILE_COUNT_Y][TIlE_COUNT_X];

    Uint32 ticker;
};


 


int getTileSize(Map map){
    return map->tileSize;
}
int getHeight(Map map){
return map->height;
}
int getWidth(Map map){
    return map->width;
}
int getMapBufferItems(Map map,int x,int y){
    return map->mapBuffer[y][x];
}

Map Map_create(int width, int height)
{
    Map map = malloc(sizeof(struct Map_type));
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

    map->ticker = 0;

    return map;
}


void Map_destroy(Map map)
{
    free(map);
}

int Player_collisionWithOtherPlayer(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    // Simple AABB 
  return (
        x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2
    );
}

int Collision_Map(Map map, float x, float y, float width, float height)
{
    float left   = x;
    float right  = x + width - 1; // update
    float top    = y;
    float bottom = y + height - 1; // updatw

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

int checkCollision(Map map, int x, int y) {
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


void resolveCollision(Map map, Player player)
{
    float x = getPlayerX(player);
    float y = getPlayerY(player);
    float w = getPlayerWidth(player);
    float h = getPlayerHeight(player);

    float tileSize = getTileSize(map);

    float left   = x;
    float right  = x + w - 1;
    float top    = y;
    float bottom = y + h - 1;

    int tileLeft   = (int)(left / tileSize);
    int tileRight  = (int)(right / tileSize);
    int tileTop    = (int)(top / tileSize);
    int tileBottom = (int)(bottom / tileSize);

    int A1 = checkCollision(map, tileLeft, tileTop);
    int A2 = checkCollision(map, tileRight, tileTop);
    int B3 = checkCollision(map, tileLeft, tileBottom);
    int B4 = checkCollision(map, tileRight, tileBottom);

    int leftBlocked;
    int rightBlocked;
    int topBlocked;
    int bottomBlocked;

    #define NORMAL_TILE 0 
    #define SLOW_TILE 3

    if ((A1 != NORMAL_TILE && A1 != SLOW_TILE) || (B3 != NORMAL_TILE && B3 != SLOW_TILE))
    {
        x = (tileLeft + 1) * tileSize;
    }
    if ((A2 != NORMAL_TILE && A2 != SLOW_TILE) || (B4 != NORMAL_TILE && B4 != SLOW_TILE))
    {
        x = tileRight * tileSize - w;
    }
    if ((A1 != NORMAL_TILE && A1 != SLOW_TILE) || (A2 != NORMAL_TILE && A2 != SLOW_TILE))
    {
        y = (tileTop + 1) * tileSize;
    }
    if ((B3 != NORMAL_TILE && B3 != SLOW_TILE) || (B4 != NORMAL_TILE && B4 != SLOW_TILE))
    {
        y = tileBottom * tileSize - h;
    }

    setPlayerPosition(player, x, y);
}

void resolveCollisionRate(Map map, Player player, int miliseconds)
{
    Uint32 now = SDL_GetTicks();

    if (now <= map->ticker)
        return;

    // try to spawn
    resolveCollision(map, player);

    // set cooldown 
    map->ticker = now + miliseconds;
}