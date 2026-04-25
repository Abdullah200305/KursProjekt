#ifndef PLAYER_H
#define PLAYER_H
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
typedef struct Player_type *Player;


Player initPlayer(float x, float y);
void damagePlayer(Player player);
void updatePlayer(Player player);
int isPlayerAlive(Player player);
void clampPlayerToMap(Player player, float minX, float minY, float maxX, float maxY);
void stopPlayer(Player player);
// void resetPlayer(Player player, float x, float y);
void killPlayer(Player player);

//Getters metoder

float getPlayerX(Player player);
float getPlayerY(Player player);
float getPlayerVelocityX(Player player);
float getPlayerVelocityY(Player player);
float getPlayerWidth(Player player);
float getPlayerHeight(Player player);
int getPlayerLives(Player player);
float getPlayerSpeedTimer(Player player);
float getPlayerFreezeTimer(Player player);
float getPlayerSizeUpTimer(Player player);
int getPlayerShield(Player player);
float getPlayerShieldTimer(Player player);

//Setters metoder
void setPlayerX(Player player, float x);
void setPlayerY(Player player, float y);
void setPlayerVelocity(Player player, float vx, float vy);
void setPlayerPosition(Player player, float x, float y);
void setPlayerSize(Player player, float width, float height);
void setPlayerSpeedYX(Player player, float speedY, float speedX);
void setPlayerSpeedTimer(Player player, float timer);
void setPlayerFreezeTimer(Player player, float timer);
void setPlayerSizeUpTimer(Player player, float timer);
void setPlayerShield(Player player, int value);
void setPlayerShieldTimer(Player player, float timer);
//MISC FUNCTIONS

void playerMovement(
    Player player,
    SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right
);

void uncollidePlayer(Player player);

#endif