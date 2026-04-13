#include "..\..\shared\Include\Renderer.h"


int Renderer_Init(Renderer* r, const char* title, int width, int height) {
    printf("Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }
    r->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width/2, height/2, SDL_WINDOW_RESIZABLE);
    
    if (!r->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    r->sdlRenderer = SDL_CreateRenderer(r->window, -1, 0);
    SDL_RenderSetLogicalSize(r->sdlRenderer, width, height);
    if (!r->sdlRenderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        Renderer_Destroy(r);
        return -1;
    }
   
    r->backgroundTexture = IMG_LoadTexture(r->sdlRenderer, "../shared/link/Island.png");
    if (!r->backgroundTexture) {
        fprintf(stderr, "IMG_LoadTexture Error: %s\n", IMG_GetError());
        Renderer_Destroy(r);
        return -1;
    }
    r->playerTexture = IMG_LoadTexture(r->sdlRenderer, "../shared/link/Player.png");
    if (!r->playerTexture) {
        fprintf(stderr, "IMG_LoadTexture Error: %s\n", IMG_GetError());
        Renderer_Destroy(r);
        return -1;
    }
    return 0;
}

// test to see if the renderer work.
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
    SDL_DestroyTexture(r->backgroundTexture);
    SDL_DestroyTexture(r->playerTexture);
    SDL_Quit();
}



//******************  Map stuff  *******************//
void Background_Image_Render(Renderer* r) {
   
    SDL_Texture* img = r->backgroundTexture;
    SDL_Rect texr={0,0,WIDTH,HEIGHT};
    SDL_RenderClear(r->sdlRenderer);
    SDL_RenderCopy(r->sdlRenderer, img, NULL, &texr);
}
// Render the map based on the map buffer to make collision
void Render_Map(Renderer* r, Map* map) {
    for (int y = 0; y < TILE_COUNT_Y; y++) {
        for (int x = 0; x < TIlE_COUNT_X; x++) {
            int tileType = map->mapBuffer[y][x];
            SDL_Rect tileRect = { x * map->tileSize, y * map->tileSize, map->tileSize, map->tileSize };
            switch (tileType) {
                case 0: 
                    // SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);
                  
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 255,0, 125); // Transparent
                    break;
                case 1:
                    // SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 255, 0, 0); // Black
                    break;
                case 2:
                
                    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255);    
                    break; 
                case 3:
                
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 255, 255);   
                    break;      
                default:
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 125, 255, 255); 
                 
                    break;
            }
            SDL_RenderDrawRect(r->sdlRenderer, &tileRect);
        }
    }
}




//******************  Player stuff  *******************//
void Render_Player(Renderer* r, Player* player) {
    SDL_Texture *img = r->playerTexture;
    SDL_Rect playerRect = { (int)player->x, (int)player->y, 32, 32 };// will be changed
    SDL_SetRenderDrawColor(r->sdlRenderer,0, 255, 0, 255); // to test the player render
    SDL_RenderFillRect(r->sdlRenderer, &playerRect);
    SDL_RenderCopy(r->sdlRenderer, img, NULL, &playerRect);


   
  // sencor four /// for testing 
    // SDL_Rect top = { (int)player->x+64/2, (int)player->y, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &top); 

    // SDL_Rect left = { (int)player->x, (int)player->y+64/2, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &left); 

    // SDL_Rect right = { (int)player->x+64-1, (int)player->y+64/2, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &right); 

    // SDL_Rect bottom = { (int)player->x+64/2, (int)player->y+64-1, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &bottom); 


  
}