#ifndef SERVER_NET
#define SERVER_NET
#include <SDL2/SDL_net.h>
#include "Game_state.h"
#include "protocol.h"
#include "Player_manager.h"



// invalied in clienat info such as ipaddress and id and active or not
#include "client_session.h"

typedef struct Server_type *Server;


/// old version
//typedef struct Client_type *Client;

Server server_net_init();
int ServerConnection(Server server, int port);

int getServerRunning(Server server);
Packet getPacket(Server server);
int getGameStart(Server server);
int getClientCount(Server server);

Client Server_GetClient(Server server, int index);



void setNewClient(Server server,int index,Client client);
void setGameStart(Server server,int gameStart);

// old version
//int  Server_Receive(Server server);

int ServerNet_Receive(Server server,Packet *packet,IPaddress *Ip);


void Server_Send(Server server,IPaddress clientIp, void *data, int size);

 
//void Server_handlePackets(Server server);

//Client Client_net_init(IPaddress ip, int id,int active);




// this will go to server handlar after 
//void handle_Connect(Server server);


// will go to packet buidler 
void Server_sendInitState(Server server);




//void handle_input(Server server);


void Destroy_Server(Server server);
#endif //SERVER_NET


