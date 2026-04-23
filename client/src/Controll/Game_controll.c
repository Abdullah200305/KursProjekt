#include "Game_controll.h"
#include "bombRelated.h"
#include "client_net.h"
#include <stdio.h>
#include <string.h>

/// This function will handle the main game loop, including event handling, updating game state, and rendering
void game_loop(Game *game, Renderer *renderer, ClientNet clientNet)
{
    // event will be uppdate
    SDL_Event event;
    while (game->state != GAME_STATE_GAME_OVER)
    {

        // this will be uppdate
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                game->state = GAME_STATE_GAME_OVER;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_h)
                {
                    damagePlayer(game->players[0]);
                }
                break;
            }
        }

        // ttry to connect
        // if (clientNet != NULL)
        // {
        //     int receiveResult = ClientNet_TryReceive(clientNet);

        //     if (receiveResult < 0)
        //     {
        //         printf("[CLIENT] Receive check failed\n");
        //     }

        //     if (ClientNet_HasGameInit(clientNet))
        //     {
        //         game_apply_network_init(game, clientNet);
        //     }
        // }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (clientNet != NULL && ClientNet_GetClientId(clientNet) >= 0)
        {
            InputPacket inputPacket;
            memset(&inputPacket, 0, sizeof(InputPacket));

            inputPacket.type = PACKET_INPUT;
            inputPacket.clientId = ClientNet_GetClientId(clientNet);
            inputPacket.up = state[SDL_SCANCODE_W] ? 1 : 0;
            inputPacket.down = state[SDL_SCANCODE_S] ? 1 : 0;
            inputPacket.left = state[SDL_SCANCODE_A] ? 1 : 0;
            inputPacket.right = state[SDL_SCANCODE_D] ? 1 : 0;
            if (ClientNet_SendInput(clientNet, &inputPacket) < 0)
            {
                printf("[CLIENT] Failed to send input packet\n");
            }
        }
        game_update(game, renderer);
        SDL_Delay(16); // Delay to cap the frame rate (approximately 60 FPS)

    }
    







    // const char *message = "Game over! Click OK to close.";

    // if (isPlayerAlive(game->players[0]) && !isPlayerAlive(game->players[1]))
    // {
    //     message = "Game over!\nPlayer 1 wins.\nClick OK to close.";
    // }
    // else if (isPlayerAlive(game->players[1]) && !isPlayerAlive(game->players[0]))
    // {
    //     message = "Game over!\nPlayer 2 wins.\nClick OK to close.";
    // }
    // else if (!isPlayerAlive(game->players[0]) && !isPlayerAlive(game->players[1]))
    // {
    //     message = "Game over!\nBoth players died.\nClick OK to close.";
    // }

    // SDL_ShowSimpleMessageBox(
    //     SDL_MESSAGEBOX_INFORMATION,
    //     "Game Over",
    //     message,
    //     renderer->window
    // );
}

void game_update(Game *game, Renderer *renderer)
{

    // Update player position based on velocity and check for collisions
    // for (int i = 0; i < game->numPlayers; i++)
    // {
    //     Player p = game->players[i];

    //     movePlayer(game->map, p);

    //     // if (i == getBombCarrier(game->bomb))
    //     // {
    //     //     movePlayerWithOther(
    //     //         p,                      // current player
    //     //         i,                      // index of current player
    //     //         game->players,          // all players
    //     //         game->numPlayers,       // count
    //     //         game->bomb              // bomb pointer
    //     //     );
    //     //     //movePlayerWithOther(p, game->players, game->numPlayers, &game->bomb);
    //     // }
    // }
    // updateBomb(game->bomb, game->players);




    // will remove
    //Render_Map(renderer, game->map);








    // Render the game state
    Background_Image_Render(renderer);
    // renderer
    for (int i = 0; i < game->numPlayers; i++)
    {
        if (isPlayerAlive(game->players[i]))
        {
            Render_Player(renderer, game->players[i]);
        }
    }
    // Render_Bomb(renderer, game->bomb);

    // if (isPlayerAlive(game->players[0]))
    // {
    //     Render_PlayerLives(renderer, game->players[0], 20, 20);
    // }

    // if (isPlayerAlive(game->players[1]))
    // {
    //     Render_PlayerLives(renderer, game->players[1], 20, 50);
    // }
    // int aliveCount = 0;
    // for (int i = 0; i < game->numPlayers; i++)
    // {
    //     if (isPlayerAlive(game->players[i]))
    //     {
    //         aliveCount++;
    //     }
    // }
    // if (aliveCount <= 1)
    // {
    //     game->state = GAME_STATE_GAME_OVER;
    // }

    Renderer_Present(renderer);
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

                if (getBombCarrier(bomb) == p_index)
                {
                    setBombCarrier(bomb, other_index);

                    printf("Bomb moved to player %d\n", other_index);
                }
                else if (getBombCarrier(bomb) == other_index)
                {
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

// void movePlayer(Map map, Player player)
// {
//     float newX = getPlayerX(player) + getPlayerVelocityX(player);
//     int colx = Collision_Map(map, newX, getPlayerY(player));
//     if (colx == 1 || colx == 2)
//     {
//         /// collision with tile type 1 or 2, stop player movement
//     }
//     else
//     {
//         if (colx == 3) // I will update so here will move slow
//         {
//             printf("Collision with tile type 3 at (%d, %d)\n",
//                    (int)(newX / getTileSize(map)),
//                    (int)(getPlayerY(player) / getTileSize(map)));
//             setPlayerPosition(player, newX, getPlayerY(player));
//         }
//         else
//         {
//             setPlayerPosition(player, newX, getPlayerY(player));
//         }
//     }

//     float newY = getPlayerY(player) + getPlayerVelocityY(player);
//     int coly = Collision_Map(map, getPlayerX(player), newY);
//     if (coly == 1 || coly == 2)
//     {
//         /// collision with tile type 1 or 2, stop player movement
//     }
//     else
//     {
//         if (coly == 3) // I will update so here will move slow
//         {
//             printf("Collision with tile type 3 at (%d, %d)\n",
//                    (int)(newX / getTileSize(map)),
//                    (int)(getPlayerY(player) / getTileSize(map)));
//             setPlayerPosition(player, getPlayerX(player), newY);
//         }
//         else
//         {
//             setPlayerPosition(player, getPlayerX(player), newY);
//         }
//     }
// }

void movePlayer(Map map, Player player)
{
    float newX = getPlayerX(player) + getPlayerVelocityX(player);
    float newY = getPlayerY(player) + getPlayerVelocityY(player);
    setPlayerPosition(player, newX, newY);
}




void game_cleanup(Game *game, Renderer *renderer)
{
    Map_destroy(game->map);
    Renderer_Destroy(renderer);
}



int game_apply_network_init(Game *game, ClientNet clientNet)
{
    if (game == NULL || clientNet == NULL)
        return -1;
    if (!ClientNet_HasGameInit(clientNet))
        return -1;

    GameInitPacket packet = ClientNet_GetGameInitPacket(clientNet);

    // Validate BEFORE mutating game state
    if (packet.data.map.mapId != MAP_ID_ISLAND)
    {
        printf("[CLIENT] Unknown mapId: %d\n", packet.data.map.mapId);
        ClientNet_ClearGameInit(clientNet);
        return -1;
    }

    int numPlayers = packet.data.numPlayers;
    if (numPlayers > MAX_PLAYERS)
        numPlayers = MAX_PLAYERS;

    game->map = Map_create(packet.data.map.width, packet.data.map.height);
    if (game->map == NULL)
    {
        ClientNet_ClearGameInit(clientNet);
        return -1;
    }

    // All good — now apply state
    game->state = GAME_STATE_PLAYING;
    game->numPlayers = numPlayers;

    for (int i = 0; i < game->numPlayers; i++)
    {
        game->players[i] = initPlayer(
            (int)packet.data.players[i].x,
            (int)packet.data.players[i].y);
    }
    game->bomb = createBomb(game->players);

    ClientNet_ClearGameInit(clientNet);
    printf("[CLIENT] Applied GAME_INIT locally\n");
    return 0;
}

// int game_apply_network_init(Game *game, ClientNet clientNet)
// {
//     GameInitPacket packet;

//     if (game == NULL || clientNet == NULL)
//     {
//         return -1;
//     }

//     if (!ClientNet_HasGameInit(clientNet))
//     {
//         return -1;
//     }

//     packet = ClientNet_GetGameInitPacket(clientNet);
//     game->map = Map_create(packet.data.map.width, packet.data.map.height);
//     game->state = GAME_STATE_PLAYING;
//     game->numPlayers = packet.data.numPlayers;

//     if (packet.data.map.mapId != MAP_ID_ISLAND)
//     {
//         printf("[CLIENT] Unknown mapId: %d\n", packet.data.map.mapId);
//         ClientNet_ClearGameInit(clientNet);
//         return -1;
//     }

//     game->numPlayers = packet.data.numPlayers;
//     if (game->numPlayers > MAX_PLAYERS)
//     {
//         game->numPlayers = MAX_PLAYERS;
//     }

//     for (int i = 0; i < game->numPlayers; i++)
//     {
//         game->players[i] = initPlayer(
//             (int)packet.data.players[i].x,
//             (int)packet.data.players[i].y);
//     }
//     game->bomb = createBomb(game->players);

//     // ClientNet_SetClientId(clientNet, packet.data.yourClientId);
//     ClientNet_ClearGameInit(clientNet);
//     printf("[CLIENT] Applied GAME_INIT locally\n");
//     return 0;
// }

//
void game_init(Game *game, Renderer *renderer, ClientNet clientNet)
{
    int game_init = -1;
    Uint32 startTime = SDL_GetTicks();
    Uint32 timeout = 30000;

    while (game_init < 0)
    {
        if (SDL_GetTicks() - startTime > timeout)
        {
            printf("[CLIENT] Game init timed out after 30 seconds\n");
            return;
        }
        if (clientNet != NULL)
        {
            int receiveResult = ClientNet_TryReceive(clientNet);
            if (receiveResult < 0)
            {
                printf("[CLIENT] Receive check failed\n");
            }
            if (ClientNet_HasGameInit(clientNet))
            {
                game_init = game_apply_network_init(game, clientNet);
            }
        }
        SDL_Delay(10);
    }

    Renderer_Init(renderer, "Hello, World!", getWidth(game->map), getHeight(game->map));

    // will be change after
    // printf("%d hello ",getPlayerX(game->players[0]));
    // game->bomb.active = 1;
    // game->bomb.timer = 200;
    // game->bomb.bombCarrier = 0;
    // game->bomb.exploding = 0;
    // game->bomb.explosionTimer = 0;
    // game->bomb.x = getPlayerX(game->players[0]);
    // game->bomb.y = getPlayerY(game->players[0]);
}