#include "Player_manager.h"


struct PlayerManager_type
{
    Player players[MAX_PLAYERS];
    int count;
};







// void PlayerManager_init(PlayerManager *pm)
// {
//     pm->count = 0;
// }

// int PlayerManager_add(PlayerManager *pm)
// {
//     if (pm->count >= MAX_PLAYERS)
//         return -1;

//     Player *p = &pm->players[pm->count];

//     *p = initPlayer(0,0);  // or {0}

//     p->id = pm->count;
//     pm->count++;

//     return p->id;
// }





