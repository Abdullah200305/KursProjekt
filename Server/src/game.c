#include "game.h"


void run(Server server,Game game)
{
    while (getServerRunning(server))
    {
        // this will contains loop inside of it 
        Server_handlePackets(server);


        /// this will uppdate after make some secure that all player get the game_init otherwise resend to them.
        // this will create onec both in client and server side before game start
        if (!getGameStart(server) && getClientCount(server) == 2)
        {
        printf("Initializing game...\n");    
        Game_Init(server,&game);
        Game_InitSendToClients(server,&game);
        setGameStart(server,1);
        printf("Game started!\n");
        }


        //uppdate game logic
        if (getGameStart(server))
        {
           
        }
        SDL_Delay(16);
    }
   
    Destroy_Server(server);
};



void Game_Init(Server server,Game *game){
// Map_init in server  Map_Init()   
game->map = Map_create(WIDTH, HEIGHT);
game->state = GAME_STATE_PLAYING;
game->numPlayers = getClientCount(server);
    

// will be check after 
    // game->abilitySystem = AbilitySystem_create();
    // AbilitySystem_init(game->abilitySystem);
    // AbilitySystem_spawn(game->abilitySystem, game->map);


// PlayerManager_Init in server    
for (int  i = 0; i < game->numPlayers; i++)
{
    game->players[i] = initPlayer(230+i*40, 300,getClientId(getClient(server,i)));  
}

// BombManager_Init() in server
game->bomb = createBomb(game->players);    
}
void Game_InitSendToClients(Server server,Game *game){
Packet packet;
Packet_BuildGameStart(game,7777,&packet);
Server_Broadcast(server, &packet);  
};



// this will uppdate for all players
void Server_Broadcast(Server server,Packet *packet){
for (int i = 0; i < getClientCount(server); i++)
{
    Server_Send(server,
                Client_GetAddress(getClient(server,i)),
                packet,
                sizeof(Packet));
}
    memset(packet, 0, sizeof(Packet));
}
