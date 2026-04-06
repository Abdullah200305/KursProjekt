#include <stdio.h>
#include <SDL2/SDL.h>
#include "..\Include\main.h"

    int main(int argc, char *argv[])
    {
        Client *client;
        Game game; // model
        Renderer renderer; // view
        Event event; // input state
        
        printf("Starting game...\n");
        game_init(&game, &renderer, &event); // update game state and initialize renderer

        // test only 
        client = client_net_init();
        ClientConnection(client,"127.168.1.1",7777);
        printf("connection is done!\n");

        Client_Send(client,&event.input,sizeof(InputState));
        printf("send info\n");
       
        

         




        game.numPlayers = 2; // Set the number of players in the game
        initPlayer(&game.players[0], 230, 300);   // fel place muse change later

        initPlayer(&game.players[1], 270, 300);   // fel place muse change later
     
        printf("Entering game loop...\n");
        game_loop(&game, &renderer,&event); // start game loop
        game_cleanup(&game, &renderer,&event); // clean up resources
        return 0;
    }



