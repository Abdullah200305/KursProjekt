#include "Sound.h"
#include <stdio.h>

int Sound_Init(SoundSystem *s) {
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    //MUSIK
    s->menuMusic = Mix_LoadWAV("link/Sound/Menu_Music.wav");
    s->gameMusic = Mix_LoadWAV("link/Sound/Game_Music.wav");

    //SFX
    s->explosion = Mix_LoadWAV("link/Sound/Explosion.wav");
    s->scream = Mix_LoadWAV("link/Sound/Scream.wav");
    s->laugh1 = Mix_LoadWAV("link/Sound/Laugh1.wav");
    s->laugh2 = Mix_LoadWAV("link/Sound/Laugh2.wav");
    s->ice = Mix_LoadWAV("link/Sound/Ice.wav");
    s->iceBreak = Mix_LoadWAV("link/Sound/IceBreak.wav");


    return 0;
}

void Sound_Destroy(SoundSystem *s) {
    Mix_FreeChunk(s->menuMusic);
    Mix_FreeChunk(s->gameMusic);
    Mix_FreeChunk(s->explosion);
    Mix_FreeChunk(s->scream);
    Mix_FreeChunk(s->laugh1);
    Mix_FreeChunk(s->laugh2);
    Mix_FreeChunk(s->ice);
    Mix_FreeChunk(s->iceBreak);
    Mix_CloseAudio();
}
//MUSIK
void Sound_PlayMenuMusic(SoundSystem *s) {
    Mix_HaltChannel(0);                   
    Mix_PlayChannel(0, s->menuMusic, -1);  
}

void Sound_PlayGameMusic(SoundSystem *s) {
    Mix_HaltChannel(0);                   
    Mix_PlayChannel(0, s->gameMusic, -1); 
}

void Sound_StopMusic() {
    Mix_HaltChannel(0);
}

//SFX
void Sound_PlayExplosion(SoundSystem *s) {
    Mix_PlayChannel(-1, s->explosion, 0); 
}

void Sound_PlayScream(SoundSystem *s) {
    Mix_PlayChannel(-1, s->scream, 0);     
}

void Sound_PlayLaugh1(SoundSystem *s) {
    Mix_PlayChannel(-1, s->laugh1, 0);     
}

void Sound_PlayLaugh2(SoundSystem *s) {
    Mix_PlayChannel(-1, s->laugh2, 0);
}

void Sound_PlayIce(SoundSystem *s) {
    Mix_PlayChannel(-1, s->ice, 0);
}

void Sound_PlayIceBreak(SoundSystem *s) {
    Mix_PlayChannel(-1, s->iceBreak, 0);
}