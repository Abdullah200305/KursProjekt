#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"
#include "Player.h"

int main(int argc, char *argv[])
{
     int ab = 100;
    // terminal output
    printf("%d\n", ab);
    

    //snabbt terminal tester om player.c funktioner funkar.
    Player p;
    initPlayer(&p, 100, 200);

    printf("start -> lives: %d, alive: %d\n", p.lives, p.alive);

    setPlayerVelocity(&p, 5, -2);
    updatePlayer(&p);
    printf("after update -> x: %.1f, y: %.1f\n", p.x, p.y);

    p.x = -20;
    p.y = 900;
    clampPlayerToMap(&p, 0, 0, 1280, 720);
    printf("after clamp -> x: %.1f, y: %.1f\n", p.x, p.y);

    setPlayerPosition(&p, 300, 400);
    printf("after set position -> x: %.1f, y: %.1f\n", p.x, p.y);

    setPlayerVelocity(&p, 7, -3);
    stopPlayer(&p);
    updatePlayer(&p);
    printf("after stop -> x: %.1f, y: %.1f, vx: %.1f, vy: %.1f\n", p.x, p.y, p.vx, p.vy);

    damagePlayer(&p);
    printf("after 1 hit -> lives: %d, alive: %d\n", p.lives, p.alive);

    damagePlayer(&p);
    damagePlayer(&p);
    printf("after 3 hits -> lives: %d, alive: %d\n", p.lives, p.alive);
    
    printf("isPlayerAlive: %d\n", isPlayerAlive(&p));

    resetPlayer(&p, 50, 60);
    printf("after reset -> x: %.1f, y: %.1f, lives: %d, alive: %d, vx: %.1f, vy: %.1f\n",
       p.x, p.y, p.lives, p.alive, p.vx, p.vy);
    killPlayer(&p);
    printf("after kill -> lives: %d, alive: %d\n", p.lives, p.alive);
    
    resetPlayer(&p, 50, 60);
    setPlayerVelocity(&p, 8, -4);

    printf("getters -> x: %.1f, y: %.1f, vx: %.1f, vy: %.1f, lives: %d, alive: %d\n",
       getPlayerX(&p),
       getPlayerY(&p),
       getPlayerVelocityX(&p),
       getPlayerVelocityY(&p),
       getPlayerLives(&p),
       isPlayerAlive(&p));
    

    setPlayerSize(&p, 64, 96);
    printf("size -> width: %.1f, height: %.1f\n", getPlayerWidth(&p), getPlayerHeight(&p)); 
       

    fflush(stdout);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


