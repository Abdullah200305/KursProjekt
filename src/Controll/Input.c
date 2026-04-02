#include "Input.h"

 InputState* Input_Init(){
    InputState *input = malloc(sizeof(InputState));
    input -> quit = false;

    input -> up = false;
    input -> down = false;
    input -> left = false;
    input -> right = false;
    input -> action = false;
    
    return input;
}

void Input_HandleEvents(InputState * input) {
    SDL_Event event;

    while ( SDL_PollEvent(&event) ) {
        if (event.type == SDL_QUIT) {
            input -> quit = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_PAGEUP:
                    input -> up = true;
                    break;
                case SDLK_PAGEDOWN:
                    input -> down = true;
                    break;
                case SDLK_HOME:
                    input -> left = true;
                    break;
                case SDLK_END:
                    input -> right = true;
                    break;
                case SDLK_SPACE:
                    input -> action = true;
                    break;
            }
        }
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_PAGEUP:
                    input -> up = false;
                    break;
                case SDLK_PAGEDOWN:
                    input -> down = false;
                    break;
                case SDLK_HOME:
                    input -> left = false;
                    break;
                case SDLK_END:
                    input -> right = false;
                    break;
                case SDLK_SPACE:
                    input -> action = false;
                    break;
            }
        }
    }
}