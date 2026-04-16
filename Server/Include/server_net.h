#ifndef SERVER_NET
#define SERVER_NET
#include <SDL2/SDL_net.h>


#define MAX_CLIENTS 4
typedef struct
{
    IPaddress address;
    int id;
    int active;
} Client;






struct 
{
  int active;  
  UDPsocket socket;
  IPaddress serverIP;

  UDPpacket *sendPacket;
  UDPpacket *recvPacket;

  
  Client clients[MAX_CLIENTS];
  int clientCount;
  int tickRate;
  int lastTick;  
} typedef Server;


void server_net_init();


#endif 


