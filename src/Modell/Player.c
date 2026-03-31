#include "Player.h"

void initPlayer(Player *player, float x, float y){
    player->x = x;
    player->y = y;
    player->vx = 0;
    player->vy = 0;
    player->lives = 3;
    player->alive = 1;
}

void damagePlayer(Player *player) {
    if (player->alive == 0) {
        return;
    }

    player->lives--;

    if (player->lives <= 0) {
        player->lives = 0;
        player->alive = 0;
    }
}

void updatePlayer(Player *player){
    player->x += player->vx;
    player->y += player->vy;
}

void setPlayerVelocity(Player *player, float vx, float vy) {
    player->vx = vx;
    player->vy = vy;
}
void setPlayerPosition(Player *player, float x, float y) {
    player->x = x;
    player->y = y;
}
int isPlayerAlive(Player *player){
    return player->alive;
}
void clampPlayerToMap(Player *player, float minX, float minY, float maxX, float maxY){
       if (player->x < minX) {
        player->x = minX;
    }

    if (player->y < minY) {
        player->y = minY;
    }

    if (player->x > maxX) {
        player->x = maxX;
    }

    if (player->y > maxY) {
        player->y = maxY;
    } 
}
void stopPlayer(Player *player) {
    player->vx = 0;
    player->vy = 0;
}
void resetPlayer(Player *player, float x, float y) {
    initPlayer(player, x, y);
}
void killPlayer(Player *player) {
    player->lives = 0;
    player->alive = 0;
}

//Implementation av getters metoden

float getPlayerX(Player *player) {
    return player->x;
}

float getPlayerY(Player *player) {
    return player->y;
}

float getPlayerVelocityX(Player *player) {
    return player->vx;
}

float getPlayerVelocityY(Player *player) {
    return player->vy;
}

int getPlayerLives(Player *player) {
    return player->lives;
}