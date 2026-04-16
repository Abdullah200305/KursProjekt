#include "physics.h"




void damagePlayer(Player player) {
    if (getPlayerLives(player) == 0) {
        return;
    }

   setPlayerLives(player);

    if (getPlayerLives(player) <= 0) {
        setPlayerAlive(0);
        // live = 0
    }
}

void updatePlayer(Player player){
    int Newx=getPlayerX(player)+getPlayerVelocityX(player);
    int Newy=getPlayerY(player)+getPlayerVelocityY(player);
    setPlayerPosition(player,Newx,Newy);
}
int isPlayerAlive(Player player){
    return getPlayerAlive(player);
}





void stopPlayer(Player player) {
setPlayerVelocity(player,0,0);
}
// void resetPlayer(Player player, float x, float y) {
//     initPlayer(player, x, y);
// }
void killPlayer(Player player) {
   
   setPlayerAlive(player);
}