#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    float x;
    float y;
    float vx;   //velocity x
    float vy;   //velocity y
    int lives;  //nr of lives
    int alive;  //alive = 1 => alive or alive = 0 => dead
} Player;

void initPlayer(Player *player, float x, float y);
void damagePlayer(Player *player);
void updatePlayer(Player *player);
void setPlayerVelocity(Player *player, float vx, float vy);
void setPlayerPosition(Player *player, float x, float y);
int isPlayerAlive(Player *player);
void clampPlayerToMap(Player *player, float minX, float minY, float maxX, float maxY);


#endif