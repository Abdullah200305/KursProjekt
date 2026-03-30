#include "Map.h"

Map *Map_create()
{
    Map *map = (Map *)malloc(sizeof(Map));
    FILE *ReadMap;
    map->running = 1;
    map->width = WIDTH;
    map->height = HEIGHT;
     
    map->tileSize = TILE_SIZE;
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