#ifndef SERVER_NET
#define SERVER_NET
#include <SDL2/SDL_net.h>
#include "Game_state.h"


typedef struct Server_type *Server;
typedef struct Client_type *Client;
Server server_net_init();
int ServerConnection(Server server, int port);
void Destroy_Server(Server server);
#endif //SERVER_NET


