#include "game.h"


void run(Server server)
{
    while (getServerRunning(server))
    {
     
        while (Server_Receive(server)){
        Server_handlePackets(server);
        }

       // to inital the game state only like map player and bomb from start 
        if (!getGameStart(server) && getClientCount(server) == 1)
        {
            printf("Initializing game...\n");
        
            Server_sendInitState(server);
            setGameStart(server,1);
            printf("Game started!\n");
        }

        // uppdate stuff like like input packet will recive
        if (getGameStart(server))
        {
           
        }
    }
}