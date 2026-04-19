#include "Player.h"

struct  Player_type{
    float x;
    float y;
    float vx;   //velocity x
    float vy;   //velocity y
    float width;
    float height;
    int lives;  //nr of lives
    int alive;  //alive = 1 => alive or alive = 0 => dead
};

Player initPlayer(float x, float y,int id)
{
    Player p = malloc(sizeof(struct Player_type));
    p->x = x;
    p->y = y;
    p->vx = 0;
    p->vy = 0;
    p->lives = 3;
    p->alive = 1;
    return p;
}

void damagePlayer(Player player) {
    if (player->alive == 0) {
        return;
    }

    player->lives--;

    if (player->lives <= 0) {
        player->lives = 0;
        player->alive = 0;
    }
}

void updatePlayer(Player player){
    player->x += player->vx;
    player->y += player->vy;
}
int isPlayerAlive(Player player){
    return player->alive;
}
void clampPlayerToMap(Player player, float minX, float minY, float maxX, float maxY){
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
void stopPlayer(Player player) {
    player->vx = 0;
    player->vy = 0;
}
// void resetPlayer(Player player, float x, float y) {
//     initPlayer(player, x, y);
// }
void killPlayer(Player player) {
    player->lives = 0;
    player->alive = 0;
}

//Implementation av getters metoder

float getPlayerX(Player player) {
    return player->x;
}

float getPlayerY(Player player) {
    return player->y;
}

float getPlayerVelocityX(Player player) {
    return player->vx;
}

float getPlayerVelocityY(Player player) {
    return player->vy;
}
float getPlayerWidth(Player player) {
    return player->width;
}

float getPlayerHeight(Player player) {
    return player->height;
}

int getPlayerLives(Player player) {
    return player->lives;
}

//Implementation av setters metoder

void setPlayerVelocity(Player player, float vx, float vy) {
    player->vx = vx;
    player->vy = vy;
}
void setPlayerPosition(Player player, float x, float y) {
    player->x = x;
    player->y = y;
}
void setPlayerSize(Player player, float width, float height) {
    player->width = width;
    player->height = height;
}
