#include "Input.h"

InputState* Input_Init(){
    InputState * input = malloc(sizeof(InputState));
    if (!input) {
        fprintf(stderr, "Failed to allocate memory for InputState\n");
        return NULL;
    }

    input -> quit = false;
    input -> up = false;
    input -> down = false;
    input -> left = false;
    input -> right = false;
    input -> action = false;
    input -> event = (SDL_Event){0};
    return input;
}

void Input_HandleEvents(InputState * input) {
   
    while ( SDL_PollEvent(&input->event) ) {
        input -> event = input->event;
        if (input->event.type == SDL_QUIT) {
            input -> quit = true;
        }
        else if (input->event.type == SDL_KEYDOWN) {
            switch (input->event.key.keysym.sym) {
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
        else if (input->event.type == SDL_KEYUP) {
            switch (input->event.key.keysym.sym) {
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