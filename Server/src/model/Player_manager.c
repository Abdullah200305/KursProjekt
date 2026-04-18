#include "Player_manager.h"


struct PlayerManager_type
{
    Player players[MAX_PLAYERS];
    int count;
};


int PlayerManager_add(PlayerManager pm)
{
    if (pm->count >= MAX_PLAYERS) return -1;
    int id = pm->count;
    pm->players[id] = initPlayer(200, 300, id);
    pm->count++;
    return id;  
}


PlayerManager PlayerManager_init()
{   
PlayerManager pm = malloc(sizeof(struct PlayerManager_type));
pm->count = 0;
return pm;
}


  

void PlayerManager_remove(PlayerManager pm, int id){
    if (id < 0 || id >= pm->count) return;
    pm->players[id] = pm->players[pm->count - 1];
    setPlayerId(pm->players[id],id);
    pm->count--;
    PlayerDestroy(pm->players[id]);
}

void PlayerManager_destroy(PlayerManager pm){
    free(pm);
}




