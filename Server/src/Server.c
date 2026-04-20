#include "Server.h"



int main(int argc, char *argv[])
{

Server server= server_net_init();
Game game;
printf("server init");
ServerConnection(server,2000);    
printf("Server Init SDL_NET\n");
printf("Startning the Server\n");
run(server,game);


return 0;    
}



