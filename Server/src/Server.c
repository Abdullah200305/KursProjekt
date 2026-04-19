#include "Server.h"



int main(int argc, char *argv[])
{

Server server= server_net_init();
printf("serve init");
ServerConnection(server,2000);    
printf("Server Init SDL_NET\n");
printf("Startning the Server\n");
run(server);



return 0;    
}



