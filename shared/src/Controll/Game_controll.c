#include "..\..\Include\Game_controll.h"

/// This function will handle the main game loop, including event handling, updating game state, and rendering
void game_loop(Game *game, Renderer *renderer,Event *event)
{
    // SDL_Event a;

    printf("%d Input initialized.\n", event->input->quit);
    while (game->state != GAME_STATE_GAME_OVER)
    {
        // THis will need to be change after
        /// start
        stopPlayer(&game->players[0]);
        Input_HandleEvents(event->input);
        Player_handleInput(game, event->input);
      













        // const Uint8 *state = SDL_GetKeyboardState(NULL);

        // // /// you can move both payers at the same time, but you can only move one player at a time.
        // // // this for the test.
        // // // PLAYER 1
      
        // if (state[SDL_SCANCODE_W])
        //     setPlayerVelocity(&game->players[0], getPlayerVelocityX(&game->players[0]), -5.0);
        // if (state[SDL_SCANCODE_S])
        //     setPlayerVelocity(&game->players[0], getPlayerVelocityX(&game->players[0]), 5.0);
        // if (state[SDL_SCANCODE_A])
        //     setPlayerVelocity(&game->players[0], -5.0, getPlayerVelocityY(&game->players[0]));
        // if (state[SDL_SCANCODE_D])
        //     setPlayerVelocity(&game->players[0], 5.0, getPlayerVelocityY(&game->players[0]));

        // // PLAYER 2
        // stopPlayer(&game->players[1]);
        // if (state[SDL_SCANCODE_UP])
        //     setPlayerVelocity(&game->players[1], getPlayerVelocityX(&game->players[1]), -5.0);
        // if (state[SDL_SCANCODE_DOWN])
        //     setPlayerVelocity(&game->players[1], getPlayerVelocityX(&game->players[1]), 5.0);
        // if (state[SDL_SCANCODE_LEFT])
        //     setPlayerVelocity(&game->players[1], -5.0, getPlayerVelocityY(&game->players[1]));
        // if (state[SDL_SCANCODE_RIGHT])
        //     setPlayerVelocity(&game->players[1], 5.0, getPlayerVelocityY(&game->players[1]));

        game_update(game, renderer);
        SDL_Delay(16); // Delay to cap the frame rate (approximately 60 FPS)
    }
}


void  Player_handleInput(Game *game,InputState *input)
{
    if (input->quit)    game->state = GAME_STATE_GAME_OVER;
    if (input->up)      setPlayerVelocity(&game->players[0], getPlayerVelocityX(&game->players[0]), -5.0);
    if (input->down)    setPlayerVelocity(&game->players[0], getPlayerVelocityX(&game->players[0]), 5.0);
    if (input->left)    setPlayerVelocity(&game->players[0], -5.0, getPlayerVelocityY(&game->players[0]));
    if (input->right)   setPlayerVelocity(&game->players[0], 5.0, getPlayerVelocityY(&game->players[0]));
}








void game_update(Game *game, Renderer *renderer)
{
    
    // Update player position based on velocity and check for collisions
    for (int i = 0; i < game->numPlayers; i++)
    {
        Player *p = &game->players[i];

        movePlayer(game->map, p);

        movePlayerWithOther(p, game->players, game->numPlayers);
    }

    // Render the game state
    Background_Image_Render(renderer);
    Render_Map(renderer, game->map);
    for (int i = 0; i < game->numPlayers; i++)
    {
        Render_Player(renderer, &game->players[i]);
    }

    Renderer_Present(renderer);
}

void movePlayerWithOther(Player *p, Player players[], int count)
{
   
    for (int i = 0; i < count; i++)
    {
        Player *other = &players[i];

        if (p != other)
        {
           
            if (Player_collisionWithOtherPlayer(
                    p->x, p->y,
                    (int)other->x, (int)other->y))
            {    // will make that boomb will go to the other player 
                printf("Collision betwsssseen players detected!\n");
            }
           
        }
    }
}
// uppdate player position based on collision with the map and uppdate player position.
void movePlayer(Map *map, Player *p)
{
    float newX = p->x + p->vx;
    int colx = Collision_Map(map, newX, p->y);
    if (colx == 1 || colx == 2)
    {
        /// collision with tile type 1 or 2, stop player movement
    }
    else
    {
        if (colx == 3) // I will update so here will move slow
        {
            printf("Collision with tile type 3 at (%d, %d)\n", (int)(newX / map->tileSize), (int)(p->y / map->tileSize));
            setPlayerPosition(p, newX, p->y);
        }
        else
        {
            setPlayerPosition(p, newX, p->y);
        }
    }

    float newY = p->y + p->vy;
    int coly = Collision_Map(map, p->x, newY);
    if (coly == 1 || coly == 2)
    {
        /// collision with tile type 1 or 2, stop player movement
    }
    else
    {
        if (coly == 3) // I will update so here will move slow
        {
            printf("Collision with tile type 3 at (%d, %d)\n", (int)(newX / map->tileSize), (int)(p->y / map->tileSize));
            setPlayerPosition(p, p->x, newY);
        }
        else
        {
            setPlayerPosition(p, p->x, newY);
        }
    }
}

void game_cleanup(Game *game, Renderer *renderer,Event *event)
{
    Destroy_Input(event->input);
    Map_destroy(game->map);
    Renderer_Destroy(renderer);
}

void game_init(Game *game, Renderer *renderer,Event *event)
{
    game->map = Map_create(WIDTH, HEIGHT);
    game->state = GAME_STATE_PLAYING;
    event->input = Input_Init();
    printf("%d Input initialized.\n", event->input->quit);



    Renderer_Init(renderer, "Hello, World!", game->map->width, game->map->height); // will be update to be as ADT
}