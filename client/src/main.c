#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"
#include "Ability.h"
#include "client_net.h"

int main(int argc, char *argv[])
{
   
    Game game;
    Renderer renderer;
    InputState *input = Input_Init();
    ClientNet clientNet = ClientNet_Init("127.0.0.1", 8080);
 

    // will be change place it 
    if (clientNet != NULL) {
        printf("[CLIENT] UDP init OK\n");
    } else {
        printf("[CLIENT] UDP init FAILED\n");
    }

    if (clientNet != NULL) {
        if (ClientNet_SendJoinRequest(clientNet) == 0) {
            printf("[CLIENT] JOIN_REQUEST sent\n");
        } else {
            printf("[CLIENT] JOIN_REQUEST failed\n");
        }
    }



    // here will happen init for game and get packet such as game_init and accept packet. from server 
    printf("Starting game...\n");
    game_init(&game, &renderer,clientNet);








    printf("Entering game loop...\n");
    game_loop(&game, &renderer, clientNet);

   

    // this will handle after will be change
    if (clientNet != NULL) {
        ClientNet_SendDisconnect(clientNet);
    }

    if (clientNet != NULL) {
        ClientNet_Destroy(clientNet);
    }
    game_cleanup(&game, &renderer);
    return 0;
}






