#include "Server.h"



int main(int argc, char *argv[])
{

Server server= server_net_init();

ServerConnection(server,7777);    
printf("Server Init SDL_NET\n");


printf("Startning the game loop\n");
run(server);



return 0;    
}



