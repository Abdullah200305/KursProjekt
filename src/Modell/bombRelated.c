#include "bombRelated.h"
#include "Player.h"
#include <stdlib.h>

struct Bomb_type{
    float x;
    float y;
    int timer;
    int bombCarrier;
    int active;
    int exploding;
    int explosionTimer;
};



Bomb createBomb(Player players[]){
    Bomb pBomb = malloc(sizeof(struct Bomb_type));

    if (pBomb == NULL) return NULL;
    
    pBomb->bombCarrier = rand() % 2;   // demo: vi har 2 spelare
    pBomb->timer = BOMB_TIMER;  //ändra i bombRelated.h
    pBomb->active = 1;
    pBomb->exploding = 0;
    pBomb->explosionTimer = 0;
    pBomb->x = getPlayerX(players[0]);
    pBomb->y = getPlayerY(players[0]);

    return pBomb;
}

void resetBomb(Bomb pBomb, Player players[]){
    if(pBomb==NULL) return;

    pBomb->bombCarrier = rand() % 2;   // demo: vi har 2 spelare
    pBomb->timer = BOMB_TIMER;
    pBomb->active = 1;
    pBomb->exploding = 0;
    pBomb->explosionTimer = 0;
    pBomb->x = getPlayerX(players[0]);
    pBomb->y = getPlayerY(players[0]);
}

void bombExplosion(Bomb pBomb, Player players[]){
    if(pBomb == NULL) return;
    if(pBomb->exploding) return;

    if (getPlayerShield(players[pBomb->bombCarrier]))
    {
        setPlayerShield(players[pBomb->bombCarrier], 0);
        setPlayerShieldTimer(players[pBomb->bombCarrier], 0);
    }
    else
    {
        damagePlayer(players[pBomb->bombCarrier]);
    }

    pBomb->active = 0;
    pBomb->exploding = 1;
    pBomb->explosionTimer = 20;
}


void updateBomb(Bomb pBomb, Player players[]){
    if(pBomb == NULL) return;

    if (pBomb->exploding) {
        pBomb->explosionTimer--;

        if (pBomb->explosionTimer <= 0) {
            resetBomb(pBomb, players);
        }
        return;
    }

    if(!pBomb->active) return;

    pBomb->x = getPlayerX(players[pBomb->bombCarrier]);
    pBomb->y = getPlayerY(players[pBomb->bombCarrier]);
    pBomb->timer--;

    if (pBomb->timer <= 0) {
        bombExplosion(pBomb, players);
    }
}

float getBombX(Bomb pBomb) {
    return pBomb->x;
}

float getBombY(Bomb pBomb) {
    return pBomb->y;
}

int getBombTimer(Bomb pBomb) {
    return pBomb->timer;
}

int getBombCarrier(Bomb pBomb) {
    return pBomb->bombCarrier;
}
int getBombExploding(Bomb pBomb){
    return pBomb->exploding;
}
int getBombActive(Bomb pBomb){
    return pBomb->active;
}


void setBombCarrier(Bomb pBomb, int index){
    pBomb->bombCarrier = index;
}