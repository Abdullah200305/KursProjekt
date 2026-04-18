#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "protocol.h"
#include "player.h"

#define MAX_PLAYERS 4

typedef struct PlayerManager_type *PlayerManager;


PlayerManager PlayerManager_init();
void PlayerManager_destroy(PlayerManager pm);


int PlayerManager_add(PlayerManager pm);
void PlayerManager_remove(PlayerManager pm, int id);
Player* PlayerManager_get(PlayerManager pm, int id);


void PlayerManager_applyInput(PlayerManager pm, Packet p);
void PlayerManager_update(PlayerManager pm, float dt);
void PlayerManager_clearInput(PlayerManager pm);

#endif