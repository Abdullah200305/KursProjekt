#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H
#include <SDL2/SDL_net.h>
#include "Player.h"


#define MAX_CLIENTS 4




// varje clinet kommer innehåller state och player 
typedef struct Client_type *Client;

Client Client_net_init(IPaddress ip, int id,int active);
IPaddress Client_GetAddress(Client client);

int getClientId(Client client);
#endif //CLIENT_SESSION_H