#include "packet_builder.h"


void Packet_BuildGameStart(Game *game,int id,Packet *packet){
Packet Newpacket = {
    .type = PACKET_GAME_INIT 
    ,.playerId= id // server id
    ,.data.Game.dataMap={
            .width=getWidth(game->map),
            .height=getHeight(game->map),
            .MapID=1
        }    
    };
    printf("Map packet ready to sent!\n"); 
    for (int i = 0; i <game->numPlayers; i++)
    {
        Newpacket.data.Game.dataPlayer[i].x = getPlayerX(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].y = getPlayerY(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].vx = getPlayerVelocityX(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].vy = getPlayerVelocityY(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].width = getPlayerWidth(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].height = getPlayerHeight(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].lives = getPlayerLives(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].alive = isPlayerAlive(game->players[i]);
        Newpacket.data.Game.dataPlayer[i].id = getPlayerId(game->players[i]);
        printf("player %d packet done ready sent! x: %f  y: %f \n",i,Newpacket.data.Game.dataPlayer[i].x,Newpacket.data.Game.dataPlayer[i].y);                        
    }
    memcpy(packet,&Newpacket,sizeof(Packet));
    printf("Ready to send");
}




void Packet_BuildGameAccept(int id,Packet *packet){   
 // this will send ack back to client accept his joining in the server
 Packet response = {
            .type = PACKET_JOIN_ACCEPT
            ,.playerId=id
            ,.data.joinAccept.assignedId= id
    };
    memcpy(packet,&response,sizeof(Packet));
    printf("Join accept send to you %d\n",id);
}
 