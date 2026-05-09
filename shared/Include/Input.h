#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    bool quit;

    bool up;
    bool down;
    bool left;
    bool right;

    bool mouseClicked;
    int mouseX;
    int mouseY;
} InputState;

InputState* Input_Init(void);
void Input_HandleEvents(InputState *input);
void Input_Destroy(InputState *input);

#endif