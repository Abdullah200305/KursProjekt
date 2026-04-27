#include "game.h"

void run(Server server,Game game)
{
Uint32 lastTick = SDL_GetTicks();
const int TICK_RATE = 16; // ~60 FPS

while (getServerRunning(server))
{
    Uint32 now = SDL_GetTicks();

    if (now - lastTick >= TICK_RATE)
    {
        Server_handlePackets(server);

        if (!getGameStart(server) && getClientCount(server) == 2) {
            Game_Init(server, &game);
            Game_InitSendToClients(server, &game);
            setGameStart(server, 1);
        }

        if (getGameStart(server))
        {
            Game_Update(server, &game);
        }

        lastTick = now;
    }





}
    server_disconnet(server);
    Map_destroy(game.map);
    Destroy_Server(server);
};










void Game_Init(Server server,Game *game){  
game->map = Map_create(WIDTH, HEIGHT);
game->state = GAME_STATE_PLAYING;
game->numPlayers = getClientCount(server);

// will be check after 
game->abilitySystem = AbilitySystem_create();
AbilitySystem_init(game->abilitySystem);
 
// PlayerManager_Init in server    
for (int  i = 0; i < game->numPlayers; i++)
{
   // game->players[i] = initPlayer(230+i*40, 300,getClientId(getClient(server,i)));  
   game->players[i] = initPlayer(230+i*40, 300);  
}

// BombManager_Init() in server
game->bomb = createBomb(game->players); 
printf("init game done\n");   
}


















// sutff inside it maybe will write some where else
void Game_Update(Server server, Game *game) {
    for (int i = 0; i < getClientCount(server); i++) {
        InputPacket in = getInputPlayer(server,i);
        int id =in.clientId;
     
        

 
        float vx = 0;
        float vy = 0;
       
        float speedX =  getPlayerSpeedX(game->players[i]);
        float speedY =  getPlayerSpeedY(game->players[i]);

        if (in.up)    vy = -speedY;
        if (in.down)  vy =  speedY;
        if (in.left)  vx = -speedX;
        if (in.right) vx =  speedX;

        setPlayerVelocity(game->players[i], vx, vy);

        // // Update player position based on velocity and check for collisions
        Player p = game->players[i];
        movePlayer(game->map, p);
        // update bomb 
        if (i == getBombCarrier(game->bomb))
           {
               movePlayerWithOther(
                   p,                      // current player
                   i,                      // index of current player
                   game->players,          // all players
                   game->numPlayers,       // count
                   game->bomb              // bomb pointer
               );
           }
    }
    updateBomb(game->bomb, game->players);


    //-------------------------------------------------------------------//
    //---------------------Ability Update Loop---------------------------//
    int miliseconds = 4000; // 2 seconds
    abilitySpawnRate(game->abilitySystem, game->map, miliseconds);
   
    int currentPlayers = 2;

    for (int i = 0; i < currentPlayers; i++)
    {
        AbilitySystem_checkPickup(game->abilitySystem, game->players[i], game->players, currentPlayers);

        if (getPlayerSpeedTimer(game->players[i]) > 0)
        {
            int speedTimer = getPlayerSpeedTimer(game->players[i]);
            setPlayerSpeedTimer(game->players[i], --speedTimer);

            if (getPlayerSpeedTimer(game->players[i]) == 0)
            {
                setPlayerSpeedYX(game->players[i], 5, 5);
            }
        }

        if (getPlayerFreezeTimer(game->players[i]) > 0)
        {
            int freezeTimer = getPlayerFreezeTimer(game->players[i]);
            setPlayerFreezeTimer(game->players[i], --freezeTimer);

            if (getPlayerFreezeTimer(game->players[i]) == 0)
            {
                setPlayerSpeedYX(game->players[i], 5, 5);
            }
        }

        if (getPlayerSizeUpTimer(game->players[i]) > 0)
        {
            int sizeUpTimer = getPlayerSizeUpTimer(game->players[i]);
            setPlayerSizeUpTimer(game->players[i], --sizeUpTimer);
            resolveCollisionRate(game->map, game->players[i], 1);

            if (getPlayerSizeUpTimer(game->players[i]) == 0)
            {
                setPlayerSize(game->players[i], 32, 32); 
            }
        }

        //Shield Ability Loses i ability.c + bombrelated.c 
    }



    
    GameStatePacket send;
    Packet_BuildGameState(&send,game);
    Server_Broadcast(server,&send,sizeof(GameStatePacket));
}

// maybe will move
void movePlayerWithOther(Player player, int p_index, Player players[], int count, Bomb bomb)
{
    for (int i = 0; i < count; i++)
    {
        Player other = players[i];

        if (player != other)
        {

             // why changed 
             if (Player_collisionWithOtherPlayer(
                getPlayerX(player),
                getPlayerY(player),
                getPlayerWidth(player),
                getPlayerHeight(player),

                getPlayerX(other),
                getPlayerY(other),
                getPlayerWidth(other),
                getPlayerHeight(other)))
            {
                int other_index = i;

               //printf("Collision between players detected!\n");

               // explain this 
                if (getBombCarrier(bomb) == p_index) {
                    setBombCarrier(bomb, other_index);

                    // uppdate
                    setPlayerSpeedYX(player, 6, 6);
                    setPlayerSpeedYX(other, 5, 5);
                   // printf("Bomb moved to player %d\n", other_index);
                }
                else if (getBombCarrier(bomb) == other_index) {
                    setBombCarrier(bomb, p_index);
                     // uppdate
                    setPlayerSpeedYX(other, 6, 6);
                    setPlayerSpeedYX(player, 5, 5);

                    // printf("Bomb moved to player %d\n", p_index);
                }
            }
        }
    }
}





// uppdate player position based on collision with the map and uppdate player position.
void movePlayer(Map map, Player player)
{
    float newX = getPlayerX(player) + getPlayerVelocityX(player);
    int colx = Collision_Map(map, newX,
        getPlayerY(player),
        getPlayerWidth(player),
        getPlayerHeight(player));
    if (colx == 1 || colx == 2) // if player outside of map will make somthing
    {
        /// collision with tile type 1 or 2, stop player movement
    }
    else
    {
       
        if (colx == 3)  // walk in sand 
        {
            setPlayerPosition(player, newX, getPlayerY(player));
            setPlayerSpeedYX(player, 3, 3);
        }
        else // walk in grass
        {
            setPlayerPosition(player, newX, getPlayerY(player));
            setPlayerSpeedYX(player, 5, 5);
        }
    }

    float newY = getPlayerY(player) + getPlayerVelocityY(player);
    int coly = Collision_Map(map, 
    getPlayerX(player), 
    newY,
    getPlayerWidth(player),
    getPlayerHeight(player));

    if (coly == 1 || coly == 2)
    {
        /// collision with tile type 1 or 2, stop player movement
    }
    else
    {
        if (coly == 3) 
        {
            setPlayerPosition(player, getPlayerX(player), newY);
            setPlayerSpeedYX(player, 3, 3);
        }
        else
        {
            setPlayerPosition(player, getPlayerX(player), newY);
            setPlayerSpeedYX(player, 5, 5);
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
    memset(packet, 0, packetSize);
}






























