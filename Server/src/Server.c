#include "Server.h"

#include <signal.h>


static Server g_server;

void handle_sigint(int sig)
{
    printf("\nStopping server...\n");
    setServerRunning(g_server, 0);
}

int main(int argc, char *argv[])
{
Server server= server_net_init();
Game game;
printf("server init");
ServerConnection(server,8080);    
printf("Server Init SDL_NET\n");
printf("Startning the Server\n");






signal(SIGINT, handle_sigint); 

g_server = server;
signal(SIGINT, handle_sigint);  
run(server, game);
return 0;    
}



