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

    damagePlayer(&p);
    printf("after 1 hit -> lives: %d, alive: %d\n", p.lives, p.alive);

    damagePlayer(&p);
    damagePlayer(&p);
    printf("after 3 hits -> lives: %d, alive: %d\n", p.lives, p.alive);

    printf("x: %.1f, y: %.1f, lives: %d, alive: %d\n", p.x, p.y, p.lives, p.alive);
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


