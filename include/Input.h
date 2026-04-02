#ifndef INPUT_H
#define INPUT_H


#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct InputState {
    bool quit;

    bool up;
    bool down;
    bool left;
    bool right;

    bool action;
} InputState;

void Input_Init(InputState * input);
void Input_HandleEvents(InputState * input);

#endif // INPUT_H

