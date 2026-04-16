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


} InputState;

InputState* Input_Init();

void Input_HandleEvents(InputState * input);



typedef struct{
    InputState *input;
} Event;

InputState* Input_Init();
void Input_HandleEvents(InputState * input);
void Destroy_Input(InputState *input);
#endif // INPUT_H

