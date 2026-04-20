#include "game.h"


void run(Server server)
{
    while (getServerRunning(server))
    {
        // this will contains loop inside of it 
        Server_handlePackets(server);




        // old version 
        // while (Server_Receive(server))
        // {
            
        //     Server_handlePackets(server);
        // }
        

        
        // old version 
        // if (!getGameStart(server) && getClientCount(server) == 2)
        // {
        //     printf("Initializing game...\n");
            
        //     Server_sendInitState(server);
        //     setGameStart(server,1);

        //     printf("Game started!\n");
        // }

        // uppdate stuff like coll other 
        // if (getGameStart(server))
        // {
           
        // }
        SDL_Delay(16);
    }
    Destroy_Server(server);
}
