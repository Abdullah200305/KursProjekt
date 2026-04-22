#include "packet_builder.h"


void Packet_BuildGameStart(Game *game,int id,GameInitPacket *packet){


//     typedef struct {
//     int type;
//     GameInitData data;
// } GameInitPacket;


GameInitPacket Newpacket ={
    .type = PACKET_GAME_INIT,
    .data.numPlayers = game->numPlayers,
    .data.yourClientId = id,
    .data.map ={.height = getHeight(game->map),
        .width = getWidth(game->map),
        .mapId = 1},
    .data.bomb ={.active = getBombActive(game->bomb),
        .bombCarrier = getBombCarrier(game->bomb),
        .exploding = getBombExploding(game->bomb),
        .timer = getBombTimer(game->bomb),
        .x= getBombX(game->bomb),
        .y= getBombY(game->bomb)}      
};



// old version 
// {
//     .type = PACKET_GAME_INIT 
//     ,.data.Game.dataMap={
//             .width=getWidth(game->map),
//             .height=getHeight(game->map),
//             .MapID=1,
//         }    
//     };
    // printf("Map packet ready to sent!\n"); 
    // for (int i = 0; i <game->numPlayers; i++)
    // {
    //     Newpacket.data.Game.dataPlayer[i].x = getPlayerX(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].y = getPlayerY(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].vx = getPlayerVelocityX(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].vy = getPlayerVelocityY(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].width = getPlayerWidth(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].height = getPlayerHeight(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].lives = getPlayerLives(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].alive = isPlayerAlive(game->players[i]);
    //     Newpacket.data.Game.dataPlayer[i].id = getPlayerId(game->players[i]);
    //     printf("player %d packet done ready sent! x: %f  y: %f \n",i,Newpacket.data.Game.dataPlayer[i].x,Newpacket.data.Game.dataPlayer[i].y);                        
    // }


    printf("Map packet ready to sent!\n"); 
    for (int i = 0; i <game->numPlayers; i++)
    {
       
        Newpacket.data.players[i].x = getPlayerX(game->players[i]);
        Newpacket.data.players[i].y = getPlayerY(game->players[i]);
        Newpacket.data.players[i].vx = getPlayerVelocityX(game->players[i]);
        Newpacket.data.players[i].vy = getPlayerVelocityY(game->players[i]);
        Newpacket.data.players[i].width = getPlayerWidth(game->players[i]);
        Newpacket.data.players[i].height = getPlayerHeight(game->players[i]);
        Newpacket.data.players[i].lives = getPlayerLives(game->players[i]);
        Newpacket.data.players[i].alive = isPlayerAlive(game->players[i]);
        Newpacket.data.players[i].id = getPlayerId(game->players[i]);
        printf("player %d packet done ready sent! x: %f  y: %f \n",i,Newpacket.data.players[i].x,Newpacket.data.players[i].y);                        
    }



    memcpy(packet,&Newpacket,sizeof(GameInitPacket));
    printf("Ready to send");
}



// old version 
// void Packet_BuildGameAccept(int id,Packet *packet){   
//  // this will send ack back to client accept his joining in the server
//  Packet response = {
//             .type = PACKET_JOIN_ACCEPT
//             ,.playerId=id
//             ,.data.joinAccept.assignedId= id
//     };
//     memcpy(packet,&response,sizeof(Packet));
//     printf("Join accept send to you %d\n",id);
// }



void Packet_BuildGameAccept(int id,JoinAcceptPacket *packet){   
 // this will send ack back to client accept his joining in the server
 
 JoinAcceptPacket response = {
        .type = PACKET_JOIN_ACCEPT
        ,.clientId=id
    };
    memcpy(packet,&response,sizeof(JoinAcceptPacket));
    printf("Join accept send to you %d\n",id);
}



 