#include "Server_net.h"

#define MAX_CLIENTS 4
struct Client_type
{
    IPaddress address;
    int id;
    int active;
    Player player;
} ;






struct Server_type
{
  int active;  
  UDPsocket socket;
  //IPaddress serverIP;

  UDPpacket *sendPacket;
  UDPpacket *recvPacket;

  
  Client clients[MAX_CLIENTS];
  int clientCount;
};

Server server_net_init(){
Server server = malloc(sizeof(struct Server_type));
    server->active = 1;
    server->socket = NULL;
    server->sendPacket = NULL;
    server->recvPacket = NULL;
    server->clientCount = -1;
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





// void Server_Send(Server server, void *data, int size){
//     memcpy(server->sendPacket->data, data, size);
//     server->sendPacket->len = size;
//     server->sendPacket->address = server->serverIP;
//     SDLNet_UDP_Send(server->socket, -1, server->sendPacket);
// }




void  Server_Receive(Server server){
    if(SDLNet_UDP_Recv(server->socket,server->recvPacket)){
        printf("run");

    }
}



void Destroy_Server(Server server){
    SDLNet_FreePacket(server->sendPacket);
    SDLNet_FreePacket(server->recvPacket);
    SDLNet_UDP_Close(server->socket);
    free(server);
}