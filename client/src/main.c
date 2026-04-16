#include "main.h"
    int main(int argc, char *argv[])
    {
        Client *c= client_net_init();
        Game game; // model
        Renderer renderer; // view
        InputState input = Input_Init();
       
    
        printf("Starting game...\n");
        game_init(&game, &renderer); // update game state and initialize renderer
    
       
       





        printf("Entering game loop...\n");
        game_loop(&game, &renderer,input); // start game loop
        game_cleanup(&game, &renderer,input); // clean up resources
        return 0;
    }







