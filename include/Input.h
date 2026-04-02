#ifndef INPUT_H
#define INPUT_H


#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct{
    bool quit;
    bool up;
    bool down;
    bool left;
    bool right;
    bool action;
    // SDL_Event event;
} InputState;

InputState* Input_Init();

void Input_HandleEvents(InputState * input);

#endif // INPUT_H

