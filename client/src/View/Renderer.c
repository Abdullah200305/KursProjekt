#include "Renderer.h"


int Renderer_Init(Renderer* r, const char* title, int width, int height) {
    printf("Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
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

    for (int i = 0; i < 14; i++) {
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



    //UI_playerState 
    r->UI_playerState = IMG_LoadTexture(r->sdlRenderer,"link/UI_state/UI_Player.png");
    
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
    
    //SDL_SetRenderDrawColor(r->sdlRenderer,0, 255, 0, 255); // to test the player render
    //SDL_RenderFillRect(r->sdlRenderer, &playerRect);  
     
    SDL_RenderCopy(r->sdlRenderer, img, &r->playerFrames[frame], &playerRect);
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



void Render_Bomb(Renderer* r, Bomb bomb) {
    float scaleX, scaleY;
    getScale(r, &scaleX, &scaleY);

    /* ===== Explosion visas först ===== */
    if (getBombExploding(bomb)) 
    {
        SDL_Rect explosionOuter = { 
            (int)((getBombX(bomb) - 20) * scaleX), 
            (int)((getBombY(bomb) - 50) * scaleY), 
            (int)(70 * scaleX), 
            (int)(70 * scaleY) 
        };
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 80, 0, 255);
        SDL_RenderFillRect(r->sdlRenderer, &explosionOuter);

        SDL_Rect explosionInner = { 
            (int)((getBombX(bomb) - 8)  * scaleX), 
            (int)((getBombY(bomb) - 38) * scaleY), 
            (int)(46 * scaleX), 
            (int)(46 * scaleY) 
        };
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 220, 0, 255);
        SDL_RenderFillRect(r->sdlRenderer, &explosionInner);

        SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(r->sdlRenderer, &explosionOuter);
        return;
    }

    /* ===== Om bomben inte är aktiv, rita inget ===== */
    if (!getBombActive(bomb)) return;

    int maxTimer = BOMB_TIMER;   
    int bodyX = (int)getBombX(bomb) + 2;
    int bodyY = (int)getBombY(bomb) - 28;

    /* ===== Bombkropp ===== */
    SDL_Rect body = { 
        (int)(bodyX * scaleX), 
        (int)(bodyY * scaleY), 
        (int)(24 * scaleX), 
        (int)(24 * scaleY) 
    };
    SDL_SetRenderDrawColor(r->sdlRenderer, 20, 20, 20, 255);
    SDL_RenderFillRect(r->sdlRenderer, &body);

    SDL_Rect shine = { 
        (int)((bodyX + 4) * scaleX), 
        (int)((bodyY + 4) * scaleY), 
        (int)(6 * scaleX), 
        (int)(6 * scaleY) 
    };
    SDL_SetRenderDrawColor(r->sdlRenderer, 120, 120, 120, 255);
    SDL_RenderFillRect(r->sdlRenderer, &shine);

    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &body);

    /* ===== Toppdel ===== */
    SDL_Rect cap = { 
        (int)((bodyX + 8) * scaleX), 
        (int)((bodyY - 4) * scaleY), 
        (int)(8 * scaleX), 
        (int)(5 * scaleY) 
    };
    SDL_SetRenderDrawColor(r->sdlRenderer, 100, 100, 100, 255);
    SDL_RenderFillRect(r->sdlRenderer, &cap);

    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &cap);

    /* ===== Säkring (BUG 2 fixed: now scaled) ===== */
    SDL_SetRenderDrawColor(r->sdlRenderer, 139, 69, 19, 255);
    SDL_RenderDrawLine(r->sdlRenderer,
        (int)((bodyX + 12) * scaleX), (int)((bodyY - 4)  * scaleY),
        (int)((bodyX + 18) * scaleX), (int)((bodyY - 10) * scaleY));
    SDL_RenderDrawLine(r->sdlRenderer,
        (int)((bodyX + 18) * scaleX), (int)((bodyY - 10) * scaleY),
        (int)((bodyX + 22) * scaleX), (int)((bodyY - 6)  * scaleY));

    /* ===== Gnista ===== */
    if ((getBombTimer(bomb) / 5) % 2 == 0) {
        SDL_Rect spark1 = { 
            (int)((bodyX + 21) * scaleX), 
            (int)((bodyY - 8)  * scaleY), 
            (int)(4 * scaleX), 
            (int)(4 * scaleY) 
        };
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 140, 0, 255);
        SDL_RenderFillRect(r->sdlRenderer, &spark1);

        SDL_Rect spark2 = { 
            (int)((bodyX + 23) * scaleX), 
            (int)((bodyY - 10) * scaleY), 
            (int)(2 * scaleX), 
            (int)(2 * scaleY) 
        };
        SDL_SetRenderDrawColor(r->sdlRenderer, 255, 255, 0, 255);
        SDL_RenderFillRect(r->sdlRenderer, &spark2);
    }

    /* ===== Timer-bar ===== */
    int barWidth  = 24;
    int barHeight = 5;
    int barX = bodyX;
    int barY = bodyY - 12;

    int safeTimer = getBombTimer(bomb);
    if (safeTimer < 0)        safeTimer = 0;
    if (safeTimer > maxTimer) safeTimer = maxTimer;  

    int currentWidth = (safeTimer * barWidth) / maxTimer;

    SDL_Rect barBg = { 
        (int)(barX * scaleX), 
        (int)(barY * scaleY), 
        (int)(barWidth  * scaleX), 
        (int)(barHeight * scaleY) 
    };
    SDL_SetRenderDrawColor(r->sdlRenderer, 60, 60, 60, 255);
    SDL_RenderFillRect(r->sdlRenderer, &barBg);

    SDL_Rect barFill = { 
        (int)(barX * scaleX), 
        (int)(barY * scaleY), 
        (int)(currentWidth * scaleX), 
        (int)(barHeight    * scaleY) 
    };

   
    if      (safeTimer > (int)(maxTimer * 0.66f)) SDL_SetRenderDrawColor(r->sdlRenderer, 0,   255, 0,   255);
    else if (safeTimer > (int)(maxTimer * 0.33f)) SDL_SetRenderDrawColor(r->sdlRenderer, 255, 255, 0,   255);
    else                                          SDL_SetRenderDrawColor(r->sdlRenderer, 255, 0,   0,   255);

    SDL_RenderFillRect(r->sdlRenderer, &barFill);

    SDL_SetRenderDrawColor(r->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(r->sdlRenderer, &barBg);
}












void AbilitySystem_render(AbilitySystem *system, Renderer *r)
{
    float scaleX, scaleY;
    getScale(r, &scaleX, &scaleY);

    SDL_Rect rect;
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        if (!AbilityItem_isActive(system, i))
            continue;

        rect.x = (int)(AbilityItem_getX(system, i) * scaleX);
        rect.y = (int)(AbilityItem_getY(system, i) * scaleY);
        rect.w = (int)(AbilityItem_getWidth(system, i)  * scaleX);
        rect.h = (int)(AbilityItem_getHeight(system, i) * scaleY);

        SDL_Texture *tex = r->abilityTextures[AbilityItem_getType(system, i)];
        if (tex)
            SDL_RenderCopy(r->sdlRenderer, tex, NULL, &rect);
    }
}












void UI_playerState(Player player[4], Renderer *r) {
    float scaleX, scaleY;
    getScale(r, &scaleX, &scaleY);

    /* ===== Bigger Background box ===== */
    int boxX = (int)(160 * scaleX);     // moved slightly left
    int boxY = (int)(500 * scaleY);     // slightly higher
    int boxW = (int)(1000 * scaleX);    // bigger width
    int boxH = (int)(260 * scaleY);     // bigger height

    SDL_Rect box = { boxX, boxY, boxW, boxH };

    SDL_RenderCopy(r->sdlRenderer, r->UI_playerState, NULL, &box);

    /* ===== Slots (smaller + more breathing space) ===== */
    int numSlots = 2;

    // Smaller slots
    int slotW = (int)(boxW * 0.25f);   // ↓ from 35% → 25%
    int slotH = (int)(boxH * 0.55f);   // ↓ from 65% → 55%

    // More spacing
    int gap = (int)(boxW * 0.08f);     // ↑ more gap between slots

    // Center vertically
    int startY = boxY + (boxH - slotH) / 2;

    // Center horizontally
    int totalSlotsW = numSlots * slotW + (numSlots - 1) * gap;
    int startX = boxX + (boxW - totalSlotsW) / 2;

    for (int i = 0; i < numSlots; i++)
    {
        int slotX = startX + i * (slotW + gap);

        SDL_Rect slot = { slotX, startY, slotW, slotH };

        SDL_SetRenderDrawColor(r->sdlRenderer, 70, 40, 15, 200);
        SDL_RenderFillRect(r->sdlRenderer, &slot);

        SDL_SetRenderDrawColor(r->sdlRenderer, 200, 150, 80, 255);
        SDL_RenderDrawRect(r->sdlRenderer, &slot);

        /* Lives — keep nicely inside */
        int livesX = (int)(slotX / scaleX) + 12;
        int livesY = (int)((startY + slotH * 0.5f) / scaleY);

        Render_PlayerLives(r, player[i], livesX, livesY);
    }
}

















































































































