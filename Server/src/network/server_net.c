#include "Server_net.h"

struct Server_type
{
  int running; 
  int gameStarted; 
  UDPsocket socket;
  UDPpacket *sendPacket;
  UDPpacket *recvPacket;

  Client clients[MAX_CLIENTS]; 
 // will see if I will change it 
 // PlayerManager playerManager;
  int clientCount;
};

Server server_net_init(){
Server server = malloc(sizeof(struct Server_type));
    server->running = 1;
    server->socket = NULL;
    server->sendPacket = NULL;
    server->recvPacket = NULL;
    server->clientCount = 0;
    server->gameStarted = 0;

    // will see if I will change it 
  //  server->playerManager=PlayerManager_init();
    return server;
}


int ServerConnection(Server server, int port) {
    if (SDLNet_Init() != 0) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        return -1;
    }
    server->socket = SDLNet_UDP_Open(port); 
    if (!server->socket) {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return -1;
    }
    server->recvPacket = SDLNet_AllocPacket(512);
    server->sendPacket = SDLNet_AllocPacket(512);
    if (!server->recvPacket || !server->sendPacket) {
       printf("SDLNet_AllocPacket failed: %s\n", SDLNet_GetError());
       return -1;
    }
    return 0;
}

///// getter 
int getServerRunning(Server server){
    return server->running;
}



// size_t getPacket(Server server){
//  return server->packet;
// }


int getGameStart(Server server){
    return server->gameStarted;
}
int getClientCount(Server server){
    return server->clientCount;
}
void setGameStart(Server server,int gameStart){
    server->gameStarted = gameStart;
}



Client Server_GetClient(Server server, int index)
{
    return server->clients[index];
}




void setNewClient(Server server,int index,Client client){
    server->clients[index]=client;
    printf("clinet joinded %d\n",server->clientCount);
    server->clientCount++;
}


Client getClient(Server server,int index){
    return server->clients[index];
};




int ServerNet_Receive(Server server,void** packet,IPaddress *Ip){

   if (SDLNet_UDP_Recv(server->socket, server->recvPacket)) {
        if (server->recvPacket->len < sizeof(HeaderPacket)) return 0;
        int type = ((HeaderPacket *)server->recvPacket->data)->type;
        size_t expectedSize;
        switch (type) {
            case PACKET_JOIN_REQUEST: expectedSize = sizeof(JoinRequestPacket); break;
            //case PACKET_INPUT:        expectedSize = sizeof(InputPacket);        break;
            case PACKET_DISCONNECT:   expectedSize = sizeof(DisconnectPacket);   break;
            default: return 0;  
        }

        if (server->recvPacket->len < expectedSize) return 0; 
        
         
        *packet = server->recvPacket->data;
        *Ip = server->recvPacket->address;
        return 1;
    }


    // uppdate after for securty 
    // if (SDLNet_UDP_Recv(server->socket, server->recvPacket)) {
    //     if (server->recvPacket->len >= sizeof(PackHe)) {
    //         memcpy(packet, server->recvPacket->data, sizeof(Packet));
    //         *Ip = server->recvPacket->address;
    //         return 1;
    //     }
    // }
    return 0;
}


void Server_Send(Server server,IPaddress clientIp, void *data, int size){
    memcpy(server->sendPacket->data, data, size);
    server->sendPacket->len = size;
    server->sendPacket->address = clientIp;
    SDLNet_UDP_Send(server->socket, -1, server->sendPacket);
}




void Destroy_Server(Server server){
    for (int i = 0; i < server->clientCount; i++) {
        if (server->clients[i]) {
            free(server->clients[i]);
        }
    }
    SDLNet_FreePacket(server->sendPacket);
    SDLNet_FreePacket(server->recvPacket);
    SDLNet_UDP_Close(server->socket);
    free(server);
}










////// changde to client_session

// struct Client_type
// {
//     IPaddress address;
//     Player player;
//     int id;
//     int active;
// };


////// changde to client_session
// Client Client_net_init(IPaddress ip, int id,int active){
// Client client = malloc(sizeof(struct Client_type));
//     client->address = ip;
//     client->id = id;
//     client->active= active;
//     return client;
// }



// old version 
// int Server_Receive(Server server) {
//     if (SDLNet_UDP_Recv(server->socket, server->recvPacket)) {
//         if (server->recvPacket->len >= sizeof(Packet)) {
//             memcpy(&server->packet, server->recvPacket->data, sizeof(Packet));
//             return 1;
//         }
//     }
//     return 0;
// }

// server will get only join request and input from player and also PACKET_DISCONNECT
// void Server_handlePackets(Server server){
//      switch (getPacket(server).type)
//     {
//         // case PACKET_INPUT:
//         //     handle_input(server);
//         //     break;

//         case PACKET_JOIN_REQUEST:  
//             handle_Connect(server);
//             break;

//         // case PACKET_DISCONNECT:
//         //     handle_Disconnect(server, getPacket(server));
//         //     break;
//     }
// }


// //👍
// // init id and ip and send back as ack
// void handle_Connect(Server server)
// {
//     if (server->clientCount >= MAX_CLIENTS)return;
//     IPaddress addr = server->recvPacket->address;
//     for (int i = 0; i < server->clientCount; i++)
//     {
//         if (server->clients[i]->address.host == addr.host &&
//             server->clients[i]->address.port == addr.port)
//         {
//             return;
//         }
//     }
//     int id = server->clientCount;
//     server->clients[id] = Client_net_init(addr, id, 1);
//     server->clientCount++;
//     printf("clinet joinded %d\n",id);
    
  

//     // this will send ack back to client accept his joining in the server
//     Packet response = {
//             .type = PACKET_JOIN_ACCEPT
//             ,.playerId=id
//             ,.data.joinAccept.assignedId= id
//         };

//     printf("Join accept send to you\n");
//     Server_Send(server, addr,(void*) &response, sizeof(Packet));
// }



/// willl be change after 
// void Server_sendInitState(Server server)
// {
//     Packet packet = {
//             .type = PACKET_MAP_INIT
//             ,.playerId=-1
//             ,.data.map={.width=1280,.height=720}
//         };
//     for (int i = 0; i < server->clientCount; i++)
//     {
//         Server_Send(server,
//                     server->clients[i]->address,
//                     &packet,
//                     sizeof(Packet));
//     }
//     memset(&packet, 0, sizeof(Packet));
//     printf("Init map sent!\n");


//     for (int i = 0; i < server->clientCount; i++)
//     {
//         packet.type = PACKET_PLAYER_INIT;
//         server->clients[i]->player = initPlayer(300+i*50,200,i);   
//         packet.playerId = server->clients[i]->id;
//         Player_ToData(server->clients[i]->player,&packet.data.player[i]);
//         printf("Init player %d sent!  x: %f  y: %f \n",i,packet.data.player[i].x,packet.data.player[i].y);                           
//     }


//     for (int i = 0; i < server->clientCount; i++)
//     {
//          Server_Send(server,
//                     server->clients[i]->address,
//                     &packet,
//                     sizeof(Packet));
//     }
//      memset(&packet, 0, sizeof(Packet));
    
  
   


// }






























// void Server_sendInitState(Server server)
// {
//     Packet packet;
//     memset(&packet, 0, sizeof(Packet));

//     packet.type = PACKET_MAP_INIT;
//     packet.playerId = -1;

//     Map map = Map_create(WIDTH, HEIGHT);

//     packet.data.mapNet = ;

//     printf("Server MAP WIDTH = %d\n", getWidth(map));
//     printf("PACKET SIZE = %zu\n", sizeof(Packet));

//     for (int i = 0; i < server->clientCount; i++)
//     {
//         Server_Send(server,
//                     server->clients[i]->address,
//                     &packet,
//                     sizeof(Packet));
//     }

//    // player 
//     // for (int p = 0; p < server->clientCount; p++)
//     // {
//     //     memset(&packet, 0, sizeof(Packet));

//     //     packet.type = PACKET_PLAYER_INIT; // or PACKET_PLAYER_INIT
//     //     packet.playerId = p;

//     //     packet.data.player = initPlayer(230, 300, p);

//     //     for (int i = 0; i < server->clientCount; i++) {
//     //         Server_Send(server,server->clients[i]->address,(void*) &packet,sizeof(Packet));
//     //     }
//     // }
//     // memset(&packet, 0, sizeof(Packet));


// // bomb   some issue in bomb
// //    memset(&packet, 0, sizeof(Packet));

// //    packet.playerId = -1;
// //    packet.type = PACKET_BOMB_INIT;
   
// //    packet.data.bomb = createBomb();
   
// //    for (int i = 0; i < server->clientCount; i++) {
// //         Server_Send(server,server->clients[i]->address,(void*) &packet,sizeof(Packet));
// //    }
//     printf("Init map and players sent!\n");
// }






