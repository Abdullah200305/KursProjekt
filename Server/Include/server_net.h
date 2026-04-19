#ifndef SERVER_NET
#define SERVER_NET
#include <SDL2/SDL_net.h>
#include "Game_state.h"
#include "protocol.h"
#include "Player_manager.h"


typedef struct Server_type *Server;
typedef struct Client_type *Client;
Server server_net_init();
int ServerConnection(Server server, int port);

int getServerRunning(Server server);
Packet getPacket(Server server);
int getGameStart(Server server);
int getClientCount(Server server);


void Server_sendInitState(Server server);

void setGameStart(Server server,int gameStart);


int  Server_Receive(Server server);
void Server_Send(Server server,IPaddress clientIp, void *data, int size);

 
void Server_handlePackets(Server server);
Client Client_net_init(IPaddress ip, int id,int active);
void handle_Connect(Server server);
void handle_input(Server server);


void Destroy_Server(Server server);
#endif //SERVER_NET


