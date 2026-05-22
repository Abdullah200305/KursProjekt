#include "Game_controll.h"

void game_loop(Game *game, Renderer *renderer, ClientNet clientNet)
{
    Sound_PlayGameMusic(&game->sound);
    Uint32 lastSend = 0;
    const int SEND_RATE = 8;
    const int FPS = 120;
    const int FRAME_DELAY = 1000 / FPS;

    SDL_Event event;

    while (game->state == GAME_STATE_PLAYING)
    {
        Uint32 frameStart = SDL_GetTicks();

        // Handle events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game->state = GAME_STATE_GAME_OVER;
                game->running = 0;
            }
        }

        // Receive server updates
        if (clientNet != NULL)
        {
            int receiveResult = 0;

            while ((receiveResult = ClientNet_TryReceive(clientNet)) > 0)
            {
                if (!ClientNet_IsConnected(clientNet) ||
                    ClientNet_HasTimedOut(clientNet, 3000))
                {
                    game_handle_server_disconnect(game, clientNet);
                    return;
                }

                if (ClientNet_HasGameState(clientNet))
                {
                    game_apply_network_state(game, clientNet);
                }
            }

            if (receiveResult < 0)
            {
                game_handle_server_disconnect(game, clientNet);
                return;
            }
        }

        // Send input
        Uint32 now = SDL_GetTicks();

        if (clientNet != NULL && ClientNet_GetClientId(clientNet) >= 0)
        {
            if (now - lastSend >= SEND_RATE)
            {
                InputPacket input = {0};
                const Uint8 *state = SDL_GetKeyboardState(NULL);

                input.type = PACKET_INPUT;
                input.clientId = ClientNet_GetClientId(clientNet);
                input.up = state[SDL_SCANCODE_W];
                input.down = state[SDL_SCANCODE_S];
                input.left = state[SDL_SCANCODE_A];
                input.right = state[SDL_SCANCODE_D];

                if (ClientNet_SendInput(clientNet, &input) < 0)
                {
                    game_handle_server_disconnect(game, clientNet);
                    return;
                }

                lastSend = now;
            }
        }

        // Game over message
        if (game->state == GAME_STATE_GAME_OVER)
        {
            const char *message = "Game over!";

            if (isPlayerAlive(game->players[0]) && !isPlayerAlive(game->players[1]))
            {
                message = "Game over!\nPlayer 1 wins.\n\nPress R to play again.\nPress M to return to menu.";
            }
            else if (isPlayerAlive(game->players[1]) && !isPlayerAlive(game->players[0]))
            {
                message = "Game over!\nPlayer 2 wins.\n\nPress R to play again.\nPress M to return to menu.";
            }
            else if (!isPlayerAlive(game->players[0]) && !isPlayerAlive(game->players[1]))
            {
                message = "Game over!\nBoth players died.\n\nPress R to play again.\nPress M to return to menu.";
            }

            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_INFORMATION,
                "Game Over",
                message,
                renderer->window);
        }

        game_update(game, renderer);

        // Frame cap
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
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






void game_handle_server_disconnect(Game *game, ClientNet clientNet)
{
    printf("[CLIENT] Lost connection to server. Returning to menu.\n");

    game_reset_network_data(game);

    if (clientNet != NULL)
    {
        ClientNet_Destroy(clientNet);
        clientNet = NULL;
    }

    game->state = GAME_STATE_MENU;
}

void game_update(Game *game, Renderer *renderer)
{
    // Render the game state
    Background_Image_Render(renderer);

    // this for test
   // Render_Map(renderer, game->map);

    AbilitySystem_render(game->abilitySystem, renderer);
    for (int i = 0; i < game->numPlayers; i++)
    {
        setPlayerAnimation(game->players[i]);
    }

    for (int i = 0; i < game->numPlayers; i++)
    {
        if (isPlayerAlive(game->players[i]))
        {
            Render_Player(renderer, game->players[i], i);
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

    if (aliveCount <= 1)
    {
        game->state = GAME_STATE_GAME_OVER;
    }

    Renderer_Present(renderer);

    // here will be the miusic and sound effect logic
    if (getBombExploding(game->bomb))
    {
        Sound_PlayExplosion(&game->sound);
    }

    // Laugh sound effect timer
    static int RNGlaughTimer = 0;
    RNGlaughTimer--;
    if (RNGlaughTimer <= 0)
    {
        RNGlaughTimer = 300 + rand() % 6000;

        if (rand() % 2 == 0)
            Sound_PlayLaugh1(&game->sound);
        else
            Sound_PlayLaugh2(&game->sound);
    }
}

// init stuff before game start like map, bomb and players
void game_init(Game *game, Renderer *renderer, ClientNet clientNet)
{
    // 10_000ms  mean 10 sec  will run game init otherwise re-join to server

    const Uint32 timeout = SDL_GetTicks() + 10000;
    int game_init = -1;
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout))
    {
        if (clientNet)
        {
            ClientNet_TryReceive(clientNet);
            if (ClientNet_HasGameInit(clientNet))
            {
                game_init = game_apply_network_init(game, clientNet);
            }

            if (game_init >= 0)
            {
                printf("Game_init have been stablished\n");
                break;
            }
        }
    }
    if (game_init < 0)
    {
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

////////////////////////// these functions will handle the depacket and init and game_init /////////////////////////////

// here will packet open and make uppdate
void game_apply_network_state(Game *game, ClientNet clientNet)
{
    GameStatePacket packet;
    int playersToUpdate;
    if (game == NULL || clientNet == NULL)
    {
        return;
    }

    if (!ClientNet_HasGameState(clientNet))
    {
        return;
    }

    packet = ClientNet_GetGameStatePacket(clientNet);

    playersToUpdate = packet.data.numPlayers;

    if (playersToUpdate > game->numPlayers)
    {
        playersToUpdate = game->numPlayers;
    }

    if (playersToUpdate > MAX_PLAYERS)
    {
        playersToUpdate = MAX_PLAYERS;
    }

    for (int i = 0; i < playersToUpdate; i++)
    {

        setPlayerPosition(
            game->players[i],
            packet.data.players[i].x,
            packet.data.players[i].y);

        setPlayerVelocity(game->players[i],
                          packet.data.players[i].vx,
                          packet.data.players[i].vy);

        setPlayerSize(game->players[i],
                      packet.data.players[i].width,
                      packet.data.players[i].height);

        setPlayerState(
            game->players[i],
            packet.data.players[i].lives,
            packet.data.players[i].alive);
        setPlayerFreezeTimer(game->players[i], packet.data.players[i].freezeTimer);
        //  printf("x: %d y: %d\n", packet.data.players[i].x, packet.data.players[i].y);
    }

    setBombState(
        game->bomb,
        packet.data.bomb.x,
        packet.data.bomb.y,
        packet.data.bomb.timer,
        packet.data.bomb.bombCarrier,
        packet.data.bomb.active,
        packet.data.bomb.exploding);

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
            packet.data.abilities.items[i].active);
    }
    // printf("[CLIENT] Applied GAME_STATE positions locally\n");
    ClientNet_ClearGameState(clientNet);
}

int game_apply_network_init(Game *game, ClientNet clientNet)
{
    GameInitPacket packet;
    if (game == NULL || clientNet == NULL)
    {
        return -1;
    }
    if (!ClientNet_HasGameInit(clientNet))
    {
        return -1;
    }
    packet = ClientNet_GetGameInitPacket(clientNet);

    game->mapId = packet.data.map.mapId;
    game->map = Map_create(packet.data.map.width, packet.data.map.height);
    // game->state = GAME_STATE_PLAYING; // this will be change

    game->numPlayers = packet.data.numPlayers;
    for (int i = 0; i < game->numPlayers; i++)
    {
        //  player init will update after
        game->players[i] = initPlayer(packet.data.players[i].x, packet.data.players[i].y);
    }
    game->bomb = createBomb(game->players);

    game->abilitySystem = AbilitySystem_create();
    AbilitySystem_init(game->abilitySystem);

    // will remove
    // printf("%d server \n",packet.data.yourClientId);
    // this is id for server not any player
    // ClientNet_SetClientId(clientNet, packet.data.yourClientId);
    ClientNet_ClearGameInit(clientNet);
    // printf("[CLIENT] Applied GAME_INIT locally\n");
    return 0;
}

void game_over_loop(Game *game, Renderer *renderer, ClientNet clientNet)
{
    if (game == NULL || renderer == NULL)
    {
        return;
    }

    SDL_Event event;
    SDL_Point mousePoint;

    int windowW = 0;
    int windowH = 0;

    SDL_GetMouseState(&mousePoint.x, &mousePoint.y);
    SDL_GetWindowSize(renderer->window, &windowW, &windowH);

    SDL_Rect bg = {0, 0, windowW, windowH};

    SDL_Rect panelShadow = {windowW / 2 - 310 + 8, 170 + 8, 620, 420};
    SDL_Rect panel       = {windowW / 2 - 310,     170,     620, 420};

    SDL_Rect titlePlate = {windowW / 2 - 220, 215, 440, 80};

    SDL_Rect menuButton = {windowW / 2 - 190, 355, 380, 80};
    SDL_Rect exitButton = {windowW / 2 - 190, 465, 380, 80};

    int menuHover = SDL_PointInRect(&mousePoint, &menuButton);
    int exitHover = SDL_PointInRect(&mousePoint, &exitButton);

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            game->running = 0;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_LEFT)
        {
            if (menuHover)
            {
                if (clientNet != NULL)
                {
                    ClientNet_SendDisconnect(clientNet);
                }

                game->state = GAME_STATE_MENU;
            }

            if (exitHover)
            {
                game->running = 0;
            }
        }
    }

    if (menuHover || exitHover)
    {
        SDL_SetCursor(renderer->cursorHand);
    }
    else
    {
        SDL_SetCursor(renderer->cursorArrow);
    }

    int clientId = -1;

    if (clientNet != NULL)
    {
        clientId = ClientNet_GetClientId(clientNet);
    }

    int playerWon = 0;

    if (clientId >= 0 && clientId < game->numPlayers && game->players[clientId] != NULL)
    {
        playerWon = isPlayerAlive(game->players[clientId]);
    }

    const char *resultText = "YOU LOST";

    if (playerWon)
    {
        resultText = "CONGRATULATIONS";
    }

    SDL_RenderClear(renderer->sdlRenderer);

    if (renderer->menuBackgroundTexture)
    {
        SDL_RenderCopy(renderer->sdlRenderer, renderer->menuBackgroundTexture, NULL, &bg);
    }

    SDL_SetRenderDrawBlendMode(renderer->sdlRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 160);
    SDL_RenderFillRect(renderer->sdlRenderer, &bg);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 170);
    SDL_RenderFillRect(renderer->sdlRenderer, &panelShadow);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 18, 22, 38, 235);
    SDL_RenderFillRect(renderer->sdlRenderer, &panel);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 180, 40, 255);
    SDL_RenderDrawRect(renderer->sdlRenderer, &panel);

    SDL_Rect innerPanel = {panel.x + 8, panel.y + 8, panel.w - 16, panel.h - 16};
    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 230, 150, 180);
    SDL_RenderDrawRect(renderer->sdlRenderer, &innerPanel);

    SDL_Color darkText = {15, 15, 20, 255};
    SDL_Color lightText = {245, 245, 255, 255};

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 170, 25, 235);
    SDL_RenderFillRect(renderer->sdlRenderer, &titlePlate);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 235, 160, 255);
    SDL_RenderDrawRect(renderer->sdlRenderer, &titlePlate);

    Render_MenuTextCentered(renderer, resultText, titlePlate, darkText);

    SDL_Rect menuShadow = {menuButton.x + 6, menuButton.y + 6, menuButton.w, menuButton.h};
    SDL_Rect exitShadow = {exitButton.x + 6, exitButton.y + 6, exitButton.w, exitButton.h};

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer->sdlRenderer, &menuShadow);
    SDL_RenderFillRect(renderer->sdlRenderer, &exitShadow);

    if (menuHover)
    {
        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 215, 70, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 190, 45, 245);
    }

    SDL_RenderFillRect(renderer->sdlRenderer, &menuButton);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 245, 190, 255);
    SDL_RenderDrawRect(renderer->sdlRenderer, &menuButton);

    if (exitHover)
    {
        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 135, 45, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 105, 25, 245);
    }

    SDL_RenderFillRect(renderer->sdlRenderer, &exitButton);

    SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 220, 120, 255);
    SDL_RenderDrawRect(renderer->sdlRenderer, &exitButton);

    Render_MenuTextCentered(renderer, "MAIN MENU", menuButton, darkText);
    Render_MenuTextCentered(renderer, "EXIT", exitButton, lightText);

    SDL_RenderPresent(renderer->sdlRenderer);
}
