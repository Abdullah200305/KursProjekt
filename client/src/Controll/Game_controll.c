#include "Game_controll.h"

/// This function will handle the main game loop, including event handling, updating game state, and rendering
void game_loop(Game *game, Renderer *renderer, ClientNet clientNet)
{
Uint32 lastSend = 0;
const int SEND_RATE = 16;

while (game->state != GAME_STATE_GAME_OVER)
{

    // this will moved to input.c file
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            game->state = GAME_STATE_GAME_OVER;
    }

   
    // this for recive uppdate logic from server only 
    if (clientNet)
    {
        ClientNet_TryReceive(clientNet);
        if (ClientNet_HasGameState(clientNet))
            game_apply_network_state(game, clientNet);  
    }


    // this will update after to put it in input files insteade for here && moved to input.c file
    // send input at fixed rate
    Uint32 now = SDL_GetTicks();
    if (clientNet && ClientNet_GetClientId(clientNet) >= 0)
    {
        if (now - lastSend >= SEND_RATE)
        {
            InputPacket input = {0};
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            input.type = PACKET_INPUT;
            input.clientId = ClientNet_GetClientId(clientNet);
            input.up    = state[SDL_SCANCODE_W];
            input.down  = state[SDL_SCANCODE_S];
            input.left  = state[SDL_SCANCODE_A];
            input.right = state[SDL_SCANCODE_D];
            ClientNet_SendInput(clientNet, &input);
            lastSend = now;
        }
    };

    game_update(game, renderer); // it will make only render
    SDL_Delay(1); 
}

    

//will updte
const char *message = "Game over! Click OK to close.";
if (isPlayerAlive(game->players[0]) && !isPlayerAlive(game->players[1]))
{
    message = "Game over!\nPlayer 1 wins.\nClick OK to close.";
}
else if (isPlayerAlive(game->players[1]) && !isPlayerAlive(game->players[0]))
{
    message = "Game over!\nPlayer 2 wins.\nClick OK to close.";
}
else if (!isPlayerAlive(game->players[0]) && !isPlayerAlive(game->players[1]))
{
    message = "Game over!\nBoth players died.\nClick OK to close.";
}
SDL_ShowSimpleMessageBox(
    SDL_MESSAGEBOX_INFORMATION,
    "Game Over",
    message,
    renderer->window
);

}






void game_update(Game *game, Renderer *renderer)
{
    // Render the game state
    Background_Image_Render(renderer);

    // this for test 
    //Render_Map(renderer, game->map);

  
   

    AbilitySystem_render(game->abilitySystem, renderer);


    for (int i = 0; i < game->numPlayers; i++)
    {
        if (isPlayerAlive(game->players[i]))
        {
            Render_Player(renderer, game->players[i],i);
            setPlayerAnimation(game->players[i]);
        }
    }


    Render_Bomb(renderer, game->bomb);


    // this will uppdate
    if (isPlayerAlive(game->players[0]))
    {
        Render_PlayerLives(renderer, game->players[0], 20, 20);
    }

    
    if (isPlayerAlive(game->players[1]))
    {
        Render_PlayerLives(renderer, game->players[1], 20, 50);
    }

    int aliveCount = 0;

    for (int i = 0; i < game->numPlayers; i++)
    {
        if (isPlayerAlive(game->players[i]))
        {
            aliveCount++;
        }
    }

    if (aliveCount <= 1)
    {
        game->state = GAME_STATE_GAME_OVER;
    }





    Renderer_Present(renderer);
}


// init stuff before game start like map, bomb and players
void game_init(Game *game, Renderer *renderer,ClientNet clientNet)
{
// 10_000ms  mean 10 sec  will run game init otherwise re-join to server
const Uint32 timeout = SDL_GetTicks() + 10000; 
int game_init = -1;
while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
    if (clientNet)
    {
        ClientNet_TryReceive(clientNet);
        if (ClientNet_HasGameInit(clientNet)){
          game_init = game_apply_network_init(game, clientNet);
        }
          
        if (game_init>=0)
        {
            printf("Game_init have been stablished\n");
            break;
        }
    }
}
if(game_init<0){
printf("failed to make game_init try again!!\n");
return;
}
Renderer_Init(renderer, "Hello, World!", getWidth(game->map), getHeight(game->map));
}




// will clean all objects  
void game_cleanup(Game *game, Renderer *renderer)
{
    Map_destroy(game->map);
    Renderer_Destroy(renderer);
}



////////////////////////// these functions will handle the depacket and init and game_init /////////////////////////////

// here will packet open and make uppdate 
void game_apply_network_state(Game *game, ClientNet clientNet)
{
    GameStatePacket packet;
    int playersToUpdate;
    if (game == NULL || clientNet == NULL) {
        return;
    }

    if (!ClientNet_HasGameState(clientNet)) {
        return;
    }

    packet = ClientNet_GetGameStatePacket(clientNet);

    playersToUpdate = packet.data.numPlayers;

    if (playersToUpdate > game->numPlayers) {
        playersToUpdate = game->numPlayers;
    }

    if (playersToUpdate > MAX_PLAYERS) {
        playersToUpdate = MAX_PLAYERS;
    }

        for (int i = 0; i < playersToUpdate; i++) {

            
        setPlayerPosition(
            game->players[i],
            packet.data.players[i].x,
            packet.data.players[i].y
        );

        setPlayerVelocity(game->players[i],
            packet.data.players[i].vx,
            packet.data.players[i].vy);

        setPlayerSize(game->players[i],
            packet.data.players[i].width,
            packet.data.players[i].height); 
      
        setPlayerState(
            game->players[i],
            packet.data.players[i].lives,
            packet.data.players[i].alive
        );
        //printf("%d livesss %d\n",i,getPlayerLives(game->players[i]));
       
    }
    
    setBombState(
        game->bomb,
        packet.data.bomb.x,
        packet.data.bomb.y,
        packet.data.bomb.timer,
        packet.data.bomb.bombCarrier,
        packet.data.bomb.active,
        packet.data.bomb.exploding
    );
   
    for (int i = 0; i < packet.data.abilities.numAbilities; i++)
    {  


    SetAbilityItemState(
        game->abilitySystem,
        i,
        packet.data.abilities.items[i].type,
        packet.data.abilities.items[i].x,
        packet.data.abilities.items[i].y,
        packet.data.abilities.items[i].width,
        packet.data.abilities.items[i].height,
        packet.data.abilities.items[i].active
    );
    }
   // printf("[CLIENT] Applied GAME_STATE positions locally\n");
    ClientNet_ClearGameState(clientNet);
}


int game_apply_network_init(Game *game, ClientNet clientNet)
{
    GameInitPacket packet;
    if (game == NULL || clientNet == NULL) {
        return -1;
    }
    if (!ClientNet_HasGameInit(clientNet)) {
        return -1;
    }
    packet = ClientNet_GetGameInitPacket(clientNet);


    // this will be change
    if (packet.data.map.mapId != MAP_ID_ISLAND) {
        printf("[CLIENT] Unknown mapId: %d\n", packet.data.map.mapId);
        ClientNet_ClearGameInit(clientNet);
        return -1;
    }
   

    game->map = Map_create(packet.data.map.width, packet.data.map.height);
    game->state = GAME_STATE_PLAYING; // this will be change

    game->numPlayers = packet.data.numPlayers;
    for (int i = 0; i < game->numPlayers; i++)
    {
        //  player init will update after 
        game->players[i] = initPlayer(packet.data.players[i].x,packet.data.players[i].y);
    }
    game->bomb = createBomb(game->players);

    
    game->abilitySystem = AbilitySystem_create();
    AbilitySystem_init(game->abilitySystem); 

    


    // will remove
    printf("%d server \n",packet.data.yourClientId);
    // this is id for server not any player
    //ClientNet_SetClientId(clientNet, packet.data.yourClientId);
    ClientNet_ClearGameInit(clientNet);
    printf("[CLIENT] Applied GAME_INIT locally\n");
    return 0;
}













































