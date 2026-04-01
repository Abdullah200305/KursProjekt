#include "Game_controll.h"


void game_update(Game* game, Renderer* renderer) {
 
        Background_Image_Render(renderer); 
        // Renderer_Clear(renderer); 
        Render_Map(renderer, game->map);
        Renderer_Present(renderer);
 
}

void game_loop(Game* game, Renderer* renderer) {
    SDL_Event event;
    while (game->state != GAME_STATE_GAME_OVER) {
          // Handle events such as window close, keyboard input, etc.
          while (SDL_PollEvent(&event))
            {
               
                switch (event.type)
                {
                case SDL_QUIT:
                    game->state = GAME_STATE_GAME_OVER;
                    break;
                default:
                    break;
                }
            }
        game_update(game, renderer);
        SDL_Delay(16); // Delay to cap the frame rate (approximately 60 FPS)
    }
}



void game_cleanup(Game* game, Renderer* renderer) {
    Map_destroy(game->map);
    Renderer_Destroy(renderer);
}


void game_init(Game* game,Renderer* renderer) {   
    game->map = Map_create(WIDTH, HEIGHT);
    game->state = GAME_STATE_PLAYING;
    Renderer_Init(renderer, "Hello, World!", game->map->width, game->map->height);  
}