#include "bombRelated.h"
#include "Player.h"
#include <stdlib.h>



Bomb* createBomb(Player players[]){
    Bomb *pBomb = malloc(sizeof(Bomb));

    if (pBomb == NULL) return NULL;
    
    pBomb->bombCarrier = rand() % 2;   // demo: vi har 2 spelare
    pBomb->timer = 200;
    pBomb->active = 1;
    pBomb->exploding = 0;
    pBomb->explosionTimer = 0;
    pBomb->x = players[pBomb->bombCarrier].x;
    pBomb->y = players[pBomb->bombCarrier].y;

    return pBomb;
}

void resetBomb(Bomb *pBomb, Player players[]){
    if(pBomb==NULL) return;

    pBomb->bombCarrier = rand() % 2;   // demo: vi har 2 spelare
    pBomb->timer = 200;
    pBomb->active = 1;
    pBomb->exploding = 0;
    pBomb->explosionTimer = 0;
    pBomb->x = players[pBomb->bombCarrier].x;
    pBomb->y = players[pBomb->bombCarrier].y;
}

void bombExplosion(Bomb *pBomb, Player players[]){
    if(pBomb == NULL) return;
    if(pBomb->exploding) return;

    damagePlayer(&players[pBomb->bombCarrier]);

    pBomb->active = 0;
    pBomb->exploding = 1;
    pBomb->explosionTimer = 20;
}


void updateBomb(Bomb *pBomb, Player players[]){
    if(pBomb == NULL) return;

    if (pBomb->exploding) {
        pBomb->explosionTimer--;

        if (pBomb->explosionTimer <= 0) {
            resetBomb(pBomb, players);
        }
        return;
    }

    if(!pBomb->active) return;

    pBomb->x = players[pBomb->bombCarrier].x;
    pBomb->y = players[pBomb->bombCarrier].y;
    pBomb->timer--;

    if (pBomb->timer <= 0) {
        bombExplosion(pBomb, players);
    }
}

float getBombX(Bomb *pBomb) {
    return pBomb->x;
}

float getBombY(Bomb *pBomb) {
    return pBomb->y;
}

int getBombTimer(Bomb *pBomb) {
    return pBomb->timer;
}

int getBombCarrier(Bomb *pBomb) {
    return pBomb->bombCarrier;
}