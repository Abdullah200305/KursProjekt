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

            case PACKET_INPUT:
                Handle_Input(server,(InputPacket *) packet);
                break;

            case PACKET_DISCONNECT:
                Handle_disconnect(server,(DisconnectPacket *) packet, ip);
                break;
            default:

                printf("Unknown packet type: %d\n", type);
                break;
        }
    }

   
}

void Handle_Input(Server server,InputPacket * packet){
    int id = packet->clientId;
    if (id < 0 || id >= MAX_PLAYERS)
    return;
    setInputPlayer(server,packet,id);
}


void Handle_join(Server server,JoinRequestPacket *packet,IPaddress ip)
{
    int id=-1;
    if(MAX_CLIENTS==getClientCount(server)){
        printf("The Team is full right now!!\n");
        return;
    }
    if(getClientCount(server)>0){
   
    for (int  i = 0; i < getClientCount(server); i++)
    {
        Client c = Server_GetClient(server, i);
         if (Client_GetAddress(c).host == ip.host &&
             Client_GetAddress(c).port == ip.port)
        {
            return;
        }
        int Assign = getClientId(getClient(server,i));
        if(Assign==-1){
            id = i;
        }
        else{
            id = getClientCount(server);
        }
    }
    }
    else{
        printf("first player\n");
        id=0;
    }
    Client newClient = Client_net_init(ip,id,1);
    setNewClient(server,id,newClient);

    JoinAcceptPacket newPacket;
    Packet_BuildGameAccept(id,&newPacket);
    Server_Send(server, ip,(void*) &newPacket, sizeof(JoinAcceptPacket));
    memset(&newPacket,0,sizeof(JoinAcceptPacket));
}


// this will remove from server
void Handle_disconnect(Server server,DisconnectPacket * packet,IPaddress ip){
   for (int i = 0; i < getClientCount(server); i++) {
        Client c = getClient(server, i);
        if (getActive(c) && getClientId(c) != packet->clientId) {
            printf("Sending disconnect packet to client %d\n", getClientId(c));
            Server_Send(server,
                Client_GetAddress(c),
                packet,
                sizeof(DisconnectPacket));
        }
    }

    ClientClean(getClient(server, packet->clientId)); 
    setClientCount(server);
    memset(packet, 0, sizeof(DisconnectPacket));
}