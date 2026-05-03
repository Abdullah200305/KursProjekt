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
    
    SDL_SetRenderDrawColor(r->sdlRenderer,0, 255, 0, 255); // to test the player render
    SDL_RenderFillRect(r->sdlRenderer, &playerRect);   
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

void Render_PlayerLives(Renderer* r, Player player, int startX, int startY) {
    int lives = getPlayerLives(player);
    int size = 4;      // storlek på varje liten blockbit
    int spacing = 30;  // avstånd mellan hjärtan

    for (int h = 0; h < lives; h++) {
        int x = startX + h * spacing;
        int y = startY;

        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255);

        // övre vänster
        SDL_Rect a = {x + size, y, size, size};
        SDL_Rect b = {x + 2 * size, y, size, size};

        // övre höger
        SDL_Rect c = {x + 4 * size, y, size, size};
        SDL_Rect d = {x + 5 * size, y, size, size};

        // andra raden
        SDL_Rect e = {x, y + size, size, size};
        SDL_Rect f = {x + size, y + size, size, size};
        SDL_Rect g = {x + 2 * size, y + size, size, size};
        SDL_Rect h1 = {x + 3 * size, y + size, size, size};
        SDL_Rect i = {x + 4 * size, y + size, size, size};
        SDL_Rect j = {x + 5 * size, y + size, size, size};
        SDL_Rect k = {x + 6 * size, y + size, size, size};

        // tredje raden
        SDL_Rect l = {x + size, y + 2 * size, size, size};
        SDL_Rect m = {x + 2 * size, y + 2 * size, size, size};
        SDL_Rect n = {x + 3 * size, y + 2 * size, size, size};
        SDL_Rect o = {x + 4 * size, y + 2 * size, size, size};
        SDL_Rect p = {x + 5 * size, y + 2 * size, size, size};

        // fjärde raden
        SDL_Rect q = {x + 2 * size, y + 3 * size, size, size};
        SDL_Rect r1 = {x + 3 * size, y + 3 * size, size, size};
        SDL_Rect s = {x + 4 * size, y + 3 * size, size, size};

        // nedersta spetsen
        SDL_Rect t = {x + 3 * size, y + 4 * size, size, size};

        SDL_RenderFillRect(r->sdlRenderer, &a);
        SDL_RenderFillRect(r->sdlRenderer, &b);
        SDL_RenderFillRect(r->sdlRenderer, &c);
        SDL_RenderFillRect(r->sdlRenderer, &d);
        SDL_RenderFillRect(r->sdlRenderer, &e);
        SDL_RenderFillRect(r->sdlRenderer, &f);
        SDL_RenderFillRect(r->sdlRenderer, &g);
        SDL_RenderFillRect(r->sdlRenderer, &h1);
        SDL_RenderFillRect(r->sdlRenderer, &i);
        SDL_RenderFillRect(r->sdlRenderer, &j);
        SDL_RenderFillRect(r->sdlRenderer, &k);
        SDL_RenderFillRect(r->sdlRenderer, &l);
        SDL_RenderFillRect(r->sdlRenderer, &m);
        SDL_RenderFillRect(r->sdlRenderer, &n);
        SDL_RenderFillRect(r->sdlRenderer, &o);
        SDL_RenderFillRect(r->sdlRenderer, &p);
        SDL_RenderFillRect(r->sdlRenderer, &q);
        SDL_RenderFillRect(r->sdlRenderer, &r1);
        SDL_RenderFillRect(r->sdlRenderer, &s);
        SDL_RenderFillRect(r->sdlRenderer, &t);
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


void Render_PlayerHUD(Renderer* r, Player player, int playerIndex, int x, int y)
{
    if (r == NULL || player == NULL)
    {
        return;
    }

    SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);

    /* ===== Adventure Island HUD panel ===== */

    SDL_Rect shadow = {x + 4, y + 4, 220, 70};
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 120);
    SDL_RenderFillRect(r->sdlRenderer, &shadow);

    SDL_Rect outerFrame = {x, y, 220, 70};
    SDL_SetRenderDrawColor(r->sdlRenderer, 92, 49, 18, 255);
    SDL_RenderFillRect(r->sdlRenderer, &outerFrame);

    SDL_Rect woodPanel = {x + 5, y + 5, 210, 60};
    SDL_SetRenderDrawColor(r->sdlRenderer, 176, 104, 42, 255);
    SDL_RenderFillRect(r->sdlRenderer, &woodPanel);

    SDL_Rect innerPanel = {x + 10, y + 10, 200, 50};
    SDL_SetRenderDrawColor(r->sdlRenderer, 222, 161, 77, 240);
    SDL_RenderFillRect(r->sdlRenderer, &innerPanel);

    SDL_Rect highlight = {x + 12, y + 12, 196, 6};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 130, 160);
    SDL_RenderFillRect(r->sdlRenderer, &highlight);

    SDL_SetRenderDrawColor(r->sdlRenderer, 55, 28, 10, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &outerFrame);
    SDL_RenderDrawRect(r->sdlRenderer, &woodPanel);

    /* ===== Player portrait placeholder ===== */

    SDL_Rect portraitFrame = {x + 14, y + 14, 46, 46};
    SDL_SetRenderDrawColor(r->sdlRenderer, 65, 35, 18, 255);
    SDL_RenderFillRect(r->sdlRenderer, &portraitFrame);

    SDL_Rect portrait = {x + 19, y + 19, 36, 36};

    if (playerIndex == 0)
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 210, 70, 150, 255);
    }
    else if (playerIndex == 1)
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 80, 120, 230, 255);
    }
    else if (playerIndex == 2)
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 70, 190, 90, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 230, 200, 70, 255);
    }

    SDL_RenderFillRect(r->sdlRenderer, &portrait);

    SDL_SetRenderDrawColor(r->sdlRenderer, 20, 20, 20, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &portraitFrame);

    

    /* ===== Player name + HP ===== */

    SDL_Color darkText = {45, 25, 10, 255};
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

    Render_HUDText(r, playerName, x + 78, y + 8, darkText);

    SDL_Rect hpBackground = {x + 78, y + 34, 122, 26};
    SDL_SetRenderDrawColor(r->sdlRenderer, 90, 50, 25, 180);
    SDL_RenderFillRect(r->sdlRenderer, &hpBackground);

    SDL_SetRenderDrawColor(r->sdlRenderer, 45, 25, 12, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &hpBackground);

    Render_HUDText(r, "HP", x + 84, y + 36, lightText);
    Render_PlayerLives(r, player, x + 118, y + 38);

    /* ===== Small rope-like corner decorations ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 230, 190, 95, 255);

    SDL_Rect rope1 = {x + 4, y + 4, 12, 5};
    SDL_Rect rope2 = {x + 204, y + 4, 12, 5};
    SDL_Rect rope3 = {x + 4, y + 61, 12, 5};
    SDL_Rect rope4 = {x + 204, y + 61, 12, 5};

    SDL_RenderFillRect(r->sdlRenderer, &rope1);
    SDL_RenderFillRect(r->sdlRenderer, &rope2);
    SDL_RenderFillRect(r->sdlRenderer, &rope3);
    SDL_RenderFillRect(r->sdlRenderer, &rope4);
}

void Render_BombHUD(Renderer* r, Bomb bomb, int x, int y)
{
    if (r == NULL || bomb == NULL)
    {
        return;
    }

    SDL_SetRenderDrawBlendMode(r->sdlRenderer, SDL_BLENDMODE_BLEND);

    int panelW = 260;
    int panelH = 82;

    /* ===== Shadow ===== */

    SDL_Rect shadow = {x + 5, y + 5, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 130);
    SDL_RenderFillRect(r->sdlRenderer, &shadow);

    /* ===== Wooden panel ===== */

    SDL_Rect outerFrame = {x, y, panelW, panelH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 92, 49, 18, 255);
    SDL_RenderFillRect(r->sdlRenderer, &outerFrame);

    SDL_Rect woodPanel = {x + 6, y + 6, panelW - 12, panelH - 12};
    SDL_SetRenderDrawColor(r->sdlRenderer, 176, 104, 42, 255);
    SDL_RenderFillRect(r->sdlRenderer, &woodPanel);

    SDL_Rect innerPanel = {x + 12, y + 12, panelW - 24, panelH - 24};
    SDL_SetRenderDrawColor(r->sdlRenderer, 222, 161, 77, 240);
    SDL_RenderFillRect(r->sdlRenderer, &innerPanel);

    SDL_Rect highlight = {x + 14, y + 14, panelW - 28, 7};
    SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 130, 160);
    SDL_RenderFillRect(r->sdlRenderer, &highlight);

    SDL_SetRenderDrawColor(r->sdlRenderer, 55, 28, 10, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &outerFrame);
    SDL_RenderDrawRect(r->sdlRenderer, &woodPanel);

    /* ===== Text ===== */

    SDL_Color darkText = {45, 25, 10, 255};
    SDL_Color lightText = {255, 235, 170, 255};

    Render_HUDText(r, "BOMB TIMER", x + 62, y + 8, darkText);

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

    char timerText[16];
    snprintf(timerText, sizeof(timerText), "%d", timerSeconds);

    Render_HUDText(r, timerText, x + 120, y + 32, lightText);

    /* ===== Timer bar ===== */

    int barW = 210;
    int barH = 12;
    int barX = x + 25;
    int barY = y + 62;

    SDL_Rect barBg = {barX, barY, barW, barH};
    SDL_SetRenderDrawColor(r->sdlRenderer, 70, 38, 18, 220);
    SDL_RenderFillRect(r->sdlRenderer, &barBg);

    int fillW = 0;

    if (BOMB_TIMER > 0)
    {
        fillW = (safeTimer * barW) / BOMB_TIMER;
    }

    SDL_Rect barFill = {barX, barY, fillW, barH};

    if (safeTimer > (BOMB_TIMER * 0.66))
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 60, 210, 70, 255);
    }
    else if (safeTimer > (BOMB_TIMER * 0.33))
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 245, 210, 55, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(r->sdlRenderer, 230, 55, 35, 255);
    }

    SDL_RenderFillRect(r->sdlRenderer, &barFill);

    SDL_SetRenderDrawColor(r->sdlRenderer, 35, 18, 8, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &barBg);

    /* ===== Small rope-like corner decorations ===== */

    SDL_SetRenderDrawColor(r->sdlRenderer, 230, 190, 95, 255);

    SDL_Rect rope1 = {x + 5, y + 5, 14, 5};
    SDL_Rect rope2 = {x + panelW - 19, y + 5, 14, 5};
    SDL_Rect rope3 = {x + 5, y + panelH - 10, 14, 5};
    SDL_Rect rope4 = {x + panelW - 19, y + panelH - 10, 14, 5};

    SDL_RenderFillRect(r->sdlRenderer, &rope1);
    SDL_RenderFillRect(r->sdlRenderer, &rope2);
    SDL_RenderFillRect(r->sdlRenderer, &rope3);
    SDL_RenderFillRect(r->sdlRenderer, &rope4);
}



void Render_Bomb(Renderer* r, Bomb bomb) {
    /* ===== Explosion visas först ===== */

    float scaleX, scaleY;
    getScale(r, &scaleX, &scaleY);

    if (getBombExploding(bomb)) 
    {

        SDL_Rect explosionOuter = { ((int)getBombX(bomb) - 20) * scaleX, ((int)getBombY(bomb) - 50) * scaleY, 70 * scaleX, 70 * scaleY };
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 80, 0, 255);   // orange
        SDL_RenderFillRect(r->sdlRenderer, &explosionOuter);

        SDL_Rect explosionInner = {  ((int)getBombX(bomb) - 8) * scaleX, ((int)getBombY(bomb) - 38) * scaleY, 46 * scaleX, 46 * scaleY};
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 0, 255);  // gul mitt
        SDL_RenderFillRect(r->sdlRenderer, &explosionInner);

        SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(r->sdlRenderer, &explosionOuter);

        return;
    }

    /* ===== Om bomben inte är aktiv, rita inget ===== */
    if (!getBombActive(bomb)) {
        return;
    }

    int maxTimer = BOMB_TIMER;
    int bodyX = (int)getBombX(bomb) + 2;
    int bodyY = (int)getBombY(bomb) - 28;

    /* ===== Bombkropp ===== */
    SDL_Rect body = { bodyX * scaleX, bodyY * scaleY, 24 * scaleX, 24 *scaleY};
    SDL_SetRenderDrawColor(r->sdlRenderer, 20, 20, 20, 255);
    SDL_RenderFillRect(r->sdlRenderer, &body);

    /* Lite highlight */
    SDL_Rect shine = { (bodyX + 4) * scaleX, (bodyY + 4) * scaleY, 6 * scaleX, 6 *scaleY};
    SDL_SetRenderDrawColor(r->sdlRenderer, 120, 120, 120, 255);
    SDL_RenderFillRect(r->sdlRenderer, &shine);

    /* Kontur */
    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &body);

    /* ===== Toppdel ===== */
    SDL_Rect cap = { (bodyX + 8) * scaleX, (bodyY - 4) * scaleY, 8 * scaleX, 5 *scaleY};
    SDL_SetRenderDrawColor(r->sdlRenderer, 100, 100, 100, 255);
    SDL_RenderFillRect(r->sdlRenderer, &cap);

    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &cap);

    /* ===== Säkring ===== */
    SDL_SetRenderDrawColor(r->sdlRenderer, 139, 69, 19, 255);
    SDL_RenderDrawLine(r->sdlRenderer, bodyX + 12, bodyY - 4, bodyX + 18, bodyY - 10);
    SDL_RenderDrawLine(r->sdlRenderer, bodyX + 18, bodyY - 10, bodyX + 22, bodyY - 6);

    /* ===== Gnista ===== */
    if ((getBombTimer(bomb) / 5) % 2 == 0) {
        SDL_Rect spark1 = { (bodyX + 21) * scaleX, (bodyY - 8) * scaleY, 4 * scaleX, 4 * scaleY};
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 140, 0, 255);
        SDL_RenderFillRect(r->sdlRenderer, &spark1);

        SDL_Rect spark2 = { (bodyX + 23) * scaleX, (bodyY - 10) * scaleY, 2 * scaleX, 2 * scaleY};
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 255, 0, 255);
        SDL_RenderFillRect(r->sdlRenderer, &spark2);
    }

    /* ===== Timer-bar ===== */
    int barWidth = 24;
    int barHeight = 5;
    int barX = bodyX;
    int barY = bodyY - 12;

    int safeTimer = getBombTimer(bomb);
    if (safeTimer < 0) safeTimer = 0;
    if (safeTimer > maxTimer) safeTimer = BOMB_TIMER;

    int currentWidth = (safeTimer * barWidth) / maxTimer;

    SDL_Rect barBg = { barX * scaleX, barY * scaleY, barWidth * scaleX, barHeight * scaleY};
    SDL_SetRenderDrawColor(r->sdlRenderer, 60, 60, 60, 255);
    SDL_RenderFillRect(r->sdlRenderer, &barBg);

    SDL_Rect barFill = { barX * scaleX, barY * scaleY, currentWidth * scaleX, barHeight * scaleY};

    if (safeTimer > (BOMB_TIMER*0.66)) {
        SDL_SetRenderDrawColor(r->sdlRenderer, 0, 255, 0, 255);
    } else if (safeTimer > (BOMB_TIMER*0.33)) {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0, 0, 255);
    }

    SDL_RenderFillRect(r->sdlRenderer, &barFill);

    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &barBg);
}


