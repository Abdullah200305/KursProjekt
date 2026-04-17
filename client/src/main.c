#include "main.h"   


int main(int argc, char *argv[])
    {
        Client *c= client_net_init();
        ClientConnection(c,"127.0.0.1",7777);
        Game game; // model
        Renderer renderer; // view
        InputState input = Input_Init();
       Packet packet;
       packet.type= PACKET_CONNECT;


        printf("Starting game...\n");
        Client_Send(c,(void *)&packet,sizeof(packet));





        






       game_init(&game, &renderer); // update game state and initialize renderer
    
       
       





        printf("Entering game loop...\n");
        game_loop(&game, &renderer,input); // start game loop
        game_cleanup(&game, &renderer,input); // clean up resources
        return 0;
    }







