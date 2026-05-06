#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H
#include "renderer.h"
#include "Game_state.h"
#include "Sound.h"
#include "client_net.h"
#include <stdio.h>



void menu_loop(Game *game, Renderer *renderer);
void menu_init(Game* game,Renderer* renderer);
void menu_cleanup(Game* game, Renderer* renderer);





void choose_host_join_loop(Game *game, Renderer *renderer);
void host_setup_loop(Game *game, Renderer *renderer,ClientNet *clientNet);
void client_setup_loop(Game *game, Renderer *renderer,ClientNet *clientNet);

#endif // MENU_CONTROLLER_H