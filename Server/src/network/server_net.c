#include "Server_net.h"

struct Server_type
{
  int running; 
  int gameStarted; 
  UDPsocket socket;
  UDPpacket *sendPacket;
  UDPpacket *recvPacket;

  Client clients[MAX_CLIENTS]; 
  InputPacket inputs[MAX_PLAYERS];
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
    // server->recvPacket = SDLNet_AllocPacket(512);
    // server->sendPacket = SDLNet_AllocPacket(512);
    server->recvPacket = SDLNet_AllocPacket(sizeof(GameStatePacket) + 64);
    server->sendPacket = SDLNet_AllocPacket(sizeof(GameStatePacket) + 64);
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
Client getClient(Server server,int index){
    return server->clients[index];
};

InputPacket getInputPlayer(Server server,int id){
    return server->inputs[id];
}




void setInputPlayer(Server server,InputPacket *input,int id){
    server->inputs[id] = *input;
}

void setGameStart(Server server,int gameStart){
    server->gameStarted = gameStart;
}
void setServerRunning(Server server, int value)
{
    if (server == NULL) return;
    server->running = value;
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
void setClientCount(Server server){
    server->clientCount--;
}





int ServerNet_Receive(Server server,void** packet,IPaddress *Ip){

   if (SDLNet_UDP_Recv(server->socket, server->recvPacket)) {
        if (server->recvPacket->len < sizeof(HeaderPacket)) return 0;
        int type = ((HeaderPacket *)server->recvPacket->data)->type;
        size_t expectedSize;
        switch (type) {
            case PACKET_JOIN_REQUEST: expectedSize = sizeof(JoinRequestPacket); break;
            case PACKET_INPUT:        expectedSize = sizeof(InputPacket);        break;
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








