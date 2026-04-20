#ifndef SERVER_HANDLAR_H
#define SERVER_HANDLAR_H
#include "server_net.h"
#include "protocol.h"
// will handlar which type of packet is this
void Handle_join(Server server,Packet packet,IPaddress ip);
void Server_handlePackets(Server server);
#endif //SERVER_HANDLAR_H