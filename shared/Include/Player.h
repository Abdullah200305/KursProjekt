#ifndef PLAYER_H
#define PLAYER_H
#include <stdio.h>
#include <stdlib.h>
#include  "protocol.h"
typedef struct Player_type *Player;


// server will call all these 
Player initPlayer(float x, float y,int id);
void damagePlayer(Player player);
void updatePlayer(Player player);
int isPlayerAlive(Player player);
void clampPlayerToMap(Player player, float minX, float minY, float maxX, float maxY);
void stopPlayer(Player player);
// void resetPlayer(Player player, float x, float y);
void killPlayer(Player player);
//////////////////////


//Getters metoder

float getPlayerX(Player player);
float getPlayerY(Player player);
float getPlayerVelocityX(Player player);
float getPlayerVelocityY(Player player);
float getPlayerWidth(Player player);
float getPlayerHeight(Player player);
int getPlayerLives(Player player);
int getPlayerId(Player player);



//Setters metoder

void setPlayerVelocity(Player player, float vx, float vy);
void setPlayerPosition(Player player, float x, float y);
void setPlayerSize(Player player, float width, float height);
void setPlayerId(Player player,int id);

void PlayerDestroy(Player player);





void Player_ToData(Player src, DataPlayer *dst);


#endif