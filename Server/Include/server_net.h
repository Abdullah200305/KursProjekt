#ifndef SERVER_NET
#define SERVER_NET
#include <SDL2/SDL_net.h>
struct 
{
  int active;  
  UDPsocket socket;
  IPaddress serverIP;

  UDPpacket *sendPacket;
  UDPpacket *recvPacket;
  
} typedef Server;


void server_net_init();


#endif 