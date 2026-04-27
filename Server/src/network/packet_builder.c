#include "packet_builder.h"

void Packet_BuildGameStart(Game *game, int id, GameInitPacket *packet)
{
    GameInitPacket Newpacket = {
        .type = PACKET_GAME_INIT,
        .data.numPlayers = game->numPlayers,
        .data.yourClientId = id,
        .data.map = {.height = getHeight(game->map),
                     .width = getWidth(game->map),
                     .mapId = 1},
        .data.bomb = {.active = getBombActive(game->bomb),
                      .bombCarrier = getBombCarrier(game->bomb),
                      .exploding = getBombExploding(game->bomb),
                      .timer = getBombTimer(game->bomb),
                      .x = getBombX(game->bomb),
                      .y = getBombY(game->bomb)}};

    printf("Map packet ready to sent!\n");
    for (int i = 0; i < game->numPlayers; i++)
    {
        Newpacket.data.players[i].x = getPlayerX(game->players[i]);
        Newpacket.data.players[i].y = getPlayerY(game->players[i]);
        Newpacket.data.players[i].vx = getPlayerVelocityX(game->players[i]);
        Newpacket.data.players[i].vy = getPlayerVelocityY(game->players[i]);
        Newpacket.data.players[i].width = getPlayerWidth(game->players[i]);
        Newpacket.data.players[i].height = getPlayerHeight(game->players[i]);
        Newpacket.data.players[i].lives = getPlayerLives(game->players[i]);
        Newpacket.data.players[i].alive = isPlayerAlive(game->players[i]);
        Newpacket.data.players[i].id = getPlayerId(game->players[i]);
        printf("player %d packet done ready sent! x: %f  y: %f \n", i, Newpacket.data.players[i].x, Newpacket.data.players[i].y);
    }

    memcpy(packet, &Newpacket, sizeof(GameInitPacket));
    printf("Ready to send");
}

// this will send ack back to client accept his joining in the server
void Packet_BuildGameAccept(int id, JoinAcceptPacket *packet)
{
    JoinAcceptPacket response = {
        .type = PACKET_JOIN_ACCEPT, .clientId = id};
    memcpy(packet, &response, sizeof(JoinAcceptPacket));
    printf("Join accept send to you %d\n", id);
}

// this will make when server went down
void Packet_BuildDisconnect(int id, DisconnectPacket *packet)
{
    DisconnectPacket response = {
        .type = PACKET_DISCONNECT, .clientId = id};
    memcpy(packet, &response, sizeof(DisconnectPacket));
    printf("DisconnectPacket send to you %d\n", id);
}

// this will send during game_loop to sned uppdate for all clients
void Packet_BuildGameState(GameStatePacket *packet, Game *game)
{
    GameStatePacket Newpacket = {0};
    Newpacket.type = PACKET_GAME_STATE;
    Newpacket.data.numPlayers = game->numPlayers;
    Newpacket.data.bomb.active = getBombActive(game->bomb);
    Newpacket.data.bomb.bombCarrier = getBombCarrier(game->bomb);
    Newpacket.data.bomb.exploding = getBombExploding(game->bomb);
    Newpacket.data.bomb.timer = getBombTimer(game->bomb);
    Newpacket.data.bomb.x = getBombX(game->bomb);
    Newpacket.data.bomb.y = getBombY(game->bomb);

   
    for (int i = 0; i < game->numPlayers; i++)
    {
        Newpacket.data.players[i].x = getPlayerX(game->players[i]);
        Newpacket.data.players[i].y = getPlayerY(game->players[i]);
        Newpacket.data.players[i].vx = getPlayerVelocityX(game->players[i]);
        Newpacket.data.players[i].vy = getPlayerVelocityY(game->players[i]);
        Newpacket.data.players[i].width = getPlayerWidth(game->players[i]);
        Newpacket.data.players[i].height = getPlayerHeight(game->players[i]);
        Newpacket.data.players[i].lives = getPlayerLives(game->players[i]);
        Newpacket.data.players[i].alive = isPlayerAlive(game->players[i]);
        Newpacket.data.players[i].id = getPlayerId(game->players[i]);
        Newpacket.data.players[i].freezeTimer = getPlayerFreezeTimer(game->players[i]);
    }

    Newpacket.data.abilities.numAbilities = MAX_ABILITIES;
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        Newpacket.data.abilities.items[i].active = AbilityItem_isActive(game->abilitySystem, i);
        Newpacket.data.abilities.items[i].type   = AbilityItem_getType(game->abilitySystem, i);
        Newpacket.data.abilities.items[i].x      = AbilityItem_getX(game->abilitySystem, i);
        Newpacket.data.abilities.items[i].y      = AbilityItem_getY(game->abilitySystem, i);
        Newpacket.data.abilities.items[i].width  = AbilityItem_getWidth(game->abilitySystem, i);
        Newpacket.data.abilities.items[i].height = AbilityItem_getHeight(game->abilitySystem, i);
    }
    

    memcpy(packet, &Newpacket, sizeof(GameStatePacket));
}


