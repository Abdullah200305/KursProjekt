#include "Player.h"
#include <SDL2/SDL.h>

struct  Player_type
{
    float x;
    float y;
    float vx;   //velocity x
    float vy;   //velocity y
    float width;
    float height;
    int lives;  //nr of lives
    int alive;  //alive = 1 => alive or alive = 0 => dead

    float speedY;
    float speedX;

    //animation
    int animationFrame;
    int animationTimer;
    int lastDirection;

    //float speedTimer;
    float speedTimer;
    float freezeTimer;
    float sizeUpTimer;
    float shieldTimer;

    int hasShield;
};




 
Player initPlayer(float x, float y)
{
    Player p = malloc(sizeof(struct Player_type));
    p->x = x;
    p->y = y;
    p->vx = 0;
    p->vy = 0;
    p->width = 32;
    p->height = 32;
    p->lives = 3;
    p->alive = 1;

    p->speedY = 5;
    p->speedX = 5;

    p->speedTimer = 0;

    //animation
    p->animationFrame = 0;
    p->animationTimer = 0;
    p->lastDirection = 0;

    p->freezeTimer = 0;
    p->sizeUpTimer = 0;
    p->shieldTimer = 0;
    p->hasShield = 0;

    return p;
}

void damagePlayer(Player player) 
{
    if (player->alive == 0) 
    {
        return;
    }

    player->lives--;

    if (player->lives <= 0) 
    {
        player->lives = 0;
        player->alive = 0;
    }
}

void updatePlayer(Player player)
{
    player->x += player->vx;
    player->y += player->vy;

    if (player->shieldTimer > 0)
    {
        player->shieldTimer--;

        if (player->shieldTimer <= 0)
        {
            player->shieldTimer = 0;
            player->hasShield = 0;
        }
    }
}

int isPlayerAlive(Player player)
{
    return player->alive;
}

void clampPlayerToMap(Player player, float minX, float minY, float maxX, float maxY)
{
    if (player->x < minX) 
    {
        player->x = minX;
    }

    if (player->y < minY) 
    {
        player->y = minY;
    }

    if (player->x > maxX) 
    {
        player->x = maxX;
    }

    if (player->y > maxY) 
    {
        player->y = maxY;
    } 
}


void stopPlayer(Player player) 
{
    player->vx = 0;
    player->vy = 0;
}

// void resetPlayer(Player player, float x, float y) {
//     initPlayer(player, x, y);
// }

void killPlayer(Player player) 
{
    player->lives = 0;
    player->alive = 0;
}

//Implementation av getters metoder

float getPlayerX(Player player) 
{
    return player->x;
}

float getPlayerY(Player player) 
{
    return player->y;
}

float getPlayerVelocityX(Player player) 
{
    return player->vx;
}

float getPlayerVelocityY(Player player) 
{
    return player->vy;
}

float getPlayerWidth(Player player) 
{
    return player->width;
}

float getPlayerHeight(Player player) 
{
    return player->height;
}

int getPlayerLives(Player player) 
{
    return player->lives;
}

// Timers

float getPlayerSpeedTimer(Player player) 
{
    return player->speedTimer;
}

float getPlayerFreezeTimer(Player player) 
{
    return player->freezeTimer;
}

float getPlayerSizeUpTimer(Player player) 
{
    return player->sizeUpTimer;
}

int getPlayerShield(Player player)
{
    return player->hasShield;
}

void setPlayerShield(Player player, int value)
{
    player->hasShield = value;
}

float getPlayerShieldTimer(Player player)
{
    return player->shieldTimer;
}

void setPlayerShieldTimer(Player player, float timer)
{
    player->shieldTimer = timer;
}

//Implementation av setters metoder

void setPlayerX(Player player, float x)
{
    player->x = x;
}

void setPlayerY(Player player, float y)
{
    player->y = y;
}

void setPlayerVelocity(Player player, float vx, float vy) 
{
    player->vx = vx;
    player->vy = vy;
}

void setPlayerPosition(Player player, float x, float y) 
{
    player->x = x;
    player->y = y;
}

void setPlayerSize(Player player, float width, float height) 
{
    player->width = width;
    player->height = height;
}

void setPlayerSpeedYX(Player player, float speedY, float speedX) 
{
    player->speedY = speedY;
    player->speedX = speedX;
}

//Timers
void setPlayerSpeedTimer(Player player, float timer) 
{
    player->speedTimer = timer;
}

void setPlayerFreezeTimer(Player player, float timer) 
{
    player->freezeTimer = timer;
}

void setPlayerSizeUpTimer(Player player, float timer) 
{
    player->sizeUpTimer = timer;
}

//Player movement and Animation
int getPlayerAnimationFrame(Player player) {
    return player->animationFrame;
}

void setPlayerAnimation(Player player) {
    if(player->freezeTimer > 0){
        player->animationFrame = 13;
        return;
    }
    
    int isMoving = (player->vx != 0 || player->vy != 0);

    if (player->vy > 0)
        player->lastDirection = 0;
    else if (player->vx > 0)
        player->lastDirection = 1;
    else if (player->vx < 0) 
        player->lastDirection = 2;
    else if (player->vy < 0) 
        player->lastDirection = 3;

    if (!isMoving) {
        switch (player->lastDirection) {
            case 0: 
                player->animationFrame = 0;  
                break;
            case 1: 
                player->animationFrame = 4;  
                break;
            case 2: 
                player->animationFrame = 7;  
                break;
            case 3: 
                player->animationFrame = 10; 
                break;
        }
        player->animationTimer = 0;
        return;
    }

    int animationTransitionSpeed = (player->speedTimer > 0) ? 3 : 6;

    player->animationTimer++;
    if (player->animationTimer >= animationTransitionSpeed) {
        player->animationTimer = 0;
        switch (player->lastDirection) {
            case 0: 
                player->animationFrame = (player->animationFrame == 2) ? 3 : 2;
                break;
            case 1: 
                player->animationFrame = (player->animationFrame == 5) ? 6 : 5;
                break;
            case 2: 
                player->animationFrame = (player->animationFrame == 8) ? 9 : 8;
                break;
            case 3: 
                player->animationFrame = (player->animationFrame == 11) ? 12 : 11;
                break;
        }
    }
}

void playerMovement(Player player,
    SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right) {

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    float vx = 0;
    float vy = 0;

    int upPressed    = state[up];
    int downPressed  = state[down];
    int leftPressed  = state[left];
    int rightPressed = state[right];

    // Vertical
    if (upPressed && !downPressed)
        vy = -player->speedY;
    else if (downPressed && !upPressed)
        vy = player->speedY;

    // Horizontal
    if (leftPressed && !rightPressed)
        vx = -player->speedX;
    else if (rightPressed && !leftPressed)
        vx = player->speedX;

    setPlayerVelocity(player, vx, vy);
    setPlayerAnimation(player);
}

