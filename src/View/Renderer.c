#include "Renderer.h"



int Renderer_Init(Renderer* r, const char* title, int width, int height) {
    printf("Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }
    r->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!r->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    r->sdlRenderer = SDL_CreateRenderer(r->window, -1, 0);
    if (!r->sdlRenderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(r->window);
        SDL_Quit();
        return -1;
    }
    return 0;
}


void Background_Image_Render(Renderer* r) {
    SDL_Texture *img = IMG_LoadTexture(r->sdlRenderer,"link/Background.png");
    SDL_Rect texr={0,0,WIDTH,HEIGHT};
    SDL_RenderClear(r->sdlRenderer);
    SDL_RenderCopy(r->sdlRenderer, img, NULL, &texr);
}



void Render_Map(Renderer* r, Map* map) {
   
    for (int y = 0; y < TILE_COUNT_Y; y++) {
        for (int x = 0; x < TIlE_COUNT_X; x++) {
            int tileType = map->mapBuffer[y][x];
            SDL_Rect tileRect = { x * map->tileSize, y * map->tileSize, map->tileSize, map->tileSize };
            switch (tileType) {
                case 0: 
                    SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 255, 255, 0); // Transparent
                    break;
                case 1:
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255); // Black
                    break;
                default:
                    SDL_SetRenderDrawColor(r->sdlRenderer, 25, 0, 0, 255); 
                    break;
            }
            SDL_RenderFillRect(r->sdlRenderer, &tileRect);
        }
    }
}

void Renderer_Clear(Renderer* r) {
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255);
    SDL_RenderClear(r->sdlRenderer);
}

void Renderer_Present(Renderer* r) {
    SDL_RenderPresent(r->sdlRenderer);
}

void Renderer_Destroy(Renderer* r) {
    SDL_DestroyRenderer(r->sdlRenderer);
    SDL_DestroyWindow(r->window);
    SDL_Quit();
}
