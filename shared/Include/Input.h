#ifndef INPUT_H
#define INPUT_H


#include <SDL2/SDL.h>
#include <stdbool.h>


typedef struct InputState_type *InputState;
InputState Input_Init();

void Input_HandleEvents(InputState  input);





void Input_HandleEvents(InputState input);
void Destroy_Input(InputState input);
#endif // INPUT_H

