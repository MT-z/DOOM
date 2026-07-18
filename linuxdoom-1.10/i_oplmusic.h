// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//	OPL2 FM synthesis music driver (Sound Blaster / AdLib style).
//	Plays MUS lumps through the Nuked OPL3 emulator using the
//	GENMIDI instrument definitions from the IWAD.
//
//-----------------------------------------------------------------------------

#ifndef __I_OPLMUSIC_H__
#define __I_OPLMUSIC_H__

#include <stdint.h>

// Initialize the OPL music system. Requires the GENMIDI lump
// (checked via the WAD) and an SDL audio device.
// Returns 1 on success, 0 on failure.
int OPL_Init(void);

// Initialize without opening an audio device; the caller must
// pump samples via OPL_Mix. For platforms where SDL supports
// only one open audio device (e.g. iOS).
int OPL_InitExternal(int samplerate);

// External mixing mode only: render nframes of music and add
// them into an interleaved stereo int16 buffer.
void OPL_Mix(int16_t* stream, int nframes);

void OPL_Shutdown(void);

// Register a MUS song (data must remain valid until unregistered).
// Returns 1 on success.
int OPL_RegisterSong(const void* data);

void OPL_UnRegisterSong(void);
void OPL_PlaySong(int looping);
void OPL_StopSong(void);
void OPL_PauseSong(void);
void OPL_ResumeSong(void);

// Set music volume, 0-15 (DOOM menu scale).
void OPL_SetMusicVolume(int volume);

#endif // __I_OPLMUSIC_H__
