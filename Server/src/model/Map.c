#include "Map.h"


struct Map_type{
    int running;
    int width;
    int height;
    int tileSize;
    int mapBuffer[TILE_COUNT_Y][TILE_COUNT_X];

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
    ReadMap = fopen("link/Test2.txt", "r");
    if (ReadMap)
    {
        int item;
        for (int y = 0; y < TILE_COUNT_Y; y++)
        {
            for (int x = 0; x < TILE_COUNT_X; x++)
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

TileType Collision_Map(Map map, float x, float y, float width, float height)
{
    float padding = 2.0f;

    float left   = x + padding;
    float right  = x + width  - 1 - padding;

    float top    = y + padding;
    float bottom = y + height - 1 - padding;

    int tileLeft   = (int)(left   / map->tileSize);
    int tileRight  = (int)(right  / map->tileSize);

    int tileTop    = (int)(top    / map->tileSize);
    int tileBottom = (int)(bottom / map->tileSize);

    TileType A1 = checkCollision(map, tileLeft,  tileTop);
    TileType A2 = checkCollision(map, tileRight, tileTop);

    TileType B1 = checkCollision(map, tileLeft,  tileBottom);
    TileType B2 = checkCollision(map, tileRight, tileBottom);

    // Walls have highest priority
    if (A1 == TILE_WALL || A1 == TILE_OUTSIDE_WALL ||
        A2 == TILE_WALL || A2 == TILE_OUTSIDE_WALL ||
        B1 == TILE_WALL || B1 == TILE_OUTSIDE_WALL ||
        B2 == TILE_WALL || B2 == TILE_OUTSIDE_WALL)
    {
        return TILE_WALL;
    }

    // Sand
    if (A1 == TILE_SAND ||
        A2 == TILE_SAND ||
        B1 == TILE_SAND ||
        B2 == TILE_SAND)
    {
        return TILE_SAND;
    }

    return TILE_NORMAL;
}



TileType checkCollision(Map map, int x, int y) {

    switch (map->mapBuffer[y][x])
    {
    case 1:
        return TILE_OUTSIDE_WALL;    
    case 2:
        return TILE_WALL;
    case 3:
        return TILE_SAND;
    default:
        return TILE_NORMAL;
    }
   
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


MapId getRandomMapId(void)
{
    return (MapId)((rand() % 6) + 1);
}
const char *getMapImagePath(MapId mapId)
{
    switch (mapId)
    {
    case MAP_DESERT:
        return "link/Maps/Desert.png";

    case MAP_FOREST:
        return "link/Maps/Forest.png";

    case MAP_ICELAND:
        return "link/Maps/Iceland.png";

    case MAP_GRAVEYARD:
        return "link/Maps/Graveyard.png";

    case MAP_LAVA:
        return "link/Maps/Lava.png";

    case MAP_PIRATE:
        return "link/Maps/Pirate.png";

    default:
        return "link/Maps/Island.png";
    }
}