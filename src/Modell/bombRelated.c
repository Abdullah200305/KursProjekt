#include "bombRelated.h"
#include "Player.h" 

void initBomb(Bomb *pBomb, Player players[]){ 
    pBomb->bombCarrier = rand() % 4;
    pBomb->timer = 200;
    pBomb->active = 1;
    pBomb->x = players[pBomb->bombCarrier].x;
    pBomb->y = players[pBomb->bombCarrier].y;
}

void bombExplosion(Bomb *pBomb, Player players[]){
    players[pBomb->bombCarrier].lives--;

    if(players[pBomb->bombCarrier].lives <= 0){
        players[pBomb->bombCarrier].alive = 0;
    }

    initBomb(pBomb, players);
}

void updateBomb(Bomb *pBomb, Player players[]){
    if(pBomb->active==0) return;

    pBomb->x = players[pBomb->bombCarrier].x;
    pBomb->y = players[pBomb->bombCarrier].y;
    pBomb->timer--;

    if(pBomb->timer<=0){
        players[pBomb->bombCarrier].lives--;

        if(players[pBomb->bombCarrier].lives<=0){
            players[pBomb->bombCarrier].alive = 0;
        }
    }
}