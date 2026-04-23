#ifndef SERVER_HANDLAR_H
#define SERVER_HANDLAR_H
#include "server_net.h"
#include "network_protocol.h"
#include "packet_builder.h"
// will handlar which type of packet is this
void Handle_join(Server server,JoinRequestPacket *packet,IPaddress ip);
void Server_handlePackets(Server server);


void Handle_disconnect(Server server,DisconnectPacket * packet,IPaddress ip);
void Handle_Input(Server server,InputPacket * packet);
#endif //SERVER_HANDLAR_H