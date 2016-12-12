#include "../include/sound.h"

sound * sound_alloc()
{
	return malloc(sizeof(sound));
}

sound * sound_init(sound * sound, const char * file)
{
	sound->sound = NULL;

	sound->sound = Mix_LoadWAV(file);
	if (sound->sound == NULL)
	{
		printf("MUSIC ERROR: %s\n", Mix_GetError());

	}
	return sound;
}

void sound_destroy(sound * sound)
{
	Mix_FreeChunk(sound->sound);
	free(sound);
}

void sound_play(sound * sound)
{
	Mix_PlayChannel(-1, sound->sound, 0);

}

void music_play(music * music)
{
	if (Mix_PausedMusic() == 1)
	{
		Mix_ResumeMusic();
	}
	else
	{
		Mix_PlayMusic(music->music, -1);
	}

}

void music_pause(music * music)
{
	if (Mix_PausedMusic() != 1) Mix_PauseMusic();
}

music * music_alloc()
{
	return malloc(sizeof(music));
}

music * music_init(music * music, const char * file)
{
	music->music = NULL;
	music->music = Mix_LoadMUS(file);
	if (music->music == NULL)
	{
		printf("MUSIC ERROR: %s\n", Mix_GetError());
	}
	return music;
}

void music_destroy(music * music)
{
	Mix_FreeMusic(music->music);
	free(music);
}
