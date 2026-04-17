#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "protocol.h"
#include "player.h"

#define MAX_PLAYERS 4

typedef struct PlayerManager_type *PlayerManager;

// lifecycle
void PlayerManager_init(PlayerManager pm);
void PlayerManager_destroy(PlayerManager pm);

// player control
int PlayerManager_add(PlayerManager pm);
void PlayerManager_remove(PlayerManager pm, int id);
Player* PlayerManager_get(PlayerManager pm, int id);

// gameplay
void PlayerManager_applyInput(PlayerManager pm, Packet p);
void PlayerManager_update(PlayerManager pm, float dt);
void PlayerManager_clearInput(PlayerManager pm);

#endif