// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	System interface for sound using SDL2 (macOS and cross-platform)
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_sound_sdl.c,v 1.0 2026/07/18 macos port $";

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "SDL2/SDL.h"

#include "z_zone.h"
#include "i_system.h"
#include "i_sound.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "doomdef.h"

// Sound device state
static SDL_AudioDeviceID audio_device = 0;
static SDL_AudioSpec obtained_spec;

// Configuration
#define SAMPLERATE		11025
#define SAMPLESIZE		2
#define NUM_CHANNELS	8
#define SAMPLECOUNT		512
#define BUFMUL			4
#define MIXBUFFERSIZE	(SAMPLECOUNT * BUFMUL)

// The sound effect lengths
int lengths[NUMSFX];

// The channel data
typedef struct
{
	unsigned char*	data;
	unsigned char*	end;
	int				length;
	int				handle;
	int				id;
	int				step;
	int				stepremainder;
	int				volume;
	int				left_volume;
	int				right_volume;
	int				start_gametic;
} channel_t;

static channel_t channels[NUM_CHANNELS];

// Volume lookup tables
static int vol_lookup[128 * 256];

// Mixing buffer
static signed short mixbuffer[MIXBUFFERSIZE];

// Master volume (0-15)
// Note: snd_SfxVolume and snd_MusicVolume are declared in doomstat.h

//
// I_InitSound
// Initialize sound system
//
void I_InitSound(void)
{
	int i;

	// Initialize SDL audio
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr, "Could not initialize SDL audio: %s\n", SDL_GetError());
		return;
	}

	// Set up audio specification
	SDL_AudioSpec desired_spec;
	SDL_zero(desired_spec);

	desired_spec.freq = SAMPLERATE;
	desired_spec.format = AUDIO_S16;
	desired_spec.channels = 2;  // Stereo
	desired_spec.samples = SAMPLECOUNT;
	desired_spec.callback = NULL;  // Use polling instead of callback

	// Open audio device
	audio_device = SDL_OpenAudioDevice(
		NULL,  // Default device
		0,     // Non-capture
		&desired_spec,
		&obtained_spec,
		0      // No flags
	);

	if (audio_device == 0)
	{
		fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());
		return;
	}

	printf("Audio device opened: %d Hz, %d channels\n",
		   obtained_spec.freq, obtained_spec.channels);

	// Clear channels
	for (i = 0; i < NUM_CHANNELS; i++)
	{
		channels[i].data = NULL;
		channels[i].id = 0;
		channels[i].handle = -1;
		channels[i].volume = 127;
		channels[i].start_gametic = 0;
	}

	// Initialize volume lookup table
	for (i = 0; i < 128; i++)
	{
		int j;
		for (j = 0; j < 256; j++)
			vol_lookup[i * 256 + j] = (i * j) >> 8;
	}

	// Start audio playback
	SDL_PauseAudioDevice(audio_device, 0);
}

//
// I_ShutdownSound
//
void I_ShutdownSound(void)
{
	if (audio_device != 0)
	{
		SDL_CloseAudioDevice(audio_device);
		audio_device = 0;
	}
}

//
// I_SetChannels
//
void I_SetChannels(void)
{
	// Channels already allocated at init
}

//
// I_SetSfxVolume
//
void I_SetSfxVolume(int volume)
{
	snd_SfxVolume = volume;
	if (snd_SfxVolume > 15)
		snd_SfxVolume = 15;
	if (snd_SfxVolume < 0)
		snd_SfxVolume = 0;
}

//
// I_SetMusicVolume
//
void I_SetMusicVolume(int volume)
{
	snd_MusicVolume = volume;
	if (snd_MusicVolume > 15)
		snd_MusicVolume = 15;
	if (snd_MusicVolume < 0)
		snd_MusicVolume = 0;
}

//
// I_LoadSfxLump
// Loads sound data from WAD lump number
//
static void I_LoadSfxLump(sfxinfo_t* sfx)
{
	unsigned char* data;
	unsigned char* padded;
	int size;
	int paddedsize;
	int i;

	if (sfx->lumpnum <= 0)
		return;

	// Get the sound data from the WAD
	size = W_LumpLength(sfx->lumpnum);
	data = (unsigned char*)W_CacheLumpNum(sfx->lumpnum, PU_STATIC);

	// Pad the sound chunk with zeroes
	paddedsize = ((size - 8 + (SAMPLECOUNT - 1)) / SAMPLECOUNT) * SAMPLECOUNT;
	padded = (unsigned char*)Z_Malloc(paddedsize * SAMPLESIZE, PU_STATIC, 0);
	memset(padded, 0, paddedsize * SAMPLESIZE);

	// Copy the padded sound
	if (size > 8)
		memcpy(padded, data + 8, size - 8);

	// Free the original lump
	Z_Free(data);

	sfx->data = padded;
}

//
// I_StartSound
//
int I_StartSound(int id, int vol, int sep, int pitch, int priority)
{
	unsigned long flags;
	int chan;
	int oldest = channels[0].start_gametic;
	int oldestnum = 0;
	int slot;
	int rightvol;
	int leftvol;
	sfxinfo_t* sfx;

	sfx = &S_sfx[id];

	// Find an empty channel
	for (chan = 0; chan < NUM_CHANNELS; chan++)
	{
		if (channels[chan].id == 0)
			break;

		// Grab the oldest sound
		if (channels[chan].start_gametic < oldest)
		{
			oldestnum = chan;
			oldest = channels[chan].start_gametic;
		}
	}

	// No free channels, take the oldest
	if (chan == NUM_CHANNELS)
		chan = oldestnum;

	// Separate left/right volume
	if (sep < 0)
	{
		sep = -sep;
		leftvol = vol - (vol * sep) / 255;
		rightvol = vol;
	}
	else if (sep > 0)
	{
		rightvol = vol - (vol * sep) / 255;
		leftvol = vol;
	}
	else
	{
		leftvol = vol;
		rightvol = vol;
	}

	// Load the sound if not cached
	if (!sfx->data)
		I_LoadSfxLump(sfx);

	if (!sfx->data)
		return -1;  // Failed to load

	// Calculate sound length from lump size
	int soundlength = W_LumpLength(sfx->lumpnum);
	if (soundlength > 8)
		soundlength -= 8;

	channels[chan].data = (unsigned char*)sfx->data;
	channels[chan].end = channels[chan].data + soundlength;
	channels[chan].stepremainder = 0;
	channels[chan].step = FRACUNIT;  // Use standard pitch for now
	channels[chan].volume = vol;
	channels[chan].left_volume = leftvol;
	channels[chan].right_volume = rightvol;
	channels[chan].id = id;
	channels[chan].start_gametic = gametic;
	channels[chan].handle = chan;

	return chan;
}

//
// I_StopSound
//
void I_StopSound(int handle)
{
	if (handle < 0 || handle >= NUM_CHANNELS)
		return;

	channels[handle].id = 0;
	channels[handle].data = NULL;
}

//
// I_SoundIsPlaying
//
int I_SoundIsPlaying(int handle)
{
	if (handle < 0 || handle >= NUM_CHANNELS)
		return 0;

	return (channels[handle].id != 0);
}

//
// I_UpdateSound
// Mix all active sounds and update the audio buffer
//
void I_UpdateSound(void)
{
	int i, chan;
	signed short* ptr;
	int sample;
	int left, right;
	channel_t* ch;

	if (audio_device == 0)
		return;

	// Clear the mixing buffer
	memset(mixbuffer, 0, MIXBUFFERSIZE * sizeof(signed short));

	// Mix each active channel
	for (chan = 0; chan < NUM_CHANNELS; chan++)
	{
		ch = &channels[chan];

		if (!ch->data || ch->id == 0)
			continue;

		// Mix samples
		ptr = mixbuffer;

		for (i = 0; i < SAMPLECOUNT; i++)
		{
			// Stop if reached end of sound
			if (ch->data >= ch->end)
			{
				ch->id = 0;
				break;
			}

			// Get sample and step
			sample = (signed char)*ch->data;
			ch->stepremainder += ch->step;

			if (ch->stepremainder & 0xffff0000)
			{
				ch->data += (ch->stepremainder >> 16);
				ch->stepremainder &= 0xffff;
			}

			// Volume lookup
			left = vol_lookup[ch->left_volume * 256 + ((sample + 128) & 0xff)];
			right = vol_lookup[ch->right_volume * 256 + ((sample + 128) & 0xff)];

			// Mix to buffer (stereo interleaved)
			ptr[0] += left;
			ptr[1] += right;
			ptr += 2;
		}
	}

	// Queue the mixed audio
	if (SDL_GetQueuedAudioSize(audio_device) == 0)
	{
		SDL_QueueAudio(audio_device, mixbuffer, MIXBUFFERSIZE * sizeof(signed short));
	}
}

//
// I_SubmitSound
// Called each game tic to update sound
//
void I_SubmitSound(void)
{
	I_UpdateSound();
}

//
// I_InitSoundModule
// Silence driver module
//
static void I_InitSoundModule(void)
{
	I_InitSound();
}

//
// I_ShutdownSoundModule
//
static void I_ShutdownSoundModule(void)
{
	I_ShutdownSound();
}

//
// STUB FUNCTIONS for music and other I_ functions
//

// Forward declaration from i_video_sdl.c
extern void I_UpdateGraphics(void);

int I_RegisterSong(void *data)
{
	// Not implemented - return dummy handle
	return 0;
}

void I_UnRegisterSong(int handle)
{
	// Not implemented
}

void I_PlaySong(int handle, int looping)
{
	// Not implemented - would need a proper music player
}

void I_StopSong(int handle)
{
	// Not implemented
}

void I_PauseSong(int handle)
{
	// Not implemented
}

void I_ResumeSong(int handle)
{
	// Not implemented
}

void I_UpdateNETgamestateFinished(void)
{
	// Not implemented - network multiplayer
}

void I_UpdateNETgamestate(void)
{
	// Not implemented - network multiplayer
}

void I_InitNetwork(void)
{
	// Not implemented - network multiplayer
}

void I_NetCmd(void)
{
	// Not implemented - network multiplayer
}

int I_GetSfxLumpNum(sfxinfo_t *sfxinfo)
{
	// Return the lump number for a sound effect
	return (sfxinfo ? sfxinfo->lumpnum : -1);
}

void I_FinishUpdate(void)
{
	// Update the display (called from main game loop)
	I_UpdateGraphics();
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
	// Not implemented - would update parameters of a playing sound
	// For now, sounds play at fixed parameters
}
