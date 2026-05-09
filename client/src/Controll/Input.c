#include "Input.h"
#include <stdlib.h>

InputState* Input_Init(void)
{
    InputState *input = malloc(sizeof(InputState));
    if (!input) return NULL;

    input->quit = false;

    input->up = false;
    input->down = false;
    input->left = false;
    input->right = false;

    input->mouseClicked = false;
    input->mouseX = 0;
    input->mouseY = 0;

    return input;
}

void Input_HandleEvents(InputState *input)
{
    SDL_Event event;

    input->mouseClicked = false;

    SDL_GetMouseState(&input->mouseX, &input->mouseY);

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            input->quit = true;
        }
        else if (event.type == SDL_MOUSEBUTTONUP &&
                 event.button.button == SDL_BUTTON_LEFT)
        {
            input->mouseClicked = true;
            input->mouseX = event.button.x;
            input->mouseY = event.button.y;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_PAGEUP: input->up = true; break;
                case SDLK_PAGEDOWN: input->down = true; break;
                case SDLK_HOME: input->left = true; break;
                case SDLK_END: input->right = true; break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_PAGEUP: input->up = false; break;
                case SDLK_PAGEDOWN: input->down = false; break;
                case SDLK_HOME: input->left = false; break;
                case SDLK_END: input->right = false; break;
            }
        }
    }
}

void Input_Destroy(InputState *input)
{
    free(input);
}