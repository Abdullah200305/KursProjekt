#include "bombRelated.h"
#include "Player.h"
#include <stdlib.h>



Bomb* createBomb(Player players[]){
    Bomb *pBomb = malloc(sizeof(Bomb));

    if (pBomb == NULL) return NULL;
    
    pBomb->bombCarrier = rand() % 4;
    pBomb->timer = 200;
    pBomb->active = 1;
    pBomb->x = players[pBomb->bombCarrier].x;
    pBomb->y = players[pBomb->bombCarrier].y;

    return pBomb;
}

void destroyBomb(Bomb *pBomb){
    free(pBomb);
}

void resetBomb(Bomb *pBomb, Player players[]){
    if(pBomb==NULL) return;

    pBomb->bombCarrier = rand() % 4;
    pBomb->timer = 200;
    pBomb->active = 1;
    pBomb->x = players[pBomb->bombCarrier].x;
    pBomb->y = players[pBomb->bombCarrier].y;

}

void bombExplosion(Bomb *pBomb, Player players[]){
    if(pBomb == NULL) return;

    players[pBomb->bombCarrier].lives--;

    if(players[pBomb->bombCarrier].lives <= 0){
        players[pBomb->bombCarrier].alive = 0;
    }

    resetBomb(pBomb, players);
}

void updateBomb(Bomb *pBomb, Player players[]){

    if(pBomb == NULL) return;
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