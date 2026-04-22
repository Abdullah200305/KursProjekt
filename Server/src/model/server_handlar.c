#include "server_handlar.h"




 

void Server_handlePackets(Server server){
    void* packet;
    IPaddress ip; 
    while (ServerNet_Receive(server, &packet, &ip))
    {
        int type = ((HeaderPacket *)packet)->type;  // packet will have this to know which typ is it 
        switch (type)
        {
            case PACKET_JOIN_REQUEST:
                Handle_join(server,(JoinRequestPacket *) packet, ip);
                break;

            // case PACKET_INPUT:
            //     HandleInput(server, &packet);
            //     break;

            // case PACKET_DISCONNECT:
            //     Handle_disconnect(server,(DisconnectPacket *) packet, ip);
            //     break;
            default:
                printf("Unknown packet type: %d\n", type);
                break;
        }
    }

   
}




//👍
// init id and ip and send back as ack
void Handle_join(Server server,JoinRequestPacket *packet,IPaddress ip)
{
    int id=-1;
    if(MAX_CLIENTS==getClientCount(server)){
        printf("The Team is full right now!!\n");
        return;
    }
    for (int  i = 0; i < MAX_CLIENTS; i++)
    {
        Client c = Server_GetClient(server, i);
         if (Client_GetAddress(c).host == ip.host &&
             Client_GetAddress(c).port == ip.port)
        {
            return;
        }
        int Assign = getClientId(getClient(server,i));
        if(Assign==-1){
            id = Assign;
        }
    }
    Client newClient = Client_net_init(ip,id,1);
    setNewClient(server,id,newClient);

    JoinAcceptPacket newPacket;
    Packet_BuildGameAccept(id,&newPacket);
    Server_Send(server, ip,(void*) &newPacket, sizeof(JoinAcceptPacket));
    memset(&newPacket,0,sizeof(JoinAcceptPacket));
}



void Handle_disconnect(Server server,DisconnectPacket * packet,IPaddress ip){
    ClientDestroy(getClient(server,packet->clientId));
}