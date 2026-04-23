#ifndef BOMB_RELATED_H
#define BOMB_RELATED_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <Player.h>

typedef struct Bomb_type *Bomb;


Bomb createBomb(Player players[]);
void destroyBomb(Bomb pBomb);

void resetBomb(Bomb pBomb, Player players[]);
void updateBomb(Bomb pBomb, Player players[]);
void bombExplosion(Bomb pBomb, Player players[]);

//---------Getters--------//
float getBombX(Bomb pBomb);
float getBombY(Bomb pBomb);
int getBombTimer(Bomb pBomb);
int getBombCarrier(Bomb pBomb);
int getBombExploding(Bomb pBomb);
int getBombActive(Bomb pBomb);

///-------Setters------//
void setBombCarrier(Bomb pBomb, int index);
void setBombState(Bomb pBomb, float x, float y, int timer, int bombCarrier, int active, int exploding);
#endif 