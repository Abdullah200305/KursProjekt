#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"
    int main(int argc, char *argv[])
    {
        Game* game;
        Renderer renderer;
        SDL_Event event;
        game->map = Map_create();
        game->state = GAME_STATE_PLAYING;
        printf("Hello, World!\n");
        fflush(stdout);



        if(Renderer_Init(&renderer, "Hello, World!", game->map->width, game->map->height) != 0) {
            return -1;
        }
          

   
        while (game->state == GAME_STATE_PLAYING)
        {
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
            
           Renderer_Clear(&renderer);
           Render_Map(&renderer, game->map);
           Renderer_Present(&renderer);
          

        }
        



        Map_destroy(game->map);
        Renderer_Destroy(&renderer);
        return 0;
    }



