#include "menu_controller.h"

void menu_init(Game *game, Renderer *renderer)
{
    Sound_Init(&game->sound);
    game->state = GAME_STATE_MENU;
    Renderer_Init(renderer, "TIC TAC BOMB", WIDTH, HEIGHT);
}

void menu_loop(Game *game, Renderer *renderer)
{
   // Sound_PlayMenuMusic(&game->sound);

    SDL_Event event;
    SDL_Point mousePoint;

    float scaleX, scaleY;
    int w, h;

    while (game->state == GAME_STATE_MENU)
    {
        // ---------------- INPUT ----------------
        SDL_GetMouseState(&mousePoint.x, &mousePoint.y);

        getScale(renderer, &scaleX, &scaleY);
        SDL_GetWindowSize(renderer->window, &w, &h);

        float buttonWidth = 750 * 0.5f * scaleX;
        float buttonHeight = 170 * 0.5f * scaleY;

        int centerX = w / 2 - buttonWidth / 2;

        SDL_Rect startRect = {centerX, (int)(300 * scaleY), buttonWidth, buttonHeight};
        SDL_Rect optionsRect = {centerX, (int)(420 * scaleY), buttonWidth, buttonHeight};
        SDL_Rect quitRect = {centerX, (int)(540 * scaleY), buttonWidth, buttonHeight};

        // ---------------- EVENTS ----------------
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game->running = 0;
                game->state = GAME_STATE_GAME_OVER;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT)
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
        }

        // ---------------- HOVER ----------------
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

        // ---------------- RENDER ----------------
        SDL_RenderClear(renderer->sdlRenderer);

        Render_Menu(renderer);

        SDL_RenderPresent(renderer->sdlRenderer);
    }
}

void choose_host_join_loop(Game *game, Renderer *renderer)
{
    if (game->state != GAME_STATE_CHOOSE_HOST_JOIN)
    {
        return;
    }

    SDL_Event event;
    SDL_Point mousePoint;

    while (game->state == GAME_STATE_CHOOSE_HOST_JOIN)
    {
        int windowW = 0;
        int windowH = 0;

        SDL_GetMouseState(&mousePoint.x, &mousePoint.y);
        SDL_GetWindowSize(renderer->window, &windowW, &windowH);

        SDL_Rect bg = {0, 0, windowW, windowH};

        SDL_Rect panelShadow = {windowW / 2 - 300 + 8, 170 + 8, 600, 430};
        SDL_Rect panel       = {windowW / 2 - 300,     170,     600, 430};

        SDL_Rect backRect = {panel.x + 20, panel.y + 18, 42, 34};

        SDL_Rect titlePlate = {windowW / 2 - 220, 205, 440, 70};

        SDL_Rect hostRect = {windowW / 2 - 190, 350, 380, 80};
        SDL_Rect joinRect = {windowW / 2 - 190, 460, 380, 80};

        int backHover = SDL_PointInRect(&mousePoint, &backRect);
        int hostHover = SDL_PointInRect(&mousePoint, &hostRect);
        int joinHover = SDL_PointInRect(&mousePoint, &joinRect);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game->running = 0;
                game->state = GAME_STATE_GAME_OVER;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                game->state = GAME_STATE_MENU;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                if (backHover)
                {
                    game->state = GAME_STATE_MENU;
                }

                if (hostHover)
                {
                    game->state = GAME_STATE_HOST_SETUP;
                }

                if (joinHover)
                {
                    game->state = GAME_STATE_CLIENT_SETUP;
                }
            }
        }

        if (backHover || hostHover || joinHover)
        {
            SDL_SetCursor(renderer->cursorHand);
        }
        else
        {
            SDL_SetCursor(renderer->cursorArrow);
        }

        // ---------------- RENDER ----------------
        SDL_RenderClear(renderer->sdlRenderer);

        if (renderer->menuBackgroundTexture)
        {
            SDL_RenderCopy(renderer->sdlRenderer, renderer->menuBackgroundTexture, NULL, &bg);
        }

        SDL_SetRenderDrawBlendMode(renderer->sdlRenderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 145);
        SDL_RenderFillRect(renderer->sdlRenderer, &bg);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 160);
        SDL_RenderFillRect(renderer->sdlRenderer, &panelShadow);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 18, 22, 38, 235);
        SDL_RenderFillRect(renderer->sdlRenderer, &panel);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 180, 40, 255);
        SDL_RenderDrawRect(renderer->sdlRenderer, &panel);

        SDL_Rect innerPanel = {panel.x + 8, panel.y + 8, panel.w - 16, panel.h - 16};
        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 230, 150, 180);
        SDL_RenderDrawRect(renderer->sdlRenderer, &innerPanel);

        /* Back arrow */
        if (backHover)
        {
            SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 170, 25, 210);
            SDL_RenderFillRect(renderer->sdlRenderer, &backRect);
        }

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 210, 90, 255);
        SDL_RenderDrawRect(renderer->sdlRenderer, &backRect);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 245, 245, 255, 255);

        int arrowCenterX = backRect.x + backRect.w / 2;
        int arrowCenterY = backRect.y + backRect.h / 2;

        SDL_RenderDrawLine(renderer->sdlRenderer,
                           arrowCenterX + 7, arrowCenterY - 9,
                           arrowCenterX - 6, arrowCenterY);

        SDL_RenderDrawLine(renderer->sdlRenderer,
                           arrowCenterX - 6, arrowCenterY,
                           arrowCenterX + 7, arrowCenterY + 9);

        /* Title */
        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 170, 25, 235);
        SDL_RenderFillRect(renderer->sdlRenderer, &titlePlate);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 235, 160, 255);
        SDL_RenderDrawRect(renderer->sdlRenderer, &titlePlate);

        SDL_Color darkText = {15, 15, 20, 255};
        SDL_Color lightText = {245, 245, 255, 255};

        Render_MenuTextCentered(renderer, "HOST or JOIN?", titlePlate, darkText);

        Render_MenuText(renderer, "Choose multiplayer mode", windowW / 2 - 145, 295, lightText);

        /* Button shadows */
        SDL_Rect hostShadow = {hostRect.x + 6, hostRect.y + 6, hostRect.w, hostRect.h};
        SDL_Rect joinShadow = {joinRect.x + 6, joinRect.y + 6, joinRect.w, joinRect.h};

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 150);
        SDL_RenderFillRect(renderer->sdlRenderer, &hostShadow);
        SDL_RenderFillRect(renderer->sdlRenderer, &joinShadow);

        /* Host button */
        if (hostHover)
        {
            SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 215, 70, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 190, 45, 245);
        }

        SDL_RenderFillRect(renderer->sdlRenderer, &hostRect);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 245, 190, 255);
        SDL_RenderDrawRect(renderer->sdlRenderer, &hostRect);

        /* Join button */
        if (joinHover)
        {
            SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 135, 45, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 105, 25, 245);
        }

        SDL_RenderFillRect(renderer->sdlRenderer, &joinRect);

        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 220, 120, 255);
        SDL_RenderDrawRect(renderer->sdlRenderer, &joinRect);

        Render_MenuTextCentered(renderer, "HOST GAME", hostRect, darkText);
        Render_MenuTextCentered(renderer, "JOIN GAME", joinRect, lightText);

        SDL_RenderPresent(renderer->sdlRenderer);
    }
}

void host_setup_loop(Game *game, Renderer *renderer, ClientNet *clientNet)
{
    if (game->state != GAME_STATE_HOST_SETUP)
        return;

    static int initialized = 0;
    static int connectedAsHost = 0;

    SDL_Event event;

    // ---------------- INIT HOST ONCE ----------------//
    if (!initialized)
    {
        printf("[HOST] Starting server...\n");
        system("start ..\\server\\server.exe");

        SDL_Delay(10000); // allow server to boot

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
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            game->running = 0;
            game->state = GAME_STATE_GAME_OVER;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (*clientNet)
            {
                printf("[HOST] Start game request sent\n");
                ClientNet_SendStartGame(*clientNet);
            }
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
    int windowW = 0;
    int windowH = 0;
    SDL_GetWindowSize(renderer->window, &windowW, &windowH);

    SDL_Rect bg = {0, 0, windowW, windowH};

    SDL_RenderClear(renderer->sdlRenderer);

    if (renderer->menuBackgroundTexture)
    {
        SDL_RenderCopy(renderer->sdlRenderer, renderer->menuBackgroundTexture, NULL, &bg);
    }

    SDL_SetRenderDrawBlendMode(renderer->sdlRenderer, SDL_BLENDMODE_BLEND);

    /* mörk overlay över bakgrunden */
    SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer->sdlRenderer, &bg);

    /* huvudpanel */
    SDL_Rect panelShadow = {windowW / 2 - 310 + 8, 175 + 8, 620, 410};
    SDL_Rect panel       = {windowW / 2 - 310,     175,     620, 410};

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 170);
    SDL_RenderFillRect(renderer->sdlRenderer, &panelShadow);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 18, 22, 38, 235);
    SDL_RenderFillRect(renderer->sdlRenderer, &panel);

    /* borders */
    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 180, 40, 255);
    SDL_RenderDrawRect(renderer->sdlRenderer, &panel);

    SDL_Rect innerPanel = {panel.x + 8, panel.y + 8, panel.w - 16, panel.h - 16};
    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 230, 150, 180);
    SDL_RenderDrawRect(renderer->sdlRenderer, &innerPanel);

    /* title plate */
    SDL_Rect titlePlate = {windowW / 2 - 220, 215, 440, 70};

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 170, 25, 235);
    SDL_RenderFillRect(renderer->sdlRenderer, &titlePlate);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 235, 160, 255);
    SDL_RenderDrawRect(renderer->sdlRenderer, &titlePlate);

    SDL_Color darkText = {15, 15, 20, 255};
    SDL_Color lightText = {245, 245, 255, 255};
    SDL_Color yellowText = {255, 220, 90, 255};

    Render_MenuTextCentered(renderer, "HOST MODE", titlePlate, darkText);

    /* status box */
    SDL_Rect statusBox = {windowW / 2 - 230, 330, 460, 80};

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 190, 45, 245);
    SDL_RenderFillRect(renderer->sdlRenderer, &statusBox);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 245, 190, 255);
    SDL_RenderDrawRect(renderer->sdlRenderer, &statusBox);

    if (connectedAsHost)
    {
        Render_MenuTextCentered(renderer, "WAITING FOR PLAYERS...", statusBox, darkText);
    }
    else
    {
        Render_MenuTextCentered(renderer, "CONNECTING...", statusBox, darkText);
    }

    /* server info */
    Render_MenuText(renderer, "SERVER ADDRESS", windowW / 2 - 95, 445, yellowText);
    Render_MenuText(renderer, "127.0.0.1:2000", windowW / 2 - 80, 480, lightText);

    /* instruction */
    Render_MenuText(renderer,
                    "Click anywhere when players are ready",
                    windowW / 2 - 190,
                    530,
                    lightText);

    SDL_RenderPresent(renderer->sdlRenderer);
}

void client_setup_loop(Game *game, Renderer *renderer, ClientNet *clientNet)
{
    if (game->state != GAME_STATE_CLIENT_SETUP)
        return;

    static int initialized = 0;

    SDL_Event event;

    if (!initialized)
    {
        *clientNet = ClientNet_Init("127.0.0.1", 2000);
        if (!*clientNet)
        {
            printf("Client init failed\n");
            return;
        }
        ClientNet_SendJoinRequest(*clientNet);
        initialized = 1;
    }

    // ---------------- INPUT ----------------
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            game->running = 0;
            game->state = GAME_STATE_GAME_OVER;
        }
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