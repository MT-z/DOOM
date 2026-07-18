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

#ifdef USE_SDL_MIXER
#include "SDL2/SDL_mixer.h"
#include "mus2mid.h"
#endif

#ifdef USE_OPL_MUSIC
#include "i_oplmusic.h"
#endif

#include "z_zone.h"
#include "i_system.h"
#include "i_sound.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "doomdef.h"
#include "d_net.h"
#include "doomstat.h"

// Sound device state
static SDL_AudioDeviceID audio_device = 0;
static SDL_AudioSpec obtained_spec;

#ifdef USE_SDL_MIXER
// Music state (SDL2_mixer, MUS lumps converted to MIDI)
static SDL_bool music_initialized = SDL_FALSE;
static Mix_Music* music = NULL;
static unsigned char* music_mididata = NULL;
#endif

#ifdef USE_OPL_MUSIC
// Nonzero when the OPL2 FM synthesis backend is active
static int opl_music = 0;
// Nonzero when OPL output is mixed into the SFX stream instead
// of playing on its own audio device (single-device platforms)
static int opl_external = 0;
#endif

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

// Pitch step table: maps the 0-255 pitch parameter to a 16.16
// resampling step (128 = normal pitch, 64 steps per octave).
static int steptable[256];

// Mixing buffer
static signed short mixbuffer[MIXBUFFERSIZE];

// Forward declarations
static void I_LoadSfxLump(sfxinfo_t* sfx);
int I_GetSfxLumpNum(sfxinfo_t *sfxinfo);

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
	// Original DOOM formula: signed output scaled to 16-bit range.
	// Index j is the raw unsigned 8-bit sample (centered at 128).
	for (i = 0; i < 128; i++)
	{
		int j;
		for (j = 0; j < 256; j++)
			vol_lookup[i * 256 + j] = (i * (j - 128) * 256) / 127;
	}

	// Initialize pitch step table (as in the original DOS version)
	for (i = 0; i < 256; i++)
		steptable[i] = (int)(pow(2.0, (i - 128) / 64.0) * 65536.0);

	// Pre-cache all sound effects, as s_sound.c expects
	// (it warns "16bit and not pre-cached - wtf?" otherwise).
	for (i = 1; i < NUMSFX; i++)
	{
		if (!S_sfx[i].link)
		{
			S_sfx[i].lumpnum = I_GetSfxLumpNum(&S_sfx[i]);
			I_LoadSfxLump(&S_sfx[i]);
		}
	}
	// Second pass: linked sounds share the linked-to data
	for (i = 1; i < NUMSFX; i++)
	{
		if (S_sfx[i].link)
		{
			S_sfx[i].data = S_sfx[i].link->data;
			S_sfx[i].lumpnum = S_sfx[i].link->lumpnum;
		}
	}

	// Start audio playback
	SDL_PauseAudioDevice(audio_device, 0);

#ifdef USE_OPL_MUSIC
	// OPL2 FM synthesis (Sound Blaster / AdLib style) is the
	// default music backend; pass -gmmusic for General MIDI.
	if (!M_CheckParm("-gmmusic"))
	{
		if (OPL_Init())
		{
			opl_music = 1;
			OPL_SetMusicVolume(snd_MusicVolume);
			printf("Music initialized (OPL2 FM synthesis)\n");
			return;
		}
		// Some platforms (iOS) allow only one open audio device;
		// fall back to mixing the music into the SFX stream.
		if (OPL_InitExternal(obtained_spec.freq))
		{
			opl_music = 1;
			opl_external = 1;
			OPL_SetMusicVolume(snd_MusicVolume);
			printf("Music initialized (OPL2 FM synthesis, mixed with SFX)\n");
			return;
		}
	}
#endif

#ifdef USE_SDL_MIXER
	// Initialize music playback on a separate mixer device.
	// MIDI rendering is handled by SDL2_mixer via FluidSynth,
	// which needs a GM soundfont (.sf2).
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) < 0)
	{
		fprintf(stderr, "I_InitSound: music disabled: %s\n", Mix_GetError());
	}
	else
	{
		// Locate a soundfont unless one is already configured
		// via the SDL_SOUNDFONTS environment variable.
		if (!SDL_getenv("SDL_SOUNDFONTS"))
		{
			static const char* soundfont_paths[] =
			{
				"/opt/homebrew/share/soundfonts/default.sf2",
				"/usr/local/share/soundfonts/default.sf2",
				"/usr/share/soundfonts/default.sf2",
				NULL
			};
			int sf;
			for (sf = 0; soundfont_paths[sf]; sf++)
			{
				FILE* f = fopen(soundfont_paths[sf], "rb");
				if (f)
				{
					fclose(f);
					Mix_SetSoundFonts(soundfont_paths[sf]);
					break;
				}
			}
			if (!soundfont_paths[sf])
				fprintf(stderr, "I_InitSound: no soundfont found; "
					"music may be silent (set SDL_SOUNDFONTS)\n");
		}

		music_initialized = SDL_TRUE;
		Mix_VolumeMusic(snd_MusicVolume * MIX_MAX_VOLUME / 15);
		printf("Music initialized (SDL2_mixer)\n");
	}
#endif
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

#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_SetMusicVolume(snd_MusicVolume);
		return;
	}
#endif
#ifdef USE_SDL_MIXER
	if (music_initialized)
		Mix_VolumeMusic(snd_MusicVolume * MIX_MAX_VOLUME / 15);
#endif
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

	// Lump -1 means the sound is absent from this WAD
	// (e.g. DOOM II sounds when running DOOM 1)
	if (sfx->lumpnum < 0)
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

	// Clamp volumes to lookup table range (0-127)
	if (leftvol < 0) leftvol = 0;
	if (leftvol > 127) leftvol = 127;
	if (rightvol < 0) rightvol = 0;
	if (rightvol > 127) rightvol = 127;

	// Calculate sound length from lump size
	int soundlength = W_LumpLength(sfx->lumpnum);
	if (soundlength > 8)
		soundlength -= 8;

	channels[chan].data = (unsigned char*)sfx->data;
	channels[chan].end = channels[chan].data + soundlength;
	channels[chan].stepremainder = 0;
	channels[chan].step = steptable[pitch & 0xff];
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

	// Keep a couple of buffers queued to avoid gaps without
	// building up latency. Check before mixing so no rendered
	// audio (in particular music) is ever dropped.
	if (SDL_GetQueuedAudioSize(audio_device) >= SAMPLECOUNT * 2 * sizeof(signed short) * 2)
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

			// Get raw unsigned 8-bit sample (centered at 128);
			// vol_lookup handles the center offset
			sample = *ch->data;
			ch->stepremainder += ch->step;

			if (ch->stepremainder & 0xffff0000)
			{
				ch->data += (ch->stepremainder >> 16);
				ch->stepremainder &= 0xffff;
			}

			// Volume lookup (signed 16-bit output)
			left = ptr[0] + vol_lookup[ch->left_volume * 256 + sample];
			right = ptr[1] + vol_lookup[ch->right_volume * 256 + sample];

			// Mix to buffer with saturation (stereo interleaved)
			if (left > 0x7fff) left = 0x7fff;
			else if (left < -0x8000) left = -0x8000;
			if (right > 0x7fff) right = 0x7fff;
			else if (right < -0x8000) right = -0x8000;
			ptr[0] = left;
			ptr[1] = right;
			ptr += 2;
		}
	}

#ifdef USE_OPL_MUSIC
	// Mix music into the SFX stream on single-device platforms
	if (opl_external)
		OPL_Mix(mixbuffer, SAMPLECOUNT);
#endif

	SDL_QueueAudio(audio_device, mixbuffer, SAMPLECOUNT * 2 * sizeof(signed short));
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
// MUSIC (SDL2_mixer) and other I_ functions
//

// Forward declaration from i_video_sdl.c
extern void I_UpdateGraphics(void);

int I_RegisterSong(void *data)
{
#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_RegisterSong(data);
		return 0;
	}
#endif
#ifdef USE_SDL_MIXER
	const unsigned char* mus = (const unsigned char*)data;
	unsigned char* mididata = NULL;
	int muslen, midilen;
	SDL_RWops* rw;

	if (!music_initialized || !data)
		return 0;

	// Free any previous song
	I_UnRegisterSong(0);

	// The MUS header carries the total data size
	if (memcmp(mus, "MUS\x1a", 4) != 0)
	{
		fprintf(stderr, "I_RegisterSong: not a MUS lump\n");
		return 0;
	}
	muslen = (mus[6] | (mus[7] << 8)) + (mus[4] | (mus[5] << 8));

	if (mus2mid(mus, muslen, &mididata, &midilen) != 0)
	{
		fprintf(stderr, "I_RegisterSong: MUS to MIDI conversion failed\n");
		return 0;
	}

	rw = SDL_RWFromMem(mididata, midilen);
	music = Mix_LoadMUS_RW(rw, SDL_TRUE);
	if (!music)
	{
		fprintf(stderr, "I_RegisterSong: %s\n", Mix_GetError());
		free(mididata);
		return 0;
	}

	music_mididata = mididata;
#endif
	return 0;
}

void I_UnRegisterSong(int handle)
{
#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_UnRegisterSong();
		return;
	}
#endif
#ifdef USE_SDL_MIXER
	if (music)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	if (music_mididata)
	{
		free(music_mididata);
		music_mididata = NULL;
	}
#endif
}

void I_PlaySong(int handle, int looping)
{
#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_PlaySong(looping);
		return;
	}
#endif
#ifdef USE_SDL_MIXER
	if (!music)
		return;

	if (Mix_PlayMusic(music, looping ? -1 : 1) < 0)
		fprintf(stderr, "I_PlaySong: %s\n", Mix_GetError());
	else
		Mix_VolumeMusic(snd_MusicVolume * MIX_MAX_VOLUME / 15);
#endif
}

void I_StopSong(int handle)
{
#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_StopSong();
		return;
	}
#endif
#ifdef USE_SDL_MIXER
	Mix_HaltMusic();
#endif
}

void I_PauseSong(int handle)
{
#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_PauseSong();
		return;
	}
#endif
#ifdef USE_SDL_MIXER
	Mix_PauseMusic();
#endif
}

void I_ResumeSong(int handle)
{
#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_ResumeSong();
		return;
	}
#endif
#ifdef USE_SDL_MIXER
	Mix_ResumeMusic();
#endif
}

void I_ShutdownMusic(void)
{
#ifdef USE_OPL_MUSIC
	if (opl_music)
	{
		OPL_Shutdown();
		opl_music = 0;
		return;
	}
#endif
#ifdef USE_SDL_MIXER
	if (music_initialized)
	{
		I_UnRegisterSong(0);
		Mix_CloseAudio();
		music_initialized = SDL_FALSE;
	}
#endif
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
	// Single-player only: network multiplayer is not implemented.
	if (!doomcom)
	{
		doomcom = malloc(sizeof(doomcom_t));
		if (!doomcom)
			I_Error("I_InitNetwork: malloc failed");
	}

	memset(doomcom, 0, sizeof(doomcom_t));
	doomcom->id = DOOMCOM_ID;
	doomcom->numplayers = 1;
	doomcom->numnodes = 1;
	doomcom->deathmatch = 0;
	doomcom->consoleplayer = 0;
	doomcom->ticdup = 1;
	doomcom->extratics = 0;
}

void I_NetCmd(void)
{
	// Not implemented - network multiplayer
}

int I_GetSfxLumpNum(sfxinfo_t *sfxinfo)
{
	// Resolve the WAD lump for a sound effect.
	// Linked sounds (e.g. chaingun -> pistol) share the
	// linked-to sound's lump.
	char namebuf[9];

	if (!sfxinfo)
		return -1;

	if (sfxinfo->link)
		sfxinfo = sfxinfo->link;

	sprintf(namebuf, "ds%s", sfxinfo->name);

	// Use CheckNumForName: some sounds (DOOM II only) are
	// absent from DOOM 1 WADs and must not be fatal.
	return W_CheckNumForName(namebuf);
}

void I_FinishUpdate(void)
{
	// Update the display (called from main game loop)
	I_UpdateGraphics();
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
	int leftvol;
	int rightvol;

	if (handle < 0 || handle >= NUM_CHANNELS)
		return;
	if (channels[handle].id == 0)
		return;

	// Same separation math as I_StartSound
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

	if (leftvol < 0) leftvol = 0;
	if (leftvol > 127) leftvol = 127;
	if (rightvol < 0) rightvol = 0;
	if (rightvol > 127) rightvol = 127;

	channels[handle].volume = vol;
	channels[handle].left_volume = leftvol;
	channels[handle].right_volume = rightvol;
	channels[handle].step = steptable[pitch & 0xff];
}
