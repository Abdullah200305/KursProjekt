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

    //if (server->clientCount >= MAX_CLIENTS)return;
    //IPaddress addr = server->recvPacket->address;
    // for (int i = 0; i < server->clientCount; i++)
    // {
        
    //     if (server->clients[i]->address.host == addr.host &&
    //         server->clients[i]->address.port == addr.port)
    //     {
    //         return;
    //     }
    // }


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

    // int id = server->clientCount;
    // server->clients[id] = Client_net_init(addr, id, 1);
    // server->clientCount++;
    // printf("clinet joinded %d\n",id);
    
  

    // this will send ack back to client accept his joining in the server
    Packet response = {
            .type = PACKET_JOIN_ACCEPT
            ,.playerId=id
            ,.data.joinAccept.assignedId= id
        };

    printf("Join accept send to you\n");
    Server_Send(server, ip,(void*) &response, sizeof(Packet));
}