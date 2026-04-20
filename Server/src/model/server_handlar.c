#include "server_handlar.h"




 

void Server_handlePackets(Server server){
    Packet packet;
    IPaddress ip; 
    while (ServerNet_Receive(server, &packet, &ip))
    {
        switch (packet.type)
        {
            case PACKET_JOIN_REQUEST:
                Handle_join(server, packet, ip);
                break;

            // case PACKET_INPUT:
            //     HandleInput(server, &packet);
            //     break;

            // case PACKET_DISCONNECT:
            //     HandleDisconnect(server, ip);
            //     break;
        }
    }

   
}




//👍
// init id and ip and send back as ack
void Handle_join(Server server,Packet packet,IPaddress ip)
{
    for (int  i = 0; i < getClientCount(server); i++)
    {
        Client c = Server_GetClient(server, i);
         if (Client_GetAddress(c).host == ip.host &&
             Client_GetAddress(c).port == ip.port)
        {
            return;
        }
    }

    int id = getClientCount(server);
    Client newClient = Client_net_init(ip,id,1);
    setNewClient(server,id,newClient);

    
    
    Packet newPacket;
    Packet_BuildGameAccept(id,&newPacket);
    Server_Send(server, ip,(void*) &newPacket, sizeof(Packet));
    memset(&newPacket,0,sizeof(Packet));
}