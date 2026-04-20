#include "Game_controll.h"
#include "bombRelated.h"
#include "Ability.h"

/// This function will handle the main game loop, including event handling, updating game state, and rendering
void game_loop(Game *game, Renderer *renderer)
{
    SDL_Event event;
    while (game->state != GAME_STATE_GAME_OVER)
    {
        // THis will need to be change after
        /// start

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
        // you can move both payers at the same time, but you can only move one player at a time.
        //this for the test.
       // PLAYER 1

        playerMovement(game->players[0], SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);
        playerMovement(game->players[1], SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);


        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if(state[SDL_SCANCODE_M])
        {
            setPlayerSpeedYX(game->players[0], 50, 34);
        }

        //old implementation of movement
       /*
        const Uint8 *state = SDL_GetKeyboardState(NULL);


        stopPlayer(game->players[0]);
        if (state[SDL_SCANCODE_W])
            setPlayerVelocity(game->players[0], getPlayerVelocityX(game->players[0]), -5.0);
        if (state[SDL_SCANCODE_S])
            setPlayerVelocity(game->players[0], getPlayerVelocityX(game->players[0]), 5.0);
        if (state[SDL_SCANCODE_A])
            setPlayerVelocity(game->players[0], -5.0, getPlayerVelocityY(game->players[0]));
        if (state[SDL_SCANCODE_D])
            setPlayerVelocity(game->players[0], 5.0, getPlayerVelocityY(game->players[0]));
        
        // PLAYER 2
        stopPlayer(game->players[1]);
        if (state[SDL_SCANCODE_UP])
            setPlayerVelocity(game->players[1], getPlayerVelocityX(game->players[1]), -5.0);
        if (state[SDL_SCANCODE_DOWN])
            setPlayerVelocity(game->players[1], getPlayerVelocityX(game->players[1]), 5.0);
        if (state[SDL_SCANCODE_LEFT])
            setPlayerVelocity(game->players[1], -5.0, getPlayerVelocityY(game->players[1]));
        if (state[SDL_SCANCODE_RIGHT])
            setPlayerVelocity(game->players[1], 5.0, getPlayerVelocityY(game->players[1]));
        */

        game_update(game, renderer);
        SDL_Delay(16); // Delay to cap the frame rate (approximately 60 FPS)
    }





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
    // // Update player position based on velocity and check for collisions
    for (int i = 0; i < game->numPlayers; i++)
    {
        Player p = game->players[i];
        
        movePlayer(game->map, p);

        if (i == getBombCarrier(game->bomb))
        {
             movePlayerWithOther(
                p,                      // current player
                i,                      // index of current player
                game->players,          // all players
                game->numPlayers,       // count
                game->bomb             // bomb pointer
            );
            //movePlayerWithOther(p, game->players, game->numPlayers, &game->bomb);
        }
    }

    updateBomb(game->bomb, game->players);












    // Render the game state
    Background_Image_Render(renderer);
    Render_Map(renderer, game->map);
   





    AbilitySystem_render(game->abilitySystem, renderer);

    int currentPlayers = 2;

    for (int i = 0; i < currentPlayers; i++)
    {
        AbilitySystem_checkPickup(game->abilitySystem, game->players[i]);
    }

    for (int i = 0; i < currentPlayers; i++)
    {
        if (getPlayerSpeedTimer(game->players[i]) > 0)
        {
            int speedTimer = getPlayerSpeedTimer(game->players[i]);
            setPlayerSpeedTimer(game->players[i], --speedTimer);

            if (getPlayerSpeedTimer(game->players[i]) == 0)
            {
                setPlayerSpeedYX(game->players[i], 5, 5);
            }
        }
    }    


    for (int i = 0; i < game->numPlayers; i++)
    {
        if (isPlayerAlive(game->players[i]))
        {
            Render_Player(renderer, game->players[i],i);
        }
    }
    Render_Bomb(renderer, game->bomb);


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
                    setBombCarrier(bomb,other_index);
                
                    printf("Bomb moved to player %d\n", other_index);
                }
                else if (getBombCarrier(bomb) == other_index) {
                    setBombCarrier(bomb,p_index);
                    
                    printf("Bomb moved to player %d\n", p_index);
                }
            }
        }
    }
}





// void movePlayerWithOther(Player player, Player players[], int count, Bomb *bomb)
// {
   
//     for (int i = 0; i < count; i++)
//     {
//         Player other = players[i];

//         if (player != other)
//         {
           
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
            printf("Collision with tile type 3 at (%d, %d)\n", (int)(newX / getTileSize(map)), (int)(getPlayerY(player) / getTileSize(map)));
            setPlayerPosition(player, newX,getPlayerY(player));
        }
        else
        {
            setPlayerPosition(player, newX,getPlayerY(player));
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
            printf("Collision with tile type 3 at (%d, %d)\n", (int)(newX /getTileSize(map)), (int)(getPlayerY(player) / getTileSize(map)));
            setPlayerPosition(player, getPlayerX(player), newY);
        }
        else
        {
            setPlayerPosition(player,getPlayerX(player), newY);
        }
    }
}

void game_cleanup(Game *game, Renderer *renderer)
{
    Map_destroy(game->map);
    Renderer_Destroy(renderer);
    AbilitySystem_destroy(game->abilitySystem);
}


void game_init(Game *game, Renderer *renderer)
{
    game->map = Map_create(WIDTH, HEIGHT);
    game->state = GAME_STATE_PLAYING;
    game->numPlayers = 2;
    
    game->abilitySystem = AbilitySystem_create();
    AbilitySystem_init(game->abilitySystem);
    AbilitySystem_spawn(game->abilitySystem, game->map);
    

    game->players[0] = initPlayer(230, 300);
    game->players[1] = initPlayer(270, 300);


    game->bomb = createBomb(game->players);
      
        // printf("%d hello ",getPlayerX(game->players[0]));
        // game->bomb.active = 1;
        // game->bomb.timer = 200;
        // game->bomb.bombCarrier = 0;
        // game->bomb.exploding = 0;
        // game->bomb.explosionTimer = 0;
        // game->bomb.x = getPlayerX(game->players[0]);
        // game->bomb.y = getPlayerY(game->players[0]);


  
    Renderer_Init(renderer, "Hello, World!", getWidth(game->map), getHeight(game->map)); // will be update to be as ADT


    
       
}