#include "Server_net.h"

#define MAX_CLIENTS 4
struct Client_type
{
    IPaddress address;
    int id;
    int active;
};




struct Server_type
{
  int running; 
  int gameStarted; 
  UDPsocket socket;
  

  UDPpacket *sendPacket;
  UDPpacket *recvPacket;
  
  Packet packet; // will tell use which kinde of packet
  
  Client clients[MAX_CLIENTS]; // 

  PlayerManager playerManager;
  
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
    PlayerManager_init(&server->playerManager);
    return server;
}


Client Client_net_init(IPaddress ip, int id,int active){
Client client = malloc(sizeof(struct Client_type));
    client->address = ip;
    client->id = id;
    client->active= active;
    return client;
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
Packet getPacket(Server server){
 return server->packet;
}
int getGameStart(Server server){
    return server->gameStarted;
}
int getClientCount(Server server){
    return server->clientCount;
}



void setGameStart(Server server,int gameStart){
    server->gameStarted = gameStart;
}

// void Server_Send(Server server, void *data, int size){
//     memcpy(server->sendPacket->data, data, size);
//     server->sendPacket->len = size;
//     server->sendPacket->address = server->serverIP;
//     SDLNet_UDP_Send(server->socket, -1, server->sendPacket);
// }




int  Server_Receive(Server server){
    if(SDLNet_UDP_Recv(server->socket,server->recvPacket)){
        Packet packet;
        memcpy(&packet,server->recvPacket->data,sizeof(Packet));
        server->packet = packet;
        return 1; 
    }
    return 0; 
}


void Server_handlePackets(Server server){
     switch (getPacket(server).type)
    {
        case PACKET_INPUT:
            handle_input(server);
            break;

        case PACKET_CONNECT:
            handle_Connect(server);
            break;

        // case PACKET_DISCONNECT:
        //     handle_Disconnect(server, getPacket(server));
        //     break;
    }
}




void handle_Connect(Server server)
{
    if (server->clientCount >= MAX_CLIENTS)return;
    IPaddress addr = server->recvPacket->address;
    // check duplicates
    for (int i = 0; i < server->clientCount; i++)
    {
        if (server->clients[i]->address.host == addr.host &&
            server->clients[i]->address.port == addr.port)
        {
            return;
        }
    }
    int id = server->clientCount;
    server->clients[id] = Client_net_init(addr, id, 1);
    server->clientCount++;
     printf("clinet joinded %d\n",id);
}





void  handle_input(Server server){

}



















void Destroy_Server(Server server){
    SDLNet_FreePacket(server->sendPacket);
    SDLNet_FreePacket(server->recvPacket);
    SDLNet_UDP_Close(server->socket);
    free(server);
}