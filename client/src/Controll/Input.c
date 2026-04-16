#include "Input.h"

struct InputState_type{

    bool quit;
    bool up;
    bool down;
    bool left;
    bool right;
    bool action;


};



InputState Input_Init(){
    InputState  input = malloc(sizeof(struct InputState_type));
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
    return input;
}



void Destroy_Input(InputState input){
    free(input);
}

void Input_HandleEvents(InputState  input){
  SDL_Event event;       
  while (SDL_PollEvent(&event)) {

    if(event.type == SDL_QUIT) {
         printf("QUIT detected!\n");
        input->quit = true;
    }

 else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
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
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
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


