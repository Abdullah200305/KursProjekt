#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Chunk *menuMusic;   // musik
    Mix_Chunk *gameMusic;

    Mix_Chunk *explosion;   // sfx
    Mix_Chunk *scream;
    Mix_Chunk *laugh1;
    Mix_Chunk *laugh2;
    Mix_Chunk *ice;
    Mix_Chunk *iceBreak;
} SoundSystem;

int Sound_Init(SoundSystem *s);
void Sound_Destroy(SoundSystem *s);

//MUSIK
void Sound_PlayMenuMusic(SoundSystem *s);
void Sound_PlayGameMusic(SoundSystem *s);
void Sound_StopMusic();

//SFX
void Sound_PlayExplosion(SoundSystem *s);
void Sound_PlayScream(SoundSystem *s);
void Sound_PlayLaugh1(SoundSystem *s);
void Sound_PlayLaugh2(SoundSystem *s);
void Sound_PlayIce(SoundSystem *s);
void Sound_PlayIceBreak(SoundSystem *s);
#endif