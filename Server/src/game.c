#include "game.h"


void run(Server server,Game game)
{
    while (getServerRunning(server))
    {
        Server_handlePackets(server);


 
        // before game start
        if (!getGameStart(server) && getClientCount(server) == 1) {
        printf("Initializing game...\n");
        Game_Init(server, &game);
        Game_InitSendToClients(server, &game);
        setGameStart(server, 1);
        printf("Game started!\n");
        }       


        //uppdate game logic
        if (getGameStart(server))
        {
         Game_Update(server,&game);
        }
        SDL_Delay(16);
    }
    server_disconnet(server);

    Map_destroy(game.map);
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
   // game->players[i] = initPlayer(230+i*40, 300,getClientId(getClient(server,i)));  
   game->players[i] = initPlayer(230+i*40, 300);  
}

// BombManager_Init() in server
game->bomb = createBomb(game->players);    
}


















void Game_Update(Server server, Game *game) {
    for (int i = 0; i < getClientCount(server); i++) {
        InputPacket in = getInputPlayer(server,i);
        int id =in.clientId;
        //printf("Player %d input: U:%d D:%d L:%d R:%d\n",
          //      i, in.up, in.down, in.left, in.right);

        stopPlayer(game->players[i]);
        if (in.up)    setPlayerVelocity(game->players[i], getPlayerVelocityX(game->players[i]), -5.0);
        if (in.down)  setPlayerVelocity(game->players[i], getPlayerVelocityX(game->players[i]),  5.0);
        if (in.left)  setPlayerVelocity(game->players[i], -5.0, getPlayerVelocityY(game->players[i]));
        if (in.right) setPlayerVelocity(game->players[i],  5.0, getPlayerVelocityY(game->players[i]));
        setPlayerPosition(
            game->players[i],
            getPlayerX(game->players[i]) + getPlayerVelocityX(game->players[i]),
            getPlayerY(game->players[i]) + getPlayerVelocityY(game->players[i])
        );




        printf("Player %d position: x=%.2f y=%.2f\n\n",
        i,
        getPlayerX(game->players[i]),
        getPlayerY(game->players[i]));






        // // Update player position based on velocity and check for collisions
        Player p = game->players[i];
        movePlayer(game->map, p);
        if (i == getBombCarrier(game->bomb))
           {
               movePlayerWithOther(
                   p,                      // current player
                   i,                      // index of current player
                   game->players,          // all players
                   game->numPlayers,       // count
                   game->bomb              // bomb pointer
               );
               //movePlayerWithOther(p, game->players, game->numPlayers, &game->bomb);
           }
    } 




    GameStatePacket send;
    Packet_BuildGameState(&send,game);
    Server_Broadcast(server,&send,sizeof(GameStatePacket));
}



void movePlayerWithOther(Player player, int p_index, Player players[], int count, Bomb bomb)
{
    for (int i = 0; i < count; i++)
    {
        Player other = players[i];

        if (player != other)
        {
            if (Player_collisionWithOtherPlayer(
                    getPlayerX(player),
                    getPlayerY(player),
                    getPlayerX(other),
                    getPlayerY(other)))
            {
                int other_index = i;

                printf("Collision between players detected!\n");

                if (getBombCarrier(bomb) == p_index) {
                    setBombCarrier(bomb, other_index);

                    printf("Bomb moved to player %d\n", other_index);
                }
                else if (getBombCarrier(bomb) == other_index) {
                    setBombCarrier(bomb, p_index);

                    printf("Bomb moved to player %d\n", p_index);
                }
            }
        }
    }
}

// void movePlayerWithOther(Player player, Player players[], int count, Bomb *bomb)
// {
//
//     for (int i = 0; i < count; i++)
//     {
//         Player other = players[i];
//
//         if (player != other)
//         {
//
//             if (Player_collisionWithOtherPlayer(getPlayerX(player),getPlayerY(player),(int)getPlayerX(other), (int)getPlayerY(other)))
//             {    // will make that bomb will go to the other player
//                 int p_index     = (int)(player - players);
//                 int other_index = i;
//                 printf("Collision between players detected!\n");
//                 if(bomb->bombCarrier == p_index){
//                     bomb->bombCarrier = other_index;
//                     printf("Bomb carrier changed to player %d\n", other_index);
//                 }
//                 else if (bomb->bombCarrier == other_index) {
//                     bomb->bombCarrier = p_index;
//                     printf("Bomb carrier changed to player %d\n", p_index);
//                 }
//             }
//         }
//     }
// }


// uppdate player position based on collision with the map and uppdate player position.
void movePlayer(Map map, Player player)
{
    float newX = getPlayerX(player) + getPlayerVelocityX(player);
    int colx = Collision_Map(map, newX, getPlayerY(player));
    if (colx == 1 || colx == 2)
    {
        /// collision with tile type 1 or 2, stop player movement
    }
    else
    {
        if (colx == 3) // I will update so here will move slow
        {
            printf("Collision with tile type 3 at (%d, %d)\n",
                   (int)(newX / getTileSize(map)),
                   (int)(getPlayerY(player) / getTileSize(map)));
            setPlayerPosition(player, newX, getPlayerY(player));
        }
        else
        {
            setPlayerPosition(player, newX, getPlayerY(player));
        }
    }

    float newY = getPlayerY(player) + getPlayerVelocityY(player);
    int coly = Collision_Map(map, getPlayerX(player), newY);
    if (coly == 1 || coly == 2)
    {
        /// collision with tile type 1 or 2, stop player movement
    }
    else
    {
        if (coly == 3) // I will update so here will move slow
        {
            printf("Collision with tile type 3 at (%d, %d)\n",
                   (int)(newX / getTileSize(map)),
                   (int)(getPlayerY(player) / getTileSize(map)));
            setPlayerPosition(player, getPlayerX(player), newY);
        }
        else
        {
            setPlayerPosition(player, getPlayerX(player), newY);
        }
    }
}















void Game_InitSendToClients(Server server, Game *game) {
    if (!game || !game->map) {
        printf("ERROR: Game not properly initialized!\n");
        return;
    }

    GameInitPacket packet;
    memset(&packet, 0, sizeof(GameInitPacket)); // zero before building
    Packet_BuildGameStart(game, 7777, &packet);

    printf("Sending GameInitPacket of size %zu\n", sizeof(GameInitPacket));
    Server_Broadcast(server, &packet, sizeof(GameInitPacket));
}




void server_disconnet(Server server){
    DisconnectPacket packet;
    Packet_BuildDisconnect(7777,&packet);
    Server_Broadcast(server,&packet, sizeof(packet)); 
}




void Server_Broadcast(Server server, void *packet, size_t packetSize) {
    for (int i = 0; i < getClientCount(server); i++) {
        Client c = getClient(server, i);
        if (getActive(c)) {
            //printf("Broadcasting to client %d\n", getClientId(c));
            Server_Send(server, Client_GetAddress(c), packet, packetSize);
           
        }
    }
    //printf("Broadcast sent to %d clients\n", sent);
    memset(packet, 0, packetSize);
}