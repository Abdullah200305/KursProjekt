#ifndef RENDERER_H
#define RENDERER_H

#include "Game_state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* playerTexture[4];
    SDL_Texture *abilityTextures[6];
    SDL_Rect playerFrames[17];
    TTF_Font* hudFont;
} Renderer;

int Renderer_Init(Renderer* r, const char* title, int width, int height);
void Renderer_Clear(Renderer* r);
void Renderer_Present(Renderer* r);
void Renderer_Destroy(Renderer* r);
void getScale(Renderer* r, float* scaleX, float* scaleY);

// Rendering to the map and background
void Background_Image_Render(Renderer* r);
void Render_Map(Renderer* r, Map map);
void Render_ScreenFrame(Renderer* r);


// Rendering the player
void Render_Player(Renderer* r, Player player, int playerIndex);

//Rendering the lives
void Render_PlayerLives(Renderer* r, Player player, int startX, int startY);
void Render_PlayerHUD(Renderer* r, Player player, int playerIndex, int x, int y, int hasBomb);
//rendering bomb
void Render_Bomb(Renderer* r, Bomb bomb);
void Render_BombHUD(Renderer* r, Bomb bomb, int x, int y);

#endif // RENDERER_H