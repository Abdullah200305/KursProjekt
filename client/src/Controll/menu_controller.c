#include "menu_controller.h"
#include "Input.h"

void menu_init(Game *game, Renderer *renderer)
{
    Sound_Init(&game->sound);
    game->state = GAME_STATE_MENU;
    Renderer_Init(renderer, "TIC TAC BOMB", WIDTH, HEIGHT);
}

void menu_loop(Game *game, Renderer *renderer)
{
    //Sound_PlayMenuMusic(&game->sound);

    InputState *input = Input_Init();
    SDL_Point mousePoint;

    float scaleX, scaleY;
    int w, h;

    while (game->state == GAME_STATE_MENU)
    {
        // ---------------- INPUT ----------------
        Input_HandleEvents(input);

        if (input->quit)
        {
            game->running = 0;
            game->state = GAME_STATE_GAME_OVER;
        }

        mousePoint.x = input->mouseX;
        mousePoint.y = input->mouseY;

        getScale(renderer, &scaleX, &scaleY);
        SDL_GetWindowSize(renderer->window, &w, &h);

        float buttonWidth = 750 * 0.5f * scaleX;
        float buttonHeight = 170 * 0.5f * scaleY;

        int centerX = w / 2 - buttonWidth / 2;

        SDL_Rect startRect = {centerX, (int)(300 * scaleY), buttonWidth, buttonHeight};
        SDL_Rect optionsRect = {centerX, (int)(420 * scaleY), buttonWidth, buttonHeight};
        SDL_Rect quitRect = {centerX, (int)(540 * scaleY), buttonWidth, buttonHeight};

        
        if (input->mouseClicked)
        {
            if (SDL_PointInRect(&mousePoint, &startRect))
            {
                printf("START CLICKED\n");
                game->state = GAME_STATE_CHOOSE_HOST_JOIN;
            }

            if (SDL_PointInRect(&mousePoint, &optionsRect))
            {
                // options
            }

            if (SDL_PointInRect(&mousePoint, &quitRect))
            {
                game->state = GAME_STATE_GAME_OVER;
                game->running = 0;
            }
        }

        if (SDL_PointInRect(&mousePoint, &startRect) ||
            SDL_PointInRect(&mousePoint, &optionsRect) ||
            SDL_PointInRect(&mousePoint, &quitRect))
        {
            SDL_SetCursor(renderer->cursorHand);
        }
        else
        {
            SDL_SetCursor(renderer->cursorArrow);
        }

        SDL_RenderClear(renderer->sdlRenderer);
        Render_Menu(renderer);
        SDL_RenderPresent(renderer->sdlRenderer);
    }

    Input_Destroy(input);
}


void choose_host_join_loop(Game *game, Renderer *renderer)
{
    if (game->state != GAME_STATE_CHOOSE_HOST_JOIN)
    {
        return;
    }
    InputState *input = Input_Init();
    SDL_Point mousePoint;

    SDL_Rect hostRect = {400, 300, 300, 80};
    SDL_Rect joinRect = {400, 420, 300, 80};

    while (game->state == GAME_STATE_CHOOSE_HOST_JOIN)
    {
        Input_HandleEvents(input);

        if (input->quit)
        {
            game->running = 0;
            game->state = GAME_STATE_GAME_OVER;
        }

        mousePoint.x = input->mouseX;
        mousePoint.y = input->mouseY;

       if (input->mouseClicked)
        {
            if (SDL_PointInRect(&mousePoint, &hostRect))
            {
                game->state = GAME_STATE_HOST_SETUP;
            }

            if (SDL_PointInRect(&mousePoint, &joinRect))
            {
                game->state = GAME_STATE_CLIENT_SETUP;
            }
        }
        // this will make it better in render
        SDL_RenderClear(renderer->sdlRenderer);

        Render_Text(renderer, "HOST or JOIN?", 350, 200);

        Render_Button(renderer, hostRect, "HOST");
        Render_Button(renderer, joinRect, "JOIN");

        SDL_RenderPresent(renderer->sdlRenderer);
    }

    Input_Destroy(input);
}


void host_setup_loop(Game *game, Renderer *renderer, ClientNet *clientNet)
{
    if (game->state != GAME_STATE_HOST_SETUP)
        return;

    static int initialized = 0;
    static int connectedAsHost = 0;

   InputState *input = Input_Init();

    // ---------------- INIT HOST ONCE ----------------//
    if (!initialized)
    {
        printf("[HOST] Starting server...\n");
        system("start ..\\server\\server.exe");

        SDL_Delay(4000); // allow server to boot

        printf("[HOST] Connecting as player 1...\n");

        *clientNet = ClientNet_Init("127.0.0.1", 2000);

        if (*clientNet)
        {
            ClientNet_SendJoinRequest(*clientNet);
            connectedAsHost = 1;
            printf("[HOST] JOIN sent\n");
        }
        else
        {
            printf("[HOST] CLIENT INIT FAILED\n");
        }

        initialized = 1;
    }

    // ---------------- INPUT ----------------
    Input_HandleEvents(input);

    if (input->quit)
    {
        game->running = 0;
        game->state = GAME_STATE_GAME_OVER;
    }

     if (input->mouseClicked)
    {
        if (*clientNet)
        {
            printf("[HOST] Start game request sent\n");
            ClientNet_SendStartGame(*clientNet);
        }
    }

    ClientNet_TryReceive(*clientNet);
    
    if (ClientNet_HasGameInit(*clientNet))
    {
        game_init_renderer(renderer);
        game_init(game, renderer, *clientNet); 
        printf("Game starting\n");
        game->state = GAME_STATE_COUNTDOWN;
    }

    // ---------------- RENDER ----------------
    SDL_RenderClear(renderer->sdlRenderer);

    Render_Text(renderer, "HOST MODE", 400, 150);

    if (connectedAsHost)
        Render_Text(renderer, "WAITING FOR PLAYERS...", 300, 300);
    else
        Render_Text(renderer, "CONNECTING...", 300, 300);

    Render_Text(renderer, "SERVER: 127.0.0.1:2000", 200, 400);

    SDL_RenderPresent(renderer->sdlRenderer);

    Input_Destroy(input);
}

void client_setup_loop(Game *game, Renderer *renderer, ClientNet *clientNet)
{
    if (game->state != GAME_STATE_CLIENT_SETUP)
        return;

    static int initialized = 0;

    InputState *input = Input_Init();
    

    if (!initialized)
    {
        *clientNet = ClientNet_Init("127.0.0.1", 2000);
        if (!*clientNet)
        {
            printf("Client init failed\n");
            Input_Destroy(input);
            return;
        }
        ClientNet_SendJoinRequest(*clientNet);
        initialized = 1;
    }

    // ---------------- INPUT ----------------
    Input_HandleEvents(input);
    
    if (input->quit)
    {
        game->running = 0;
        game->state = GAME_STATE_GAME_OVER;
    }

    // ---------------- RECEIVE ----------------
    ClientNet_TryReceive(*clientNet);

    if (ClientNet_HasGameInit(*clientNet))
    {
        game_init_renderer(renderer);
        game_init(game, renderer, *clientNet); 
        printf("Game starting\n");
        game->state = GAME_STATE_COUNTDOWN;
    }

    // ---------------- RENDER ----------------
    SDL_RenderClear(renderer->sdlRenderer);
    Render_Text(renderer, "WAITING FOR HOST...", 300, 300);
    SDL_RenderPresent(renderer->sdlRenderer);

    Input_Destroy(input);
}



void countdown_loop(Game *game, Renderer *renderer, ClientNet clientNet)
{
    
    
    if (game->state != GAME_STATE_COUNTDOWN)
        return;

    ClientNet_TryReceive(clientNet);
     int countdown = ClientNet_getConutDown(clientNet);
    
    if(ClientNet_HasGameStart(clientNet))
    {
    printf("Countdown: %d\n", countdown);
  

    SDL_RenderClear(renderer->sdlRenderer);

    if (countdown <= 0)
    {
        game->state = GAME_STATE_PLAYING;
        return;
    } 
    }
   

    char countdownStr[16];
    snprintf(countdownStr, sizeof(countdownStr), "%d", countdown);

    Render_Text(renderer, countdownStr, 400, 300);
    SDL_RenderPresent(renderer->sdlRenderer);
}