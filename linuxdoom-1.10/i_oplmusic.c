// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//	OPL2 FM synthesis music driver (Sound Blaster / AdLib style).
//
//	Plays MUS lumps through the Nuked OPL3 emulator (running in
//	OPL2 compatibility mode, 9 channels) using the GENMIDI
//	instrument definitions shipped inside the IWAD -- the same
//	FM patches DMX used on a real Sound Blaster in 1993.
//
//	The MUS sequencer runs inside the SDL audio callback:
//	events are processed at 140 Hz and the emulator renders
//	samples in between, so timing is sample-accurate.
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SDL2/SDL.h"

#include "opl3.h"
#include "i_oplmusic.h"
#include "w_wad.h"
#include "z_zone.h"

// OPL master clock and output
#define OPL_SAMPLERATE		44100
#define OPL_CHIP_CLOCK		49716.0
#define MUS_TICKS_PER_SEC	140

#define OPL_NUM_VOICES		9
#define MUS_NUM_CHANNELS	16
#define MUS_PERCUSSION		15

// Output gain applied after FM synthesis (with saturation).
// The raw OPL2 output of 9 voices is quiet compared to the
// 8-bit SFX mixer, so boost it to a comparable loudness.
#define OPL_GAIN			3

// GENMIDI lump layout
#define GENMIDI_HEADER		"#OPL_II#"
#define GENMIDI_NUM_INSTRS	175
#define GENMIDI_INSTR_SIZE	36

// GENMIDI instrument flags
#define GENMIDI_FLAG_FIXED	0x0001

// Byte offsets inside a 36-byte GENMIDI record.
// Voice block (16 bytes) appears twice, at offsets 4 and 20:
//   +0 modulator tremolo/vibrato/sustain/KSR/multiplier (reg 0x20)
//   +1 modulator attack/decay                           (reg 0x60)
//   +2 modulator sustain/release                        (reg 0x80)
//   +3 modulator waveform                               (reg 0xE0)
//   +4 modulator key scale level                        (reg 0x40 hi)
//   +5 modulator output level                           (reg 0x40 lo)
//   +6 feedback/connection                              (reg 0xC0)
//   +7..12 carrier: same six parameters
//   +13 unused
//   +14..15 base note offset (signed 16-bit LE)

typedef struct
{
	int		active;
	int		mus_chan;	// owning MUS channel, -1 if free
	int		note;		// note as sent in the MUS event (for note-off)
	int		instr;		// GENMIDI instrument index
	int		note_vol;	// note-on volume 0-127
	int		age;		// for voice stealing
	double	freq_note;	// note used for frequency (after offsets)
	int		car_scale;	// carrier KSL bits (register 0x40 high bits)
	int		car_level;	// carrier output level 0-63 from the instrument
} opl_voice_t;

// Chip and device
static opl3_chip		chip;
static SDL_AudioDeviceID	opl_device = 0;

// GENMIDI instrument data (record area, past the 8-byte header)
static const unsigned char*	genmidi = NULL;

// Song state (accessed from the audio callback; guard changes
// from the game thread with SDL_LockAudioDevice)
static const unsigned char*	mus_song = NULL;	// registered lump
static const unsigned char*	mus_start = NULL;	// score start
static const unsigned char*	mus_end = NULL;
static const unsigned char*	mus_pos = NULL;
static int				music_playing = 0;
static int				music_paused = 0;
static int				music_looping = 0;
static double			samples_per_tick;
static double			tick_accum = 0.0;	// samples until next event

// Per-MUS-channel state
static int				chan_instr[MUS_NUM_CHANNELS];
static int				chan_vol[MUS_NUM_CHANNELS];		// controller 3
static int				chan_notevol[MUS_NUM_CHANNELS];	// last note velocity
static double			chan_bend[MUS_NUM_CHANNELS];	// in semitones

// Voices
static opl_voice_t		voices[OPL_NUM_VOICES];
static int				voice_age = 0;

// Master music volume 0-15
static int				master_vol = 8;

// OPL2 operator offsets for the modulator of channels 0-8;
// the carrier is at offset + 3.
static const int op_offset[OPL_NUM_VOICES] =
{
	0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10, 0x11, 0x12
};

//
// Low-level helpers
//

static void WriteReg(int reg, int value)
{
	OPL3_WriteReg(&chip, (uint16_t)reg, (uint8_t)value);
}

// Compute the carrier total-level byte for a voice, combining the
// instrument's own level with note velocity, channel volume and the
// master music volume (DMX-style multiplicative attenuation).
static int VoiceCarrierLevel(const opl_voice_t* v)
{
	int vol;

	vol = (v->note_vol * chan_vol[v->mus_chan]) / 127;
	vol = (vol * master_vol) / 15;
	if (vol > 127)
		vol = 127;
	if (vol < 0)
		vol = 0;

	return v->car_scale
	     | (0x3f - ((0x3f - v->car_level) * vol) / 127);
}

// Program the instrument parameters into an OPL channel.
static void VoiceSetInstrument(int vnum, const unsigned char* patch)
{
	int mod = op_offset[vnum];
	int car = mod + 3;

	WriteReg(0x20 + mod, patch[0]);		// mod trem/vib/sus/ksr/mult
	WriteReg(0x60 + mod, patch[1]);		// mod attack/decay
	WriteReg(0x80 + mod, patch[2]);		// mod sustain/release
	WriteReg(0xE0 + mod, patch[3]);		// mod waveform
	WriteReg(0x40 + mod, (patch[4] & 0xc0) | (patch[5] & 0x3f));

	WriteReg(0x20 + car, patch[7]);		// car trem/vib/sus/ksr/mult
	WriteReg(0x60 + car, patch[8]);		// car attack/decay
	WriteReg(0x80 + car, patch[9]);		// car sustain/release
	WriteReg(0xE0 + car, patch[10]);	// car waveform
	// carrier 0x40 is written by VoiceUpdateVolume

	WriteReg(0xC0 + vnum, patch[6]);	// feedback/connection
}

static void VoiceUpdateVolume(int vnum)
{
	WriteReg(0x40 + op_offset[vnum] + 3, VoiceCarrierLevel(&voices[vnum]));
}

// Write frequency and key-on for a voice.
static void VoiceKeyOn(int vnum)
{
	opl_voice_t* v = &voices[vnum];
	double note = v->freq_note + chan_bend[v->mus_chan];
	double freq = 440.0 * pow(2.0, (note - 69.0) / 12.0);
	int block, fnum;

	for (block = 0; block < 7; block++)
	{
		fnum = (int)(freq * (double)(1 << (20 - block)) / OPL_CHIP_CLOCK + 0.5);
		if (fnum < 1024)
			break;
	}
	if (fnum > 1023)
		fnum = 1023;

	WriteReg(0xA0 + vnum, fnum & 0xff);
	WriteReg(0xB0 + vnum, 0x20 | (block << 2) | (fnum >> 8));
}

static void VoiceKeyOff(int vnum)
{
	// Clear only the key-on bit; frequency bits do not matter
	// for the release phase envelope.
	WriteReg(0xB0 + vnum, 0x00);
	voices[vnum].active = 0;
}

static void AllVoicesOff(void)
{
	int i;
	for (i = 0; i < OPL_NUM_VOICES; i++)
		VoiceKeyOff(i);
}

//
// MUS event handling (audio-callback context)
//

static void EventNoteOff(int mus_chan, int note)
{
	int i;
	for (i = 0; i < OPL_NUM_VOICES; i++)
	{
		if (voices[i].active
		    && voices[i].mus_chan == mus_chan
		    && voices[i].note == note)
		{
			VoiceKeyOff(i);
		}
	}
}

static void EventNoteOn(int mus_chan, int note, int vol)
{
	const unsigned char* patch;
	int instr;
	int vnum, i;
	int oldest_age;
	opl_voice_t* v;
	double freq_note;

	// Pick the instrument. On the percussion channel the note
	// number selects the patch (GENMIDI records 128-174 map to
	// MIDI keys 35-81) and the pitch comes from the patch itself.
	if (mus_chan == MUS_PERCUSSION)
	{
		if (note < 35 || note > 81)
			return;
		instr = 128 + note - 35;
	}
	else
	{
		instr = chan_instr[mus_chan];
	}

	if (instr < 0 || instr >= GENMIDI_NUM_INSTRS)
		return;

	patch = genmidi + instr * GENMIDI_INSTR_SIZE;

	// Note used for the oscillator frequency
	if ((patch[0] | (patch[1] << 8)) & GENMIDI_FLAG_FIXED)
		freq_note = patch[3];			// fixed note
	else if (mus_chan == MUS_PERCUSSION)
		freq_note = patch[3] ? patch[3] : 60;
	else
		freq_note = note;

	// Add the first voice's base note offset (signed 16-bit LE)
	{
		int off = patch[4 + 14] | (patch[4 + 15] << 8);
		if (off >= 0x8000)
			off -= 0x10000;
		freq_note += off;
	}

	// Allocate a voice: prefer a free one, otherwise steal the oldest
	vnum = -1;
	oldest_age = 0x7fffffff;
	for (i = 0; i < OPL_NUM_VOICES; i++)
	{
		if (!voices[i].active)
		{
			vnum = i;
			break;
		}
		if (voices[i].age < oldest_age)
		{
			oldest_age = voices[i].age;
			vnum = i;
		}
	}

	if (voices[vnum].active)
		VoiceKeyOff(vnum);

	v = &voices[vnum];
	v->active = 1;
	v->mus_chan = mus_chan;
	v->note = note;
	v->instr = instr;
	v->note_vol = vol & 0x7f;
	v->age = voice_age++;
	v->freq_note = freq_note;
	v->car_scale = patch[4 + 11] & 0xc0;
	v->car_level = patch[4 + 12] & 0x3f;

	VoiceSetInstrument(vnum, patch + 4);
	VoiceUpdateVolume(vnum);
	VoiceKeyOn(vnum);
}

static void EventPitchWheel(int mus_chan, int value)
{
	int i;

	// MUS: 0-255, 128 = center, 64 = one semitone
	chan_bend[mus_chan] = (value - 128) / 64.0;

	// Re-tune sounding notes on this channel
	for (i = 0; i < OPL_NUM_VOICES; i++)
	{
		if (voices[i].active && voices[i].mus_chan == mus_chan)
			VoiceKeyOn(i);
	}
}

static void EventController(int mus_chan, int ctrl, int value)
{
	int i;

	switch (ctrl)
	{
	case 0:		// program change
		if (mus_chan != MUS_PERCUSSION)
			chan_instr[mus_chan] = value;
		break;

	case 3:		// channel volume
		chan_vol[mus_chan] = value & 0x7f;
		for (i = 0; i < OPL_NUM_VOICES; i++)
		{
			if (voices[i].active && voices[i].mus_chan == mus_chan)
				VoiceUpdateVolume(i);
		}
		break;

	default:	// pan and others: OPL2 output is mono, ignore
		break;
	}
}

static void ChannelAllNotesOff(int mus_chan)
{
	int i;
	for (i = 0; i < OPL_NUM_VOICES; i++)
	{
		if (voices[i].active && voices[i].mus_chan == mus_chan)
			VoiceKeyOff(i);
	}
}

static void ResetPlaybackState(void)
{
	int i;

	for (i = 0; i < MUS_NUM_CHANNELS; i++)
	{
		chan_instr[i] = 0;
		chan_vol[i] = 127;
		chan_notevol[i] = 127;
		chan_bend[i] = 0.0;
	}
	mus_pos = mus_start;
	tick_accum = 0.0;
}

// Process MUS events until a nonzero delay is reached or the song
// ends. Returns the delay in MUS ticks (0 means playback stopped).
static int ProcessEvents(void)
{
	while (mus_pos < mus_end)
	{
		unsigned char evbyte = *mus_pos++;
		int last = evbyte & 0x80;
		int type = (evbyte >> 4) & 7;
		int mus_chan = evbyte & 15;
		unsigned char b1, b2;

		switch (type)
		{
		case 0:		// release note
			if (mus_pos >= mus_end) goto endsong;
			b1 = *mus_pos++ & 0x7f;
			EventNoteOff(mus_chan, b1);
			break;

		case 1:		// play note
			if (mus_pos >= mus_end) goto endsong;
			b1 = *mus_pos++;
			if (b1 & 0x80)
			{
				if (mus_pos >= mus_end) goto endsong;
				b2 = *mus_pos++ & 0x7f;
				chan_notevol[mus_chan] = b2;
				EventNoteOn(mus_chan, b1 & 0x7f, b2);
			}
			else
			{
				EventNoteOn(mus_chan, b1, chan_notevol[mus_chan]);
			}
			break;

		case 2:		// pitch wheel
			if (mus_pos >= mus_end) goto endsong;
			EventPitchWheel(mus_chan, *mus_pos++);
			break;

		case 3:		// system event
			if (mus_pos >= mus_end) goto endsong;
			b1 = *mus_pos++;
			if (b1 == 10 || b1 == 11)
				ChannelAllNotesOff(mus_chan);
			break;

		case 4:		// controller
			if (mus_pos + 1 >= mus_end) goto endsong;
			b1 = *mus_pos++;
			b2 = *mus_pos++;
			EventController(mus_chan, b1, b2);
			break;

		case 5:		// end of measure
			break;

		case 6:		// score end
			goto endsong;

		case 7:		// unused
			if (mus_pos >= mus_end) goto endsong;
			mus_pos++;
			break;
		}

		if (last)
		{
			unsigned int time = 0;
			unsigned char b;
			do
			{
				if (mus_pos >= mus_end)
					goto endsong;
				b = *mus_pos++;
				time = (time << 7) | (b & 0x7f);
			} while (b & 0x80);

			if (time > 0)
				return (int)time;
		}
	}

endsong:
	if (music_looping)
	{
		AllVoicesOff();
		ResetPlaybackState();
		return 1;	// come back next tick
	}

	AllVoicesOff();
	music_playing = 0;
	return 0;
}

//
// SDL audio callback: sequence and render
//

static void OPL_AudioCallback(void* userdata, Uint8* stream, int len)
{
	int16_t* out = (int16_t*)stream;
	int nframes = len / (2 * sizeof(int16_t));

	while (nframes > 0)
	{
		int chunk = nframes;

		if (music_playing && !music_paused)
		{
			while (tick_accum < 1.0 && music_playing)
			{
				int delay = ProcessEvents();
				if (delay <= 0)
					break;
				tick_accum += delay * samples_per_tick;
			}
			if (music_playing && tick_accum < (double)chunk)
			{
				chunk = (int)tick_accum;
				if (chunk < 1)
					chunk = 1;
			}
		}

		// Render even when stopped/paused so envelopes decay
		OPL3_GenerateStream(&chip, out, (uint32_t)chunk);

		// Apply output gain with saturation
		{
			int i;
			for (i = 0; i < chunk * 2; i++)
			{
				int s = out[i] * OPL_GAIN;
				if (s > 0x7fff) s = 0x7fff;
				else if (s < -0x8000) s = -0x8000;
				out[i] = (int16_t)s;
			}
		}

		out += chunk * 2;
		nframes -= chunk;

		if (music_playing && !music_paused)
			tick_accum -= (double)chunk;
	}
}

//
// Public interface (game-thread context)
//

int OPL_Init(void)
{
	SDL_AudioSpec desired, obtained;
	int lumpnum;
	const unsigned char* lump;

	// GENMIDI lump carries the FM instrument set
	lumpnum = W_CheckNumForName("GENMIDI");
	if (lumpnum < 0)
	{
		fprintf(stderr, "OPL_Init: no GENMIDI lump in WAD\n");
		return 0;
	}

	lump = (const unsigned char*)W_CacheLumpNum(lumpnum, PU_STATIC);
	if (W_LumpLength(lumpnum) < 8 + GENMIDI_NUM_INSTRS * GENMIDI_INSTR_SIZE
	    || memcmp(lump, GENMIDI_HEADER, 8) != 0)
	{
		fprintf(stderr, "OPL_Init: bad GENMIDI lump\n");
		return 0;
	}
	genmidi = lump + 8;

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		return 0;

	SDL_zero(desired);
	desired.freq = OPL_SAMPLERATE;
	desired.format = AUDIO_S16SYS;
	desired.channels = 2;
	desired.samples = 512;
	desired.callback = OPL_AudioCallback;

	opl_device = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
	if (opl_device == 0)
	{
		fprintf(stderr, "OPL_Init: %s\n", SDL_GetError());
		return 0;
	}

	OPL3_Reset(&chip, (uint32_t)obtained.freq);
	samples_per_tick = (double)obtained.freq / MUS_TICKS_PER_SEC;

	// OPL2 mode: leave the OPL3 NEW bit clear, enable waveform
	// select, no rhythm mode, all keys off.
	WriteReg(0x01, 0x20);
	WriteReg(0xBD, 0x00);
	AllVoicesOff();
	memset(voices, 0, sizeof(voices));
	ResetPlaybackState();

	SDL_PauseAudioDevice(opl_device, 0);
	return 1;
}

void OPL_Shutdown(void)
{
	if (opl_device)
	{
		SDL_CloseAudioDevice(opl_device);
		opl_device = 0;
	}
}

int OPL_RegisterSong(const void* data)
{
	const unsigned char* mus = (const unsigned char*)data;
	int scorelen, scorestart;

	if (!opl_device || !mus || memcmp(mus, "MUS\x1a", 4) != 0)
		return 0;

	scorelen = mus[4] | (mus[5] << 8);
	scorestart = mus[6] | (mus[7] << 8);

	SDL_LockAudioDevice(opl_device);
	music_playing = 0;
	AllVoicesOff();
	mus_song = mus;
	mus_start = mus + scorestart;
	mus_end = mus_start + scorelen;
	ResetPlaybackState();
	SDL_UnlockAudioDevice(opl_device);

	return 1;
}

void OPL_UnRegisterSong(void)
{
	if (!opl_device)
		return;

	SDL_LockAudioDevice(opl_device);
	music_playing = 0;
	AllVoicesOff();
	mus_song = mus_start = mus_end = mus_pos = NULL;
	SDL_UnlockAudioDevice(opl_device);
}

void OPL_PlaySong(int looping)
{
	if (!opl_device || !mus_song)
		return;

	SDL_LockAudioDevice(opl_device);
	ResetPlaybackState();
	music_looping = looping;
	music_paused = 0;
	music_playing = 1;
	SDL_UnlockAudioDevice(opl_device);
}

void OPL_StopSong(void)
{
	if (!opl_device)
		return;

	SDL_LockAudioDevice(opl_device);
	music_playing = 0;
	AllVoicesOff();
	SDL_UnlockAudioDevice(opl_device);
}

void OPL_PauseSong(void)
{
	if (!opl_device)
		return;

	SDL_LockAudioDevice(opl_device);
	music_paused = 1;
	AllVoicesOff();
	SDL_UnlockAudioDevice(opl_device);
}

void OPL_ResumeSong(void)
{
	if (!opl_device)
		return;

	SDL_LockAudioDevice(opl_device);
	music_paused = 0;
	SDL_UnlockAudioDevice(opl_device);
}

void OPL_SetMusicVolume(int volume)
{
	int i;

	if (volume < 0) volume = 0;
	if (volume > 15) volume = 15;

	if (!opl_device)
	{
		master_vol = volume;
		return;
	}

	SDL_LockAudioDevice(opl_device);
	master_vol = volume;
	for (i = 0; i < OPL_NUM_VOICES; i++)
	{
		if (voices[i].active)
			VoiceUpdateVolume(i);
	}
	SDL_UnlockAudioDevice(opl_device);
}
