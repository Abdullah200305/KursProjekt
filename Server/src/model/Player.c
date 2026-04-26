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
    int id;

    float speedY;
    float speedX;

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
    p->freezeTimer = 0;
    p->sizeUpTimer = 0;
    p->shieldTimer = 0;

    p->hasShield = 0;
    return p;
}



// will use abody
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


// will remove
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

// will remove
void stopPlayer(Player player) 
{
    player->vx = 0;
    player->vy = 0;
}
// will remove
// void resetPlayer(Player player, float x, float y) {
//     initPlayer(player, x, y);
// }
// will remove
void killPlayer(Player player) 
{
    player->lives = 0;
    player->alive = 0;
}





//Implementation av getters metoder

int getPlayerId(Player player){
    return player->id;
}
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
float getPlayerSpeedX(Player p)
{
    return p->speedX;
}
float getPlayerSpeedY(Player p)
{
    return p->speedY;
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

//

void playerMovement(
    Player player,
    SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right
)
{
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
}

void PlayerDestroy(Player player){
    free(player);
}



