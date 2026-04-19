#include "game.h"


void run(Server server)
{
    while (getServerRunning(server))
    {
     
        while (Server_Receive(server))
        {
            
            Server_handlePackets(server);
        }

      
        if (!getGameStart(server) && getClientCount(server) == 1)
        {
            printf("Initializing game...\n");
            
             Server_sendInitState(server);
            setGameStart(server,1);

            printf("Game started!\n");
        }

        // uppdate stuff like coll other 
        if (getGameStart(server))
        {
           
        }
    }
}
