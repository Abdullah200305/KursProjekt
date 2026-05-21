#include "main.h"

int main(int argc, char *argv[])
{
   Game game;
   game.running = 1; // Start with the game not running, will be set to 1 when we want to exit
   Renderer renderer;
   InputState *input = Input_Init();
   ClientNet clientNet = NULL;

   // step one menu and sign in as client or server+client or qiut the game this will be inside
   menu_init(&game, &renderer);

  
   

while (game.running)
{
    switch (game.state)
    {
        case GAME_STATE_MENU:
            //printf("Client is in menu state\n");
            menu_loop(&game, &renderer);
            break;
        case GAME_STATE_COUNTDOWN:
           //printf("run\n");
            countdown_loop(&game, &renderer, clientNet);
            break;
            
            
        case GAME_STATE_PLAYING:
           // printf("Client is in menu playing state\n");
            game_loop(&game, &renderer, clientNet);
            break;

        case GAME_STATE_CHOOSE_HOST_JOIN:
           // printf("Client is in menu select state\n");
            choose_host_join_loop(&game, &renderer);
            break;

        case GAME_STATE_CLIENT_SETUP:
           // printf("Client is in menu join state\n");
            client_setup_loop(&game, &renderer, &clientNet);
            break;

        case GAME_STATE_HOST_SETUP:
           // printf("Client is in menu host state\n");
            host_setup_loop(&game, &renderer, &clientNet);
            break;

        case GAME_STATE_GAME_OVER:
            game_over_loop(&game, &renderer, clientNet);
            break;
        }
      
   
    SDL_Delay(16); // Delay to cap the frame rate at ~60 FPS    
}

 






   // steop chossing host or join and then if join will write the ip and port_numer and then click on connect button and wait for game to start
   // choose_host_join_loop(&game, &renderer);
   // host_setup_loop(&game, &renderer, clientNet);
   // client_setup_loop(&game, &renderer, clientNet);

   // Game game;

   // this will run when some one clikck on client player button and to write ip-for server and port number and then click on connect button and wiaatttttt
   // ClientNet clientNet = ClientNet_Init("127.0.0.1", 2000);
   // // will be change place it
   // if (clientNet != NULL) {
   //     printf("[CLIENT] UDP init OK\n");
   // } else {
   //     printf("[CLIENT] UDP init FAILED\n");
   // }

   // if (clientNet != NULL) {
   //     if (ClientNet_SendJoinRequest(clientNet) == 0) {
   //         printf("[CLIENT] JOIN_REQUEST sent\n");
   //     } else {
   //         printf("[CLIENT] JOIN_REQUEST failed\n");
   //     }
   // }

   // will start as load to every thing will be place right place update will be 7sec.
   // here will happen init for game and get packet such as game_init and accept packet. from server
   // printf("Starting game...\n");
   // game_init(&game, &renderer,clientNet);

   // in game runnn only nothing to deal with other stuff like menu or
   // this will run during game is in
   // printf("Entering game loop...\n");
   // game_loop(&game, &renderer, clientNet);

   // here will clean stuff

   if (clientNet != NULL) {
       ClientNet_SendDisconnect(clientNet);
   }
   if (clientNet != NULL) {
       ClientNet_Destroy(clientNet);
   }

  // issue here that make the game exit closer 
   // game_cleanup(&game, &renderer);
   return 0;
}
