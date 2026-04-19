#ifndef COLLISION_H
#define COLLISION_H
#include "Map.h"
int Player_collisionWithOtherPlayer(int x1, int y1, int x2, int y2);
int Collision_Map(Map map, float x, float y);
int checkCollision(Map map, int x, int y);

#endif //COLLISION_H