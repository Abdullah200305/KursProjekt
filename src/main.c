#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"
    int main(int argc, char *argv[])
    {
        Game game; // model
        Renderer renderer; // view
        SDL_Event event;
        printf("Starting game...\n");
        game_init(&game, &renderer); // update game state and initialize renderer
        game_loop(&game, &renderer); // start game loop
        game_cleanup(&game, &renderer); // clean up resources
        return 0;
    }



