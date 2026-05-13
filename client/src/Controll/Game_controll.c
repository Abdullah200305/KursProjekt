#include "Game_controll.h"
#include "bombRelated.h"


/// This function will handle the main game loop, including event handling, updating game state, and rendering
void game_loop(Game *game, Renderer *renderer, ClientNet *clientNet)
{
    Uint32 lastSend = 0;
    const int SEND_RATE = 16;
    SDL_Event event;

    while (game->state == GAME_STATE_PLAYING)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    game->state = GAME_STATE_GAME_OVER;
                    game->running = 0;
                    break;
            }
        }

        // Receive update logic from server
        if (clientNet && *clientNet)
        {
            int receiveResult = ClientNet_TryReceive(*clientNet);

            if (receiveResult < 0 ||
                !ClientNet_IsConnected(*clientNet) ||
                ClientNet_HasTimedOut(*clientNet, 3000))
            {
                game_handle_server_disconnect(game, clientNet);
                return;
            }

            if (ClientNet_HasGameState(*clientNet))
            {
                game_apply_network_state(game, *clientNet);
            }
        }

        // Send input at fixed rate
        Uint32 now = SDL_GetTicks();

        if (clientNet && *clientNet && ClientNet_GetClientId(*clientNet) >= 0)
        {
            if (now - lastSend >= SEND_RATE)
            {
                InputPacket input = {0};
                const Uint8 *state = SDL_GetKeyboardState(NULL);

                input.type = PACKET_INPUT;
                input.clientId = ClientNet_GetClientId(*clientNet);
                input.up    = state[SDL_SCANCODE_W];
                input.down  = state[SDL_SCANCODE_S];
                input.left  = state[SDL_SCANCODE_A];
                input.right = state[SDL_SCANCODE_D];

                if (ClientNet_SendInput(*clientNet, &input) < 0)
                {
                    game_handle_server_disconnect(game, clientNet);
                    return;
                }

                lastSend = now;
            }
        }

        game_update(game, renderer);

        SDL_Delay(1);


        //MUSIK NÄR SPELET KÖRS
        static GameState lastState = -1;

        if (game->state != lastState) {
            if (game->state == GAME_STATE_PLAYING) Sound_PlayGameMusic(&game->sound);
            else if (game->state == GAME_STATE_GAME_OVER) Sound_StopMusic();
            lastState = game->state;
        }
    }
}

















void game_update(Game *game, Renderer *renderer)
{
    if (game == NULL || renderer == NULL)
    {
        return;
    }

    if (game->state != GAME_STATE_PLAYING)
    {
        return;
    }

    if (game->map == NULL ||
        game->bomb == NULL ||
        game->abilitySystem == NULL)
    {
        return;
    }

    for (int i = 0; i < game->numPlayers; i++)
    {
        if (game->players[i] == NULL)
        {
            return;
        }
    }



    // Render the game state
    Background_Image_Render(renderer);

    // this for test 
    //Render_Map(renderer, game->map);
 
  
   

   AbilitySystem_render(game->abilitySystem, renderer);
   for (int i = 0; i < game->numPlayers; i++)
    {
        setPlayerAnimation(game->players[i]);
    }

    for (int i = 0; i < game->numPlayers; i++)
    {
        if (isPlayerAlive(game->players[i]))
        {
            Render_Player(renderer, game->players[i],i);
        }
    }


    Render_Bomb(renderer, game->bomb);

    Render_ScreenFrame(renderer);
    










    // will uppdate
int windowW = 0;
int windowH = 0;
SDL_GetWindowSize(renderer->window, &windowW, &windowH);

Render_BombHUD(renderer, game->bomb, (windowW / 2) - 130, 20);
const int hudPanelW = 250;
const int hudPanelH = 74;
const int hudMargin = 20;


int hudPlayerCount = game->numPlayers;

if (hudPlayerCount > 4)
{
    hudPlayerCount = 4;
}

for (int i = 0; i < hudPlayerCount; i++)
{
    int hudX = 20;
    int hudY = 20;

    if (i == 0)
    {
        hudX = hudMargin;
        hudY = hudMargin;
    }
    else if (i == 1)
    {
        hudX = windowW - hudPanelW - hudMargin;
        hudY = hudMargin;
    }
    else if (i == 2)
    {
        hudX = hudMargin;
        hudY = windowH - hudPanelH - hudMargin;
    }
    else if (i == 3)
    {
        hudX = windowW - hudPanelW - hudMargin;
        hudY = windowH - hudPanelH - hudMargin;
    }

    if (game->players[i] != NULL && isPlayerAlive(game->players[i]))
    {
        int hasBomb = (i == getBombCarrier(game->bomb));

        Render_PlayerHUD(renderer, game->players[i], i, hudX, hudY, hasBomb);
    }
}


    int aliveCount = 0;

    for (int i = 0; i < game->numPlayers; i++)
    {
        if (isPlayerAlive(game->players[i]))
        {
            aliveCount++;
        }
    }

    if (game->state == GAME_STATE_PLAYING && game->numPlayers > 1 && aliveCount <= 1)
    {
        for (int i = 0; i < game->numPlayers; i++)
        {
            if (isPlayerAlive(game->players[i]))
                setPlayerWinner(game->players[i], 1);
        }
        game->state = GAME_STATE_GAME_OVER;
    }









    Renderer_Present(renderer);





 // here will be the miusic and sound effect logic
    // BOMB EXPLOSION SFX
    static int explosionPlayed = 0;

    if (getBombExploding(game->bomb)) {
        if (!explosionPlayed) {
            Sound_PlayExplosion(&game->sound);
            Sound_PlayScream(&game->sound);
            explosionPlayed = 1;
        }
    } else {
        explosionPlayed = 0;  // nollställ när explosion är klar
    }

    //FREEZE ABILITY SFX
    for (int i = 0; i < game->numPlayers; i++) {
        if (getPlayerFreezeTimer(game->players[i]) == 80) Sound_PlayIce(&game->sound);

        if (getPlayerFreezeTimer(game->players[i]) == 1) Sound_PlayIceBreak(&game->sound);
    }

    // RNG LAUGH SFX 
    static int RNG_laughTimer = 300;
    RNG_laughTimer--;
    if (RNG_laughTimer <= 0) {
        RNG_laughTimer = 300 + rand() % 600;

        if (rand() % 2 == 0)
            Sound_PlayLaugh1(&game->sound);
        else
            Sound_PlayLaugh2(&game->sound);
    }
}






// init stuff before game start like map, bomb and players
void game_init(Game *game, Renderer *renderer,ClientNet clientNet)
{
//10_000ms  mean 10 sec  will run game init otherwise re-join to server

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
}




// will clean all objects  
void game_cleanup(Game *game, Renderer *renderer)
{
    Map_destroy(game->map);
    Renderer_Destroy(renderer);
}

void game_reset_network_data(Game *game)
{
    if (game == NULL)
    {
        return;
    }

    for (int i = 0; i < Max_Players; i++)
    {
        if (game->players[i] != NULL)
        {
            PlayerDestroy(game->players[i]);
            game->players[i] = NULL;
        }
    }

    if (game->bomb != NULL)
    {
        destroyBomb(game->bomb);
        game->bomb = NULL;
    }

    if (game->abilitySystem != NULL)
    {
        AbilitySystem_destroy(game->abilitySystem);
        game->abilitySystem = NULL;
    }

    if (game->map != NULL)
    {
        Map_destroy(game->map);
        game->map = NULL;
    }

    game->numPlayers = 0;
    game->countdownValue = 0;
}

void game_handle_server_disconnect(Game *game, ClientNet *clientNet)
{
    printf("[CLIENT] Lost connection to server. Returning to menu.\n");

    game_reset_network_data(game);

    if (clientNet != NULL && *clientNet != NULL)
    {
        ClientNet_Destroy(*clientNet);
        *clientNet = NULL;
    }

    game->state = GAME_STATE_MENU;
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
        setPlayerFreezeTimer( game->players[i],packet.data.players[i].freezeTimer);  
        //  printf("x: %d y: %d\n", packet.data.players[i].x, packet.data.players[i].y);
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
    //game->state = GAME_STATE_PLAYING; // this will be change



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
