#include "client_session.h"





struct Client_type
{
    IPaddress address;
    Player player;
    int id;
    int active;
};


Client Client_net_init(IPaddress ip, int id,int active){
Client client = malloc(sizeof(struct Client_type));
    client->address = ip;
    client->id = id;
    client->active= active;
    client->player=NULL;  ///  first client will joined after in game before start it will init the player of client maybe I will change it in future
    return client;
}

IPaddress Client_GetAddress(Client client)
{
    return client->address;
}

int getClientId(Client client){
    return client->id;
}