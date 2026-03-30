#ifndef RENDERER_H
#define RENDERER_H

#include "Game_state.h"
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
} Renderer;

int Renderer_Init(Renderer* r, const char* title, int width, int height);
void Renderer_Clear(Renderer* r);
void Renderer_Present(Renderer* r);
void Renderer_Destroy(Renderer* r);




void Render_Map(Renderer* r, Map* map);

#endif // RENDERER_H