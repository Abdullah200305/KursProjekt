#include "Server.h"



int main(int argc, char *argv[])
{

Server server= server_net_init();
printf("Server Init");
ServerConnection(server,7777);    
printf("Server Init SDL_NET");

return 0;    
}



