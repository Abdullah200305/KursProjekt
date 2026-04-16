#ifndef PHYSICS_H
#define PHYSICS_H
#include "Player.h"
void damagePlayer(Player player);
void updatePlayer(Player player);
int isPlayerAlive(Player player);
void clampPlayerToMap(Player player, float minX, float minY, float maxX, float maxY);
void stopPlayer(Player player);
// void resetPlayer(Player player, float x, float y);
void killPlayer(Player player);



#endif //PHYSICS_H