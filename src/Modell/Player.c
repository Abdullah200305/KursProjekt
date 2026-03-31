#include "Player.h"

void initPlayer(Player *player, float x, float y){
    player->x = x;
    player->y = y;
    player->vx = 0;
    player->vy = 0;
    player->lives = 3;
    player->alive = 1;
}

void damagePlayer(Player *player){
    player->lives--;
    if(player->lives <=0){
        player->alive = 0;
    }
    player ->lives--;
    if(player->lives <= 0){
        player->lives = 0;
        player->alive = 0;
    }
}

void updatePlayer(Player *player){
    player->x += player->vx;
    player->y += player->vy;
}