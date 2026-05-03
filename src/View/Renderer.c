#include "Renderer.h"


int Renderer_Init(Renderer* r, const char* title, int width, int height) {
    r->hudFont = NULL;

    printf("Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }
    r->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!r->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    r->sdlRenderer = SDL_CreateRenderer(r->window, -1, 0);
    if (!r->sdlRenderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        Renderer_Destroy(r);
        return -1;
    }
    
    r->hudFont = TTF_OpenFont("C:/Windows/Fonts/arialbd.ttf", 20);

    if (r->hudFont == NULL) {
        r->hudFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 20);
    }

    if (r->hudFont == NULL) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
    }
   
    r->backgroundTexture = IMG_LoadTexture(r->sdlRenderer, "link/Island.png");
    if (!r->backgroundTexture) {
        fprintf(stderr, "IMG_LoadTexture Error: %s\n", IMG_GetError());
        Renderer_Destroy(r);
        return -1;
    }
    
    //SPELARE
    r->playerTexture[0] = IMG_LoadTexture(r->sdlRenderer, "link/Player/Player1_Sheet.png");
    r->playerTexture[1] = IMG_LoadTexture(r->sdlRenderer, "link/Player/Player2_Sheet.png");
    //r->playerTexture[2] = IMG_LoadTexture(r->sdlRenderer, "link/Player3_Sheet.png");
    //r->playerTexture[3] = IMG_LoadTexture(r->sdlRenderer, "link/Player4_Sheet.png");
    for(int i = 0; i < 2; i++){
        if (!r->playerTexture[i]) {
        fprintf(stderr, "IMG_LoadTexture Error (player %d): %s\n", i+1,IMG_GetError());
        Renderer_Destroy(r);
        return -1;
        }
    }
    
    //ANIMATION
    int PLAYER_FRAME_WIDTH = 471;
    int PLAYER_FRAME_HEIGHT = 530;

    for (int i = 0; i < 17; i++) {
        r->playerFrames[i].x = i * PLAYER_FRAME_WIDTH;
        r->playerFrames[i].y = 0;
        r->playerFrames[i].w = PLAYER_FRAME_WIDTH;
        r->playerFrames[i].h = PLAYER_FRAME_HEIGHT;
    }

    //ABILITY
    r->abilityTextures[0] = NULL;
    r->abilityTextures[1] = IMG_LoadTexture(r->sdlRenderer, "link/ABILITY_SPEED.png");
    r->abilityTextures[2] = IMG_LoadTexture(r->sdlRenderer, "link/ABILITY_FREEZE.png");
    r->abilityTextures[3] = IMG_LoadTexture(r->sdlRenderer, "link/ABILITY_SWAP.png");
    r->abilityTextures[4] = IMG_LoadTexture(r->sdlRenderer, "link/ABILITY_SIZEUP.png");
    r->abilityTextures[5] = IMG_LoadTexture(r->sdlRenderer, "link/ABILITY_SHIELD.png");
    
    return 0;
}

void getScale(Renderer* r, float* scaleX, float* scaleY) {
    int w, h;
    SDL_GetWindowSize(r->window, &w, &h);

    *scaleX = (float)w / WIDTH;
    *scaleY = (float)h / HEIGHT;
}

// test to see if the renderer work.
void Renderer_Clear(Renderer* r) {
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255);
    SDL_RenderClear(r->sdlRenderer);
}

void Renderer_Present(Renderer* r) {
    SDL_RenderPresent(r->sdlRenderer);
}

void Renderer_Destroy(Renderer* r) {
    if (r->hudFont)
    {
        TTF_CloseFont(r->hudFont);
        r->hudFont = NULL;
    }
    
    SDL_DestroyRenderer(r->sdlRenderer);
    SDL_DestroyWindow(r->window);
    SDL_DestroyTexture(r->backgroundTexture);

    for (int i = 0; i < 2; i++) 
    {
        if (r->playerTexture[i])
        {
            SDL_DestroyTexture(r->playerTexture[i]);
        }
    }

    for (int i = 0; i < 6; i++) 
    {
        if (r->abilityTextures[i])
        {
            SDL_DestroyTexture(r->abilityTextures[i]);
        }
    }
    TTF_Quit();
    SDL_Quit();
}



//******************  Map stuff  *******************//
void Background_Image_Render(Renderer* r) {
    SDL_Texture* img = r->backgroundTexture;

    int w, h;
    SDL_GetWindowSize(r->window, &w, &h);

    SDL_Rect texr = {0, 0, w, h};

    SDL_RenderClear(r->sdlRenderer);
    SDL_RenderCopy(r->sdlRenderer, img, NULL, &texr);
}


// Render the map based on the map buffer to make collision
void Render_Map(Renderer* r, Map map) {
    for (int y = 0; y < TILE_COUNT_Y; y++) {
        for (int x = 0; x < TIlE_COUNT_X; x++)
        {
            int tileType = getMapBufferItems(map,x,y);

            float scaleX, scaleY;
            getScale(r, &scaleX, &scaleY);

            SDL_Rect tileRect = {x * getTileSize(map) * scaleX, y * getTileSize(map) * scaleY, getTileSize(map) * scaleX, getTileSize(map) * scaleY};

            switch (tileType) {
                case 0: 
                    // SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);
                  
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 255,0, 125); // Transparent
                    break;
                case 1:
                    // SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 255, 0, 0); // Black
                    break;
                case 2:
                
                    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255);    
                    break; 
                case 3:
                
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 255, 255);   
                    break;      
                default:
                    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 125, 255, 255); 
                 
                    break;
            }
            SDL_RenderDrawRect(r->sdlRenderer, &tileRect);
        }
    }
}




//******************  Player stuff  *******************//
void Render_Player(Renderer* r, Player player, int playerIndex) {
    SDL_Texture *img = r->playerTexture[playerIndex];
    int frame = getPlayerAnimationFrame(player); 

    float scaleX, scaleY;
    getScale(r, &scaleX, &scaleY);

    SDL_Rect playerRect = 
    {
        getPlayerX(player) * scaleX,
        getPlayerY(player) * scaleY,
        getPlayerHeight(player) * scaleX,
        getPlayerWidth(player) * scaleY
    };
      
    SDL_RenderCopy(r->sdlRenderer, img, &r->playerFrames[frame], &playerRect);


   
  // sencor four /// for testing 
    // SDL_Rect top = { (int)player->x+64/2, (int)player->y, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &top); 

    // SDL_Rect left = { (int)player->x, (int)player->y+64/2, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &left); 

    // SDL_Rect right = { (int)player->x+64-1, (int)player->y+64/2, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &right); 

    // SDL_Rect bottom = { (int)player->x+64/2, (int)player->y+64-1, 1, 1 };
    // SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255); 
    // SDL_RenderFillRect(r->sdlRenderer, &bottom); 


  
}

void Render_PlayerLives(Renderer* r, Player player, int startX, int startY)
{
    if (r == NULL || player == NULL)
    {
        return;
    }

    int lives = getPlayerLives(player);

    if (lives < 0)
    {
        lives = 0;
    }

    if (lives > 3)
    {
        lives = 3;
    }

    int maxLives = 3;
    int size = 4;
    int spacing = 29;

    for (int heartIndex = 0; heartIndex < maxLives; heartIndex++)
    {
        int x = startX + heartIndex * spacing;
        int y = startY;

        int isFilled = (heartIndex < lives);

        SDL_Rect blocks[20] = {
            {x + size,     y,             size, size},
            {x + 2 * size, y,             size, size},
            {x + 4 * size, y,             size, size},
            {x + 5 * size, y,             size, size},

            {x,            y + size,      size, size},
            {x + size,     y + size,      size, size},
            {x + 2 * size, y + size,      size, size},
            {x + 3 * size, y + size,      size, size},
            {x + 4 * size, y + size,      size, size},
            {x + 5 * size, y + size,      size, size},
            {x + 6 * size, y + size,      size, size},

            {x + size,     y + 2 * size,  size, size},
            {x + 2 * size, y + 2 * size,  size, size},
            {x + 3 * size, y + 2 * size,  size, size},
            {x + 4 * size, y + 2 * size,  size, size},
            {x + 5 * size, y + 2 * size,  size, size},

            {x + 2 * size, y + 3 * size,  size, size},
            {x + 3 * size, y + 3 * size,  size, size},
            {x + 4 * size, y + 3 * size,  size, size},

            {x + 3 * size, y + 4 * size,  size, size}
        };

        /* ===== Shadow ===== */
        SDL_SetRenderDrawColor(r->sdlRenderer, 45, 0, 0, 130);

        for (int i = 0; i < 20; i++)
        {
            SDL_Rect shadow = {
                blocks[i].x + 2,
                blocks[i].y + 2,
                blocks[i].w,
                blocks[i].h
            };

            SDL_RenderFillRect(r->sdlRenderer, &shadow);
        }

        /* ===== Dark outline ===== */
        if (isFilled)
        {
            SDL_SetRenderDrawColor(r->sdlRenderer, 120, 0, 10, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(r->sdlRenderer, 65, 35, 25, 230);
        }

        for (int i = 0; i < 20; i++)
        {
            SDL_RenderFillRect(r->sdlRenderer, &blocks[i]);
        }

        /* ===== Main fill ===== */
        if (isFilled)
        {
            SDL_SetRenderDrawColor(r->sdlRenderer, 255, 25, 35, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(r->sdlRenderer, 95, 60, 45, 170);
        }

        for (int i = 0; i < 20; i++)
        {
            SDL_Rect inner = {
                blocks[i].x + 1,
                blocks[i].y + 1,
                blocks[i].w - 1,
                blocks[i].h - 1
            };

            if (inner.w > 0 && inner.h > 0)
            {
                SDL_RenderFillRect(r->sdlRenderer, &inner);
            }
        }

        /* ===== Bright highlight on filled hearts ===== */
        if (isFilled)
        {
            SDL_SetRenderDrawColor(r->sdlRenderer, 255, 145, 160, 240);

            SDL_Rect shine1 = {x + size + 1,     y + 1,        size - 1, size - 1};
            SDL_Rect shine2 = {x + 2 * size + 1, y + 1,        size - 1, size - 1};
            SDL_Rect shine3 = {x + 1,            y + size + 1, size - 1, size - 1};

            SDL_RenderFillRect(r->sdlRenderer, &shine1);
            SDL_RenderFillRect(r->sdlRenderer, &shine2);
            SDL_RenderFillRect(r->sdlRenderer, &shine3);

            SDL_SetRenderDrawColor(r->sdlRenderer, 255, 70, 85, 255);

            SDL_Rect deepRed = {x + 3 * size, y + 2 * size, size, size};
            SDL_RenderFillRect(r->sdlRenderer, &deepRed);
        }
    }
}

static void Render_HUDText(Renderer* r, const char* text, int x, int y, SDL_Color color)
{
    if (r == NULL || r->hudFont == NULL || text == NULL)
    {
        return;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(r->hudFont, text, color);

    if (surface == NULL)
    {
        fprintf(stderr, "TTF_RenderUTF8_Blended Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(r->sdlRenderer, surface);

    if (texture == NULL)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst = {x, y, surface->w, surface->h};

    SDL_FreeSurface(surface);

    SDL_RenderCopy(r->sdlRenderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}


void Render_PlayerHUD(Renderer* r, Player player, int playerIndex, int x, int y, int hasBomb)
{
    if (r == NULL || player == NULL)
    {
        return;
    }

    SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);

    int panelW = 250;
    int panelH = 74;

    SDL_Color accentColor;

    if (playerIndex == 0)
    {
        accentColor = (SDL_Color){210, 70, 150, 255};
    }
    else if (playerIndex == 1)
    {
        accentColor = (SDL_Color){130, 85, 230, 255};
    }
    else if (playerIndex == 2)
    {
        accentColor = (SDL_Color){70, 190, 90, 255};
    }
    else
    {
        accentColor = (SDL_Color){230, 200, 70, 255};
    }

    /* ===== Stronger outer glow / shadow ===== */

    SDL_Rect deepShadow = {x + 8, y + 8, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 110);
    SDL_RenderFillRect(r->sdlRenderer, &deepShadow);

    SDL_Rect softShadow = {x + 4, y + 4, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 80);
    SDL_RenderFillRect(r->sdlRenderer, &softShadow);

    if (hasBomb)
    {
        SDL_Rect bombGlow = {x - 5, y - 5, panelW + 10, panelH + 10};
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 200, 45, 85);
        SDL_RenderFillRect(r->sdlRenderer, &bombGlow);
    }

    /* ===== Main wooden panel layers ===== */

    SDL_Rect outerFrame = {x, y, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 64, 31, 12, 255);
    SDL_RenderFillRect(r->sdlRenderer, &outerFrame);

    SDL_Rect goldFrame = {x + 3, y + 3, panelW - 6, panelH - 6};
    SDL_SetRenderDrawColor(r->sdlRenderer, 235, 178, 72, 255);
    SDL_RenderFillRect(r->sdlRenderer, &goldFrame);

    SDL_Rect darkWood = {x + 7, y + 7, panelW - 14, panelH - 14};
    SDL_SetRenderDrawColor(r->sdlRenderer, 111, 61, 23, 255);
    SDL_RenderFillRect(r->sdlRenderer, &darkWood);

    SDL_Rect woodPanel = {x + 11, y + 11, panelW - 22, panelH - 22};
    SDL_SetRenderDrawColor(r->sdlRenderer, 211, 135, 55, 255);
    SDL_RenderFillRect(r->sdlRenderer, &woodPanel);

    SDL_Rect innerPanel = {x + 15, y + 15, panelW - 30, panelH - 30};
    SDL_SetRenderDrawColor(r->sdlRenderer, 235, 172, 85, 245);
    SDL_RenderFillRect(r->sdlRenderer, &innerPanel);

    /* bright top shine */
    SDL_Rect topShine = {x + 17, y + 17, panelW - 34, 6};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 229, 146, 180);
    SDL_RenderFillRect(r->sdlRenderer, &topShine);

    SDL_Rect thinShine = {x + 20, y + 25, panelW - 40, 2};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 238, 170, 100);
    SDL_RenderFillRect(r->sdlRenderer, &thinShine);

    /* subtle lower dark strip for depth */
    SDL_Rect bottomDepth = {x + 10, y + panelH - 13, panelW - 20, 6};
    SDL_SetRenderDrawColor(r->sdlRenderer, 90, 48, 19, 160);
    SDL_RenderFillRect(r->sdlRenderer, &bottomDepth);

    /* outer outlines */
    SDL_SetRenderDrawColor(r->sdlRenderer, 45, 22, 8, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &outerFrame);

    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 105, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &goldFrame);

    SDL_SetRenderDrawColor(r->sdlRenderer, 55, 28, 10, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &darkWood);

    /* ===== Decorative corner pieces ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 215, 95, 255);

    SDL_Rect c1 = {x + 3, y + 3, 18, 6};
    SDL_Rect c2 = {x + panelW - 21, y + 3, 18, 6};
    SDL_Rect c3 = {x + 3, y + panelH - 9, 18, 6};
    SDL_Rect c4 = {x + panelW - 21, y + panelH - 9, 18, 6};

    SDL_RenderFillRect(r->sdlRenderer, &c1);
    SDL_RenderFillRect(r->sdlRenderer, &c2);
    SDL_RenderFillRect(r->sdlRenderer, &c3);
    SDL_RenderFillRect(r->sdlRenderer, &c4);

    /* small nails */
    SDL_SetRenderDrawColor(r->sdlRenderer, 95, 48, 18, 255);

    SDL_Rect nail1 = {x + 7, y + 7, 5, 5};
    SDL_Rect nail2 = {x + panelW - 12, y + 7, 5, 5};
    SDL_Rect nail3 = {x + 7, y + panelH - 12, 5, 5};
    SDL_Rect nail4 = {x + panelW - 12, y + panelH - 12, 5, 5};

    SDL_RenderFillRect(r->sdlRenderer, &nail1);
    SDL_RenderFillRect(r->sdlRenderer, &nail2);
    SDL_RenderFillRect(r->sdlRenderer, &nail3);
    SDL_RenderFillRect(r->sdlRenderer, &nail4);

    /* ===== Player portrait frame ===== */

    SDL_Rect portraitOuter = {x + 14, y + 13, 52, 52};
    SDL_SetRenderDrawColor(r->sdlRenderer, 53, 27, 12, 255);
    SDL_RenderFillRect(r->sdlRenderer, &portraitOuter);

    SDL_Rect portraitAccent = {x + 17, y + 16, 46, 46};
    SDL_SetRenderDrawColor(r->sdlRenderer, accentColor.r, accentColor.g, accentColor.b, 255);
    SDL_RenderFillRect(r->sdlRenderer, &portraitAccent);

    SDL_Rect portraitFrame = {x + 20, y + 19, 40, 40};
    SDL_SetRenderDrawColor(r->sdlRenderer, 35, 18, 8, 255);
    SDL_RenderFillRect(r->sdlRenderer, &portraitFrame);

    if (playerIndex >= 0 && playerIndex < 4 && r->playerTexture[playerIndex] != NULL)
    {
        SDL_Rect portraitDst = {x + 19, y + 16, 42, 46};

        SDL_RenderCopy(
            r->sdlRenderer,
            r->playerTexture[playerIndex],
            &r->playerFrames[0],
            &portraitDst
        );
    }
    else
    {
        SDL_Rect fallbackPortrait = {x + 23, y + 22, 34, 34};
        SDL_SetRenderDrawColor(r->sdlRenderer, accentColor.r, accentColor.g, accentColor.b, 255);
        SDL_RenderFillRect(r->sdlRenderer, &fallbackPortrait);
    }

    SDL_SetRenderDrawColor(r->sdlRenderer, 20, 10, 5, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &portraitOuter);

    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 100, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &portraitAccent);

    if (hasBomb)
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 230, 70, 255);
        SDL_Rect portraitGlow = {x + 12, y + 11, 56, 56};
        SDL_RenderDrawRect(r->sdlRenderer, &portraitGlow);
    }

    /* ===== Bomb slot ===== */

    SDL_Rect bombSlotOuter = {x + 72, y + 40, 26, 24};
    SDL_SetRenderDrawColor(r->sdlRenderer, 58, 31, 14, 255);
    SDL_RenderFillRect(r->sdlRenderer, &bombSlotOuter);

    SDL_Rect bombSlotInner = {x + 75, y + 43, 20, 18};
    SDL_SetRenderDrawColor(r->sdlRenderer, 112, 66, 30, 230);
    SDL_RenderFillRect(r->sdlRenderer, &bombSlotInner);

    SDL_SetRenderDrawColor(r->sdlRenderer, 35, 18, 8, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &bombSlotOuter);

    if (hasBomb)
    {
        int cx = x + 85;
        int cy = y + 52;
        int radius = 8;

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 210, 60, 120);
        for (int dy = -radius - 3; dy <= radius + 3; dy++)
        {
            for (int dx = -radius - 3; dx <= radius + 3; dx++)
            {
                if (dx * dx + dy * dy <= (radius + 3) * (radius + 3))
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
                }
            }
        }

        SDL_SetRenderDrawColor(r->sdlRenderer, 20, 20, 20, 255);
        for (int dy = -radius; dy <= radius; dy++)
        {
            for (int dx = -radius; dx <= radius; dx++)
            {
                if (dx * dx + dy * dy <= radius * radius)
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
                }
            }
        }

        SDL_SetRenderDrawColor(r->sdlRenderer, 120, 120, 120, 255);
        for (int dy = -2; dy <= 2; dy++)
        {
            for (int dx = -2; dx <= 2; dx++)
            {
                if (dx * dx + dy * dy <= 4)
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx - 3 + dx, cy - 3 + dy);
                }
            }
        }

        SDL_Rect fuseCap = {cx + 1, cy - 10, 6, 4};
        SDL_SetRenderDrawColor(r->sdlRenderer, 90, 90, 90, 255);
        SDL_RenderFillRect(r->sdlRenderer, &fuseCap);

        SDL_SetRenderDrawColor(r->sdlRenderer, 120, 65, 25, 255);
        SDL_RenderDrawLine(r->sdlRenderer, cx + 6, cy - 10, cx + 10, cy - 14);

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 120, 0, 255);
        SDL_RenderDrawPoint(r->sdlRenderer, cx + 11, cy - 15);
        SDL_RenderDrawPoint(r->sdlRenderer, cx + 12, cy - 14);
        SDL_RenderDrawPoint(r->sdlRenderer, cx + 10, cy - 14);

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 240, 80, 255);
        SDL_RenderDrawPoint(r->sdlRenderer, cx + 11, cy - 14);
    }

    /* ===== Player name ===== */

    SDL_Color darkText = {45, 25, 10, 255};
    SDL_Color shadowText = {120, 70, 25, 180};
    SDL_Color lightText = {255, 235, 170, 255};

    const char* playerName = "Player";

    if (playerIndex == 0)
    {
        playerName = "Player 1";
    }
    else if (playerIndex == 1)
    {
        playerName = "Player 2";
    }
    else if (playerIndex == 2)
    {
        playerName = "Player 3";
    }
    else if (playerIndex == 3)
    {
        playerName = "Player 4";
    }

    SDL_Rect namePlate = {x + 105, y + 16, 122, 22};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 204, 95, 90);
    SDL_RenderFillRect(r->sdlRenderer, &namePlate);

    SDL_SetRenderDrawColor(r->sdlRenderer, 120, 65, 24, 120);
    SDL_RenderDrawRect(r->sdlRenderer, &namePlate);

    Render_HUDText(r, playerName, x + 108, y + 11, shadowText);
    Render_HUDText(r, playerName, x + 106, y + 9, darkText);

    /* ===== HP bar background ===== */

    SDL_Rect hpOuter = {x + 105, y + 42, 132, 24};
    SDL_SetRenderDrawColor(r->sdlRenderer, 54, 28, 12, 255);
    SDL_RenderFillRect(r->sdlRenderer, &hpOuter);

    SDL_Rect hpBackground = {x + 109, y + 45, 124, 18};
    SDL_SetRenderDrawColor(r->sdlRenderer, 107, 60, 26, 230);
    SDL_RenderFillRect(r->sdlRenderer, &hpBackground);

    SDL_Rect hpHighlight = {x + 111, y + 47, 120, 3};
    SDL_SetRenderDrawColor(r->sdlRenderer, 180, 115, 50, 150);
    SDL_RenderFillRect(r->sdlRenderer, &hpHighlight);

    SDL_SetRenderDrawColor(r->sdlRenderer, 35, 18, 8, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &hpOuter);

    Render_HUDText(r, "HP", x + 113, y + 40, lightText);

    /* heart shadow */
    SDL_SetRenderDrawColor(r->sdlRenderer, 80, 0, 0, 180);
    SDL_Rect heartShadow = {x + 147, y + 48, 80, 18};
    SDL_RenderFillRect(r->sdlRenderer, &heartShadow);

    Render_PlayerLives(r, player, x + 143, y + 45);

    /* ===== Player-colored bottom accent ===== */

    SDL_Rect accentStrip = {x + 17, y + panelH - 10, 50, 5};
    SDL_SetRenderDrawColor(r->sdlRenderer, accentColor.r, accentColor.g, accentColor.b, 230);
    SDL_RenderFillRect(r->sdlRenderer, &accentStrip);

    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 120, 180);
    SDL_Rect accentShine = {x + 17, y + panelH - 10, 50, 2};
    SDL_RenderFillRect(r->sdlRenderer, &accentShine);
}

void Render_BombHUD(Renderer* r, Bomb bomb, int x, int y)
{
    if (r == NULL || bomb == NULL)
    {
        return;
    }

    SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);

    int panelW = 260;
    int panelH = 88;

    int safeTimer = getBombTimer(bomb);

    if (safeTimer < 0)
    {
        safeTimer = 0;
    }

    if (safeTimer > BOMB_TIMER)
    {
        safeTimer = BOMB_TIMER;
    }

    int timerSeconds = (safeTimer + 59) / 60;

    int isDanger = safeTimer <= (BOMB_TIMER * 0.33);
    int blinkOn = ((safeTimer / 8) % 2 == 0);

    /* ===== Danger glow behind panel ===== */

    if (isDanger && blinkOn)
    {
        SDL_Rect dangerGlow = {x - 8, y - 8, panelW + 16, panelH + 16};
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 60, 30, 70);
        SDL_RenderFillRect(r->sdlRenderer, &dangerGlow);
    }
    else
    {
        SDL_Rect softGlow = {x - 5, y - 5, panelW + 10, panelH + 10};
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 190, 65, 35);
        SDL_RenderFillRect(r->sdlRenderer, &softGlow);
    }

    /* ===== Strong shadow ===== */

    SDL_Rect deepShadow = {x + 8, y + 8, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 120);
    SDL_RenderFillRect(r->sdlRenderer, &deepShadow);

    SDL_Rect softShadow = {x + 4, y + 4, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 80);
    SDL_RenderFillRect(r->sdlRenderer, &softShadow);

    /* ===== Wooden panel layers ===== */

    SDL_Rect outerFrame = {x, y, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 55, 27, 10, 255);
    SDL_RenderFillRect(r->sdlRenderer, &outerFrame);

    SDL_Rect goldFrame = {x + 3, y + 3, panelW - 6, panelH - 6};
    SDL_SetRenderDrawColor(r->sdlRenderer, 235, 178, 72, 255);
    SDL_RenderFillRect(r->sdlRenderer, &goldFrame);

    SDL_Rect darkWood = {x + 7, y + 7, panelW - 14, panelH - 14};
    SDL_SetRenderDrawColor(r->sdlRenderer, 105, 57, 22, 255);
    SDL_RenderFillRect(r->sdlRenderer, &darkWood);

    SDL_Rect woodPanel = {x + 11, y + 11, panelW - 22, panelH - 22};
    SDL_SetRenderDrawColor(r->sdlRenderer, 211, 135, 55, 255);
    SDL_RenderFillRect(r->sdlRenderer, &woodPanel);

    SDL_Rect innerPanel = {x + 16, y + 16, panelW - 32, panelH - 32};
    SDL_SetRenderDrawColor(r->sdlRenderer, 235, 172, 85, 245);
    SDL_RenderFillRect(r->sdlRenderer, &innerPanel);

    /* Top shine */
    SDL_Rect topShine = {x + 18, y + 18, panelW - 36, 7};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 230, 145, 180);
    SDL_RenderFillRect(r->sdlRenderer, &topShine);

    SDL_Rect thinShine = {x + 22, y + 27, panelW - 44, 2};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 240, 180, 100);
    SDL_RenderFillRect(r->sdlRenderer, &thinShine);

    /* Bottom depth */
    SDL_Rect bottomDepth = {x + 10, y + panelH - 14, panelW - 20, 7};
    SDL_SetRenderDrawColor(r->sdlRenderer, 80, 42, 16, 160);
    SDL_RenderFillRect(r->sdlRenderer, &bottomDepth);

    /* Outlines */
    SDL_SetRenderDrawColor(r->sdlRenderer, 35, 16, 6, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &outerFrame);

    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 105, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &goldFrame);

    SDL_SetRenderDrawColor(r->sdlRenderer, 55, 28, 10, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &darkWood);

    /* ===== Decorative corner pieces ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 215, 95, 255);

    SDL_Rect c1 = {x + 3, y + 3, 18, 6};
    SDL_Rect c2 = {x + panelW - 21, y + 3, 18, 6};
    SDL_Rect c3 = {x + 3, y + panelH - 9, 18, 6};
    SDL_Rect c4 = {x + panelW - 21, y + panelH - 9, 18, 6};

    SDL_RenderFillRect(r->sdlRenderer, &c1);
    SDL_RenderFillRect(r->sdlRenderer, &c2);
    SDL_RenderFillRect(r->sdlRenderer, &c3);
    SDL_RenderFillRect(r->sdlRenderer, &c4);

    /* Small nails */
    SDL_SetRenderDrawColor(r->sdlRenderer, 95, 48, 18, 255);

    SDL_Rect nail1 = {x + 8, y + 8, 5, 5};
    SDL_Rect nail2 = {x + panelW - 13, y + 8, 5, 5};
    SDL_Rect nail3 = {x + 8, y + panelH - 13, 5, 5};
    SDL_Rect nail4 = {x + panelW - 13, y + panelH - 13, 5, 5};

    SDL_RenderFillRect(r->sdlRenderer, &nail1);
    SDL_RenderFillRect(r->sdlRenderer, &nail2);
    SDL_RenderFillRect(r->sdlRenderer, &nail3);
    SDL_RenderFillRect(r->sdlRenderer, &nail4);

    /* ===== Title plate ===== */

    SDL_Rect titlePlate = {x + 42, y + 12, 176, 24};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 205, 95, 95);
    SDL_RenderFillRect(r->sdlRenderer, &titlePlate);

    SDL_SetRenderDrawColor(r->sdlRenderer, 120, 65, 24, 140);
    SDL_RenderDrawRect(r->sdlRenderer, &titlePlate);

    SDL_Color titleShadow = {120, 70, 25, 180};
    SDL_Color titleColor = {45, 25, 10, 255};

    Render_HUDText(r, "BOMB TIMER", x + 57, y + 8, titleShadow);
    Render_HUDText(r, "BOMB TIMER", x + 55, y + 6, titleColor);

    /* ===== Timer number ===== */

    char timerText[16];
    snprintf(timerText, sizeof(timerText), "%d", timerSeconds);

    SDL_Color timerShadow = {120, 55, 20, 200};
    SDL_Color timerColor;

    if (safeTimer > (BOMB_TIMER * 0.66))
    {
        timerColor = (SDL_Color){255, 245, 185, 255};
    }
    else if (safeTimer > (BOMB_TIMER * 0.33))
    {
        timerColor = (SDL_Color){255, 230, 80, 255};
    }
    else
    {
        timerColor = (SDL_Color){255, 95, 55, 255};
    }

    Render_HUDText(r, timerText, x + 121, y + 34, timerShadow);
    Render_HUDText(r, timerText, x + 119, y + 32, timerColor);

    /* ===== Timer bar ===== */

    int barW = 212;
    int barH = 14;
    int barX = x + 24;
    int barY = y + 66;

    SDL_Rect barOuter = {barX - 4, barY - 4, barW + 8, barH + 8};
    SDL_SetRenderDrawColor(r->sdlRenderer, 45, 22, 8, 255);
    SDL_RenderFillRect(r->sdlRenderer, &barOuter);

    SDL_Rect barBg = {barX, barY, barW, barH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 72, 38, 17, 255);
    SDL_RenderFillRect(r->sdlRenderer, &barBg);

    int fillW = 0;

    if (BOMB_TIMER > 0)
    {
        fillW = (safeTimer * barW) / BOMB_TIMER;
    }

    SDL_Rect barFill = {barX, barY, fillW, barH};

    if (safeTimer > (BOMB_TIMER * 0.66))
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 65, 220, 75, 255);
    }
    else if (safeTimer > (BOMB_TIMER * 0.33))
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 215, 45, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 235, 55, 35, 255);
    }

    SDL_RenderFillRect(r->sdlRenderer, &barFill);

    /* Bar shine */
    if (fillW > 4)
    {
        SDL_Rect barShine = {barX + 2, barY + 2, fillW - 4, 3};
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 255, 210, 90);
        SDL_RenderFillRect(r->sdlRenderer, &barShine);
    }

    SDL_SetRenderDrawColor(r->sdlRenderer, 25, 12, 5, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &barOuter);
    SDL_RenderDrawRect(r->sdlRenderer, &barBg);

    /* ===== Small side fuse sparks when dangerous ===== */

    if (isDanger && blinkOn)
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 120, 0, 255);

        SDL_Rect sparkL1 = {x + 25, y + 38, 5, 5};
        SDL_Rect sparkR1 = {x + panelW - 30, y + 38, 5, 5};

        SDL_RenderFillRect(r->sdlRenderer, &sparkL1);
        SDL_RenderFillRect(r->sdlRenderer, &sparkR1);

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 240, 80, 255);

        SDL_Rect sparkL2 = {x + 27, y + 40, 2, 2};
        SDL_Rect sparkR2 = {x + panelW - 28, y + 40, 2, 2};

        SDL_RenderFillRect(r->sdlRenderer, &sparkL2);
        SDL_RenderFillRect(r->sdlRenderer, &sparkR2);
    }
}



void Render_Bomb(Renderer* r, Bomb bomb)
{
    if (r == NULL || bomb == NULL)
    {
        return;
    }

    float scaleX, scaleY;
    getScale(r, &scaleX, &scaleY);

    /* ===== Explosion visas först ===== */

    if (getBombExploding(bomb))
    {
        int cx = (int)((getBombX(bomb) + 14) * scaleX);
        int cy = (int)((getBombY(bomb) - 16) * scaleY);

        int outerRadius = (int)(46 * scaleX);
        int middleRadius = (int)(32 * scaleX);
        int innerRadius = (int)(18 * scaleX);

        if (outerRadius < 1) outerRadius = 1;
        if (middleRadius < 1) middleRadius = 1;
        if (innerRadius < 1) innerRadius = 1;

        SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);

        /* ===== Outer red/orange glow ===== */

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 60, 20, 120);

        for (int dy = -outerRadius; dy <= outerRadius; dy++)
        {
            for (int dx = -outerRadius; dx <= outerRadius; dx++)
            {
                if (dx * dx + dy * dy <= outerRadius * outerRadius)
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
                }
            }
        }

        /* ===== Middle orange explosion ===== */

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 135, 20, 210);

        for (int dy = -middleRadius; dy <= middleRadius; dy++)
        {
            for (int dx = -middleRadius; dx <= middleRadius; dx++)
            {
                if (dx * dx + dy * dy <= middleRadius * middleRadius)
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
                }
            }
        }

        /* ===== Bright yellow core ===== */

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 235, 55, 245);

        for (int dy = -innerRadius; dy <= innerRadius; dy++)
        {
            for (int dx = -innerRadius; dx <= innerRadius; dx++)
            {
                if (dx * dx + dy * dy <= innerRadius * innerRadius)
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
                }
            }
        }

        /* ===== Sharp burst lines ===== */

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 230, 70, 255);

        SDL_RenderDrawLine(r->sdlRenderer, cx, cy - 55, cx, cy - 20);
        SDL_RenderDrawLine(r->sdlRenderer, cx, cy + 20, cx, cy + 55);
        SDL_RenderDrawLine(r->sdlRenderer, cx - 55, cy, cx - 20, cy);
        SDL_RenderDrawLine(r->sdlRenderer, cx + 20, cy, cx + 55, cy);

        SDL_RenderDrawLine(r->sdlRenderer, cx - 38, cy - 38, cx - 15, cy - 15);
        SDL_RenderDrawLine(r->sdlRenderer, cx + 15, cy - 15, cx + 38, cy - 38);
        SDL_RenderDrawLine(r->sdlRenderer, cx - 38, cy + 38, cx - 15, cy + 15);
        SDL_RenderDrawLine(r->sdlRenderer, cx + 15, cy + 15, cx + 38, cy + 38);

        /* ===== Small sparks around explosion ===== */

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 120, 0, 230);

        SDL_Rect spark1 = {cx - 52, cy - 18, 7, 7};
        SDL_Rect spark2 = {cx + 46, cy - 24, 6, 6};
        SDL_Rect spark3 = {cx - 30, cy + 42, 6, 6};
        SDL_Rect spark4 = {cx + 26, cy + 38, 8, 8};

        SDL_RenderFillRect(r->sdlRenderer, &spark1);
        SDL_RenderFillRect(r->sdlRenderer, &spark2);
        SDL_RenderFillRect(r->sdlRenderer, &spark3);
        SDL_RenderFillRect(r->sdlRenderer, &spark4);

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 245, 90, 255);

        SDL_Rect sparkCore1 = {cx - 50, cy - 16, 3, 3};
        SDL_Rect sparkCore2 = {cx + 48, cy - 22, 2, 2};
        SDL_Rect sparkCore3 = {cx - 28, cy + 44, 2, 2};
        SDL_Rect sparkCore4 = {cx + 29, cy + 41, 3, 3};

        SDL_RenderFillRect(r->sdlRenderer, &sparkCore1);
        SDL_RenderFillRect(r->sdlRenderer, &sparkCore2);
        SDL_RenderFillRect(r->sdlRenderer, &sparkCore3);
        SDL_RenderFillRect(r->sdlRenderer, &sparkCore4);

        /* ===== Dark smoke chunks ===== */

        SDL_SetRenderDrawColor(r->sdlRenderer, 45, 35, 30, 120);

        SDL_Rect smoke1 = {cx - 40, cy - 10, 14, 10};
        SDL_Rect smoke2 = {cx + 28, cy + 4, 16, 11};
        SDL_Rect smoke3 = {cx - 8, cy + 28, 15, 10};

        SDL_RenderFillRect(r->sdlRenderer, &smoke1);
        SDL_RenderFillRect(r->sdlRenderer, &smoke2);
        SDL_RenderFillRect(r->sdlRenderer, &smoke3);

        return;
    }

    /* ===== Om bomben inte är aktiv, rita inget ===== */

    if (!getBombActive(bomb))
    {
        return;
    }

    SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);

    int maxTimer = BOMB_TIMER;

    if (maxTimer <= 0)
    {
        maxTimer = 1;
    }

    int safeTimer = getBombTimer(bomb);

    if (safeTimer < 0)
    {
        safeTimer = 0;
    }

    if (safeTimer > maxTimer)
    {
        safeTimer = maxTimer;
    }

    int isDanger = safeTimer <= (BOMB_TIMER * 0.33);
    int blinkOn = ((safeTimer / 5) % 2 == 0);

    /*
        Bomb position in screen coordinates.
        getBombX/Y is game/world coordinate, so we scale it.
    */
    int cx = (int)((getBombX(bomb) + 14) * scaleX);
    int cy = (int)((getBombY(bomb) - 16) * scaleY);

    int radius = (int)(14 * scaleX);

    if (radius < 6)
    {
        radius = 6;
    }

    /* ===== Soft shadow under bomb ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 90);

    for (int dy = -5; dy <= 5; dy++)
    {
        for (int dx = -14; dx <= 14; dx++)
        {
            if ((dx * dx) / 2 + dy * dy <= 70)
            {
                SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + radius + 7 + dy);
            }
        }
    }

    /* ===== Danger glow when timer is low ===== */

    if (isDanger && blinkOn)
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 70, 25, 90);

        int glowRadius = radius + 8;

        for (int dy = -glowRadius; dy <= glowRadius; dy++)
        {
            for (int dx = -glowRadius; dx <= glowRadius; dx++)
            {
                if (dx * dx + dy * dy <= glowRadius * glowRadius)
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
                }
            }
        }
    }
    else
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 210, 60, 45);

        int glowRadius = radius + 5;

        for (int dy = -glowRadius; dy <= glowRadius; dy++)
        {
            for (int dx = -glowRadius; dx <= glowRadius; dx++)
            {
                if (dx * dx + dy * dy <= glowRadius * glowRadius)
                {
                    SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
                }
            }
        }
    }

    /* ===== Bomb body ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 18, 18, 20, 255);

    for (int dy = -radius; dy <= radius; dy++)
    {
        for (int dx = -radius; dx <= radius; dx++)
        {
            if (dx * dx + dy * dy <= radius * radius)
            {
                SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
            }
        }
    }

    /* Dark outline */
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);

    for (int dy = -radius; dy <= radius; dy++)
    {
        for (int dx = -radius; dx <= radius; dx++)
        {
            int dist = dx * dx + dy * dy;

            if (dist <= radius * radius && dist >= (radius - 2) * (radius - 2))
            {
                SDL_RenderDrawPoint(r->sdlRenderer, cx + dx, cy + dy);
            }
        }
    }

    /* ===== Bomb shine ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 105, 105, 110, 255);

    int shineRadius = 4;

    for (int dy = -shineRadius; dy <= shineRadius; dy++)
    {
        for (int dx = -shineRadius; dx <= shineRadius; dx++)
        {
            if (dx * dx + dy * dy <= shineRadius * shineRadius)
            {
                SDL_RenderDrawPoint(r->sdlRenderer, cx - 5 + dx, cy - 5 + dy);
            }
        }
    }

    /* ===== Bomb cap ===== */

    SDL_Rect cap = {cx + 2, cy - radius - 5, 10, 6};
    SDL_SetRenderDrawColor(r->sdlRenderer, 90, 90, 90, 255);
    SDL_RenderFillRect(r->sdlRenderer, &cap);

    SDL_SetRenderDrawColor(r->sdlRenderer, 20, 20, 20, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &cap);

    /* ===== Fuse ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 120, 65, 25, 255);

    SDL_RenderDrawLine(
        r->sdlRenderer,
        cx + 10,
        cy - radius - 5,
        cx + 18,
        cy - radius - 14
    );

    SDL_RenderDrawLine(
        r->sdlRenderer,
        cx + 18,
        cy - radius - 14,
        cx + 24,
        cy - radius - 10
    );

    /* ===== Spark ===== */

    if (blinkOn)
    {
        int sparkX = cx + 25;
        int sparkY = cy - radius - 11;

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 120, 0, 255);

        SDL_RenderDrawLine(r->sdlRenderer, sparkX - 4, sparkY, sparkX + 4, sparkY);
        SDL_RenderDrawLine(r->sdlRenderer, sparkX, sparkY - 4, sparkX, sparkY + 4);

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 235, 70, 255);

        SDL_Rect sparkCore = {sparkX - 2, sparkY - 2, 4, 4};
        SDL_RenderFillRect(r->sdlRenderer, &sparkCore);
    }

    /* ===== Small timer bar above bomb ===== */

    int barWidth = 30;
    int barHeight = 5;
    int barX = cx - (barWidth / 2);
    int barY = cy - radius - 18;

    SDL_Rect barOuter = {barX - 2, barY - 2, barWidth + 4, barHeight + 4};
    SDL_SetRenderDrawColor(r->sdlRenderer, 20, 20, 20, 220);
    SDL_RenderFillRect(r->sdlRenderer, &barOuter);

    SDL_Rect barBg = {barX, barY, barWidth, barHeight};
    SDL_SetRenderDrawColor(r->sdlRenderer, 65, 45, 30, 255);
    SDL_RenderFillRect(r->sdlRenderer, &barBg);

    int currentWidth = (safeTimer * barWidth) / maxTimer;

    SDL_Rect barFill = {barX, barY, currentWidth, barHeight};

    if (safeTimer > (BOMB_TIMER * 0.66))
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 65, 220, 75, 255);
    }
    else if (safeTimer > (BOMB_TIMER * 0.33))
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 215, 45, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 235, 55, 35, 255);
    }

    SDL_RenderFillRect(r->sdlRenderer, &barFill);

    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &barOuter);
    SDL_RenderDrawRect(r->sdlRenderer, &barBg);
}

