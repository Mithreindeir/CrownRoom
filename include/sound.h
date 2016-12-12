#ifndef HEADER_SOUND
#define HEADER_SOUND

#include <SDL.h>
#include <SDL_mixer.h>

typedef struct sound
{
	Mix_Chunk* sound;
} sound;

typedef struct music
{
	Mix_Music* music;
} music;

sound* sound_alloc();
sound* sound_init(sound* sound, const char* file);
void sound_destroy(sound* sound);
void sound_play(sound* sound);
void music_play(music* music);
void music_pause(music* music);
music* music_alloc();
music* music_init(music* music, const char* file);
void music_destroy(music* music);

#endif