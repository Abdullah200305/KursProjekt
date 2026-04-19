#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"
#include "Ability.h"
#include "client_net.h"

int main(int argc, char *argv[])
{
    Game game;
    Renderer renderer;
    InputState input = Input_Init();

    ClientNet clientNet= ClientNet_Init("127.0.0.1", 2000);
    int clientNetReady = 0;

    printf("Starting game...\n");
    game_init(&game, &renderer);

    if (clientNet == 0) {
        printf("[CLIENT] UDP init OK\n");
        clientNetReady = 1;
    } else {
        printf("[CLIENT] UDP init FAILED\n");
    }

    if (clientNetReady) {
        if (ClientNet_SendJoinRequest(clientNet) == 0) {
            printf("[CLIENT] JOIN_REQUEST sent\n");
        } else {
            printf("[CLIENT] JOIN_REQUEST failed\n");
        }
    }

    if (clientNetReady) {
        int receiveResult = ClientNet_TryReceive(clientNet);

        if (receiveResult == 0) {
            printf("[CLIENT] No server response yet\n");
        } else if (receiveResult < 0) {
            printf("[CLIENT] Receive check failed\n");
        }
    }

    AbilitySystem *system = AbilitySystem_create();
    AbilitySystem_init(system);
    AbilitySystem_spawn(system, 800, 600);

    printf("Entering game loop...\n");
    game_loop(&game, &renderer);

    AbilitySystem_destroy(system);

    if (clientNetReady) {
        ClientNet_Destroy(clientNet);
    }

    game_cleanup(&game, &renderer);
    return 0;
}