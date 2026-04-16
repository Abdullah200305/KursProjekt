#include "collision.h"

int Player_collisionWithOtherPlayer(int x1, int y1, int x2, int y2) {
    // Simple AABB 
  return (
        x1 < x2 + 32 &&
        x1 + 32 > x2 &&
        y1 < y2 + 32 &&
        y1 + 32 > y2
    );
}

int Collision_Map(Map map, float x, float y)
{
    float left   = x;
    float right  = x + 32 - 1; // update
    float top    = y;
    float bottom = y + 32 - 1; // updatw

    int tileLeft   = (int)(left / getTileSize(map));
    int tileRight  = (int)(right / getTileSize(map));
    int tileTop    = (int)(top / getTileSize(map));
    int tileBottom = (int)(bottom / getTileSize(map));
    
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
    switch (getMapBufferItems(map,x,y))
    {
    case 1:    
    case 2:
        return 1;
    case 3:
        return 3;
    default:
        return 0;
    }
}