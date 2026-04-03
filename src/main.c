#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"
    int main(int argc, char *argv[])
    {
        Game game; // model
        Renderer renderer; // view
        InputState input; // input state
    
        printf("Starting game...\n");
        game_init(&game, &renderer,&input); // update game state and initialize renderer
       


        game.numPlayers = 2; // Set the number of players in the game
        initPlayer(&game.players[0], 230, 300);   // fel place muse change later

        initPlayer(&game.players[1], 270, 300);   // fel place muse change later
     
        printf("Entering game loop...\n");
        game_loop(&game, &renderer); // start game loop
        game_cleanup(&game, &renderer); // clean up resources
        return 0;
    }



