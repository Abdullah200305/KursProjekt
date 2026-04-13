#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"
    int main(int argc, char *argv[])
    {
        Game game; // model
        Renderer renderer; // view
         InputState*input = Input_Init();
    
        printf("Starting game...\n");
        game_init(&game, &renderer); // update game state and initialize renderer
        
        game.numPlayers = 2; // Set the number of players in the game
        initPlayer(&game.players[0], 230, 300);   // fel place muse change later
        initPlayer(&game.players[1], 270, 300);   // fel place muse change later
        
        game.bomb.active = 1;
        game.bomb.timer = 200;
        game.bomb.bombCarrier = 0;
        game.bomb.x = game.players[0].x;
        game.bomb.y = game.players[0].y;
       
       
        printf("Entering game loop...\n");
        game_loop(&game, &renderer); // start game loop
        game_cleanup(&game, &renderer); // clean up resources
        return 0;
    }



