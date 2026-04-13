#ifndef BOMB_RELATED_H
#define BOMB_RELATED_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Player.h>

typedef struct{
    float x;
    float y;
    int timer;
    int bombCarrier;
    int active; //vem som har bomb, 1=bomb, 0=ingen bomb

}Bomb;

void initBomb(Bomb *pBomb, Player players[]);
void bombExplosion(Bomb *pBomb, Player players[]);
void updateBomb(Bomb *pBomb, Player players[]);




#endif 