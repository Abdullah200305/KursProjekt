#ifndef SERVER_NET
#define SERVER_NET
#include <SDL2/SDL_net.h>
#include "Game_state.h"
#include "network_protocol.h"

#include "Player_manager.h"

typedef enum {
    SERVER_LOBBY,
    SERVER_GAME_INIT,
    SERVER_COUNTDOWN,
    SERVER_RUNNING
} ServerState;


// invalied in clienat info such as ipaddress and id and active or not
#include "client_session.h"

typedef struct Server_type *Server;


/// old version
//typedef struct Client_type *Client;

Server server_net_init();
int ServerConnection(Server server, int port);

int getServerRunning(Server server);



//Packet getPacket(Server server);


Uint32 getlastTick(Server server);
int getCountdown(Server server);

int getGameStart(Server server);
int getClientCount(Server server);
ServerState getServerState(Server server);
Client Server_GetClient(Server server, int index);

void setCountdown(Server server,int value);
void setlastTick(Server server,Uint32 tick);
void setServerState(Server server, ServerState state);
void setNewClient(Server server,int index,Client client);
void setGameStart(Server server,int gameStart);
void setClientCount(Server server);
void setServerRunning(Server server, int value);
Client getClient(Server server,int index);
InputPacket getInputPlayer(Server server,int id);



void setInputPlayer(Server server,InputPacket *input,int id);

int ServerNet_Receive(Server server,void** packet,IPaddress *Ip);
void Server_Send(Server server,IPaddress clientIp, void *data, int size);

// old version
//int  Server_Receive(Server server); 
//void Server_handlePackets(Server server);
//Client Client_net_init(IPaddress ip, int id,int active);
// this will go to server handlar after 
//void handle_Connect(Server server);
//void handle_input(Server server);






// will go to packet buidler fix later 
void Server_sendInitState(Server server);





void Destroy_Server(Server server);
#endif //SERVER_NET


