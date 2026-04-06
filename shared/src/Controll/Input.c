#include "..\..\shared\Include\Input.h"

InputState* Input_Init(){
    InputState * input = malloc(sizeof(InputState));
    if (!input) {
        fprintf(stderr, "Failed to allocate memory for InputState\n");
        return NULL;
    }
    input -> type = 0;
    input -> quit = false;
    input -> up = false;
    input -> down = false;
    input -> left = false;
    input -> right = false;
    input -> action = false;
    input -> event = (SDL_Event){0};
    return input;
}



void Destroy_Input(InputState *input){
    free(input);
}

void Input_HandleEvents(InputState * input){    
  while (SDL_PollEvent(&input->event)) {

    if(input->event.type == SDL_QUIT) {
         printf("QUIT detected!\n");
        input->quit = true;
    }

 else if (input->event.type == SDL_KEYDOWN) {
            switch (input->event.key.keysym.sym) {
                case SDLK_w:
                    input -> up = true;
                    break;
                case SDLK_s:
                    input -> down = true;
                    break;
                case SDLK_a:
                    input -> left = true;
                    break;
                case SDLK_d:
                    input -> right = true;
                    break;
                case SDLK_SPACE:
                    input -> action = true;
                    break;
            }
        }
        else if (input->event.type == SDL_KEYUP) {
            switch (input->event.key.keysym.sym) {
                case SDLK_w:
                    input -> up = false;
                    break;
                case SDLK_s:
                    input -> down = false;
                    break;
                case SDLK_a:
                    input -> left = false;
                    break;
                case SDLK_d:
                    input -> right = false;
                    break;
                case SDLK_SPACE:
                    input -> action = false;
                    break;
            }
        }     
}       
}