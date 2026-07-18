// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//	MUS (DMX music format) to Standard MIDI File converter.
//
//	The MUS format is a compact MIDI-like format used by DMX,
//	the sound library of DOOM. This module converts a MUS lump
//	into a format-0 Standard MIDI File in memory so that it can
//	be played back with any MIDI-capable player (SDL2_mixer).
//
//	MUS events run at 140 Hz. We use a MIDI division of 70
//	ticks per quarter note; with the default MIDI tempo of
//	500000 usec per quarter note this gives exactly 140
//	ticks per second, so MUS delay values map 1:1 to MIDI
//	delta times.
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mus2mid.h"

// MUS event types (bits 4-6 of the event byte)
#define MUS_EV_RELEASE_NOTE	0
#define MUS_EV_PLAY_NOTE	1
#define MUS_EV_PITCH_WHEEL	2
#define MUS_EV_SYSTEM		3
#define MUS_EV_CONTROLLER	4
#define MUS_EV_END_MEASURE	5
#define MUS_EV_SCORE_END	6
#define MUS_EV_UNUSED		7

// MIDI channel used for percussion
#define MIDI_PERCUSSION		9
// MUS channel that carries percussion
#define MUS_PERCUSSION		15

// Growable output buffer
typedef struct
{
	unsigned char*	data;
	int				len;
	int				cap;
} membuf_t;

static int buf_write(membuf_t* buf, const unsigned char* bytes, int count)
{
	if (buf->len + count > buf->cap)
	{
		int newcap = buf->cap ? buf->cap * 2 : 4096;
		while (newcap < buf->len + count)
			newcap *= 2;
		unsigned char* p = (unsigned char*)realloc(buf->data, newcap);
		if (!p)
			return -1;
		buf->data = p;
		buf->cap = newcap;
	}
	memcpy(buf->data + buf->len, bytes, count);
	buf->len += count;
	return 0;
}

static int buf_write1(membuf_t* buf, unsigned char b)
{
	return buf_write(buf, &b, 1);
}

// Write a MIDI variable-length quantity
static int write_varlen(membuf_t* buf, unsigned int value)
{
	unsigned char out[4];
	int count = 0;
	int i;

	out[count++] = value & 0x7f;
	while (value >>= 7)
		out[count++] = (value & 0x7f) | 0x80;

	for (i = count - 1; i >= 0; i--)
	{
		if (buf_write1(buf, out[i]))
			return -1;
	}
	return 0;
}

// Map a MUS controller number to a MIDI controller number.
// Controller 0 is a program change and handled separately.
static const unsigned char mus_to_midi_ctrl[10] =
{
	0,		// 0: program change (special-cased)
	0,		// 1: bank select
	1,		// 2: modulation wheel
	7,		// 3: channel volume
	10,		// 4: pan
	11,		// 5: expression
	91,		// 6: reverb depth
	93,		// 7: chorus depth
	64,		// 8: sustain pedal
	67,		// 9: soft pedal
};

int mus2mid(const unsigned char* mus, int muslen,
            unsigned char** midi_out, int* midilen_out)
{
	int scorelen, scorestart;
	const unsigned char* p;
	const unsigned char* end;
	membuf_t buf;
	unsigned int delta = 0;
	int channel_map[16];		// MUS channel -> MIDI channel
	int channel_vol[16];		// last note volume per MUS channel
	int next_midi_channel = 0;
	int hit_end = 0;
	int i;

	if (!mus || muslen < 16 || memcmp(mus, "MUS\x1a", 4) != 0)
		return -1;

	scorelen   = mus[4] | (mus[5] << 8);
	scorestart = mus[6] | (mus[7] << 8);

	if (scorestart >= muslen)
		return -1;
	if (scorestart + scorelen > muslen)
		scorelen = muslen - scorestart;

	p = mus + scorestart;
	end = p + scorelen;

	for (i = 0; i < 16; i++)
	{
		channel_map[i] = -1;
		channel_vol[i] = 127;
	}
	// Percussion channel is fixed
	channel_map[MUS_PERCUSSION] = MIDI_PERCUSSION;

	memset(&buf, 0, sizeof(buf));

	// MIDI file header: format 0, 1 track, division 70
	{
		static const unsigned char header[] =
		{
			'M','T','h','d',
			0, 0, 0, 6,		// header length
			0, 0,			// format 0
			0, 1,			// one track
			0, 70,			// division: 70 ticks/quarter note
			'M','T','r','k',
			0, 0, 0, 0,		// track length (patched at the end)
		};
		if (buf_write(&buf, header, sizeof(header)))
			goto fail;
	}

	while (p < end && !hit_end)
	{
		unsigned char evbyte = *p++;
		int last = evbyte & 0x80;
		int type = (evbyte >> 4) & 7;
		int mus_chan = evbyte & 15;
		int midi_chan;
		unsigned char b1, b2;

		// Allocate a MIDI channel on first use (skip percussion)
		midi_chan = channel_map[mus_chan];
		if (midi_chan < 0)
		{
			if (next_midi_channel == MIDI_PERCUSSION)
				next_midi_channel++;
			midi_chan = next_midi_channel < 16 ? next_midi_channel++ : 15;
			channel_map[mus_chan] = midi_chan;
		}

		switch (type)
		{
		case MUS_EV_RELEASE_NOTE:
			if (p >= end) goto fail;
			b1 = *p++ & 0x7f;
			if (write_varlen(&buf, delta)) goto fail;
			delta = 0;
			if (buf_write1(&buf, 0x80 | midi_chan)) goto fail;
			if (buf_write1(&buf, b1)) goto fail;
			if (buf_write1(&buf, 0)) goto fail;
			break;

		case MUS_EV_PLAY_NOTE:
			if (p >= end) goto fail;
			b1 = *p++;
			if (b1 & 0x80)
			{
				if (p >= end) goto fail;
				channel_vol[mus_chan] = *p++ & 0x7f;
				b1 &= 0x7f;
			}
			if (write_varlen(&buf, delta)) goto fail;
			delta = 0;
			if (buf_write1(&buf, 0x90 | midi_chan)) goto fail;
			if (buf_write1(&buf, b1)) goto fail;
			if (buf_write1(&buf, (unsigned char)channel_vol[mus_chan])) goto fail;
			break;

		case MUS_EV_PITCH_WHEEL:
		{
			int bend;
			if (p >= end) goto fail;
			b1 = *p++;
			// MUS: 0-255, 128 = center. MIDI: 14-bit, 8192 = center.
			bend = b1 * 64;
			if (write_varlen(&buf, delta)) goto fail;
			delta = 0;
			if (buf_write1(&buf, 0xE0 | midi_chan)) goto fail;
			if (buf_write1(&buf, bend & 0x7f)) goto fail;
			if (buf_write1(&buf, (bend >> 7) & 0x7f)) goto fail;
			break;
		}

		case MUS_EV_SYSTEM:
		{
			unsigned char ctrl;
			if (p >= end) goto fail;
			b1 = *p++;
			switch (b1)
			{
			case 10: ctrl = 120; break;	// all sounds off
			case 11: ctrl = 123; break;	// all notes off
			case 12: ctrl = 126; break;	// mono
			case 13: ctrl = 127; break;	// poly
			case 14: ctrl = 121; break;	// reset all controllers
			default: ctrl = 0; break;
			}
			if (!ctrl)
				break;
			if (write_varlen(&buf, delta)) goto fail;
			delta = 0;
			if (buf_write1(&buf, 0xB0 | midi_chan)) goto fail;
			if (buf_write1(&buf, ctrl)) goto fail;
			if (buf_write1(&buf, 0)) goto fail;
			break;
		}

		case MUS_EV_CONTROLLER:
			if (p + 1 >= end) goto fail;
			b1 = *p++;
			b2 = *p++;
			if (b2 & 0x80)
				b2 = 0x7f;
			if (write_varlen(&buf, delta)) goto fail;
			delta = 0;
			if (b1 == 0)
			{
				// Program (instrument) change
				if (buf_write1(&buf, 0xC0 | midi_chan)) goto fail;
				if (buf_write1(&buf, b2 & 0x7f)) goto fail;
			}
			else if (b1 < 10)
			{
				if (buf_write1(&buf, 0xB0 | midi_chan)) goto fail;
				if (buf_write1(&buf, mus_to_midi_ctrl[b1])) goto fail;
				if (buf_write1(&buf, b2)) goto fail;
			}
			else
			{
				goto fail;
			}
			break;

		case MUS_EV_END_MEASURE:
			// No data, no MIDI equivalent
			break;

		case MUS_EV_SCORE_END:
			hit_end = 1;
			break;

		case MUS_EV_UNUSED:
			if (p >= end) goto fail;
			p++;
			break;
		}

		// Read the delay if this was the last event in the group
		if (last && !hit_end)
		{
			unsigned int time = 0;
			unsigned char b;
			do
			{
				if (p >= end) goto fail;
				b = *p++;
				time = (time << 7) | (b & 0x7f);
			} while (b & 0x80);
			delta += time;
		}
	}

	// End-of-track meta event
	if (write_varlen(&buf, delta)) goto fail;
	if (buf_write1(&buf, 0xFF)) goto fail;
	if (buf_write1(&buf, 0x2F)) goto fail;
	if (buf_write1(&buf, 0x00)) goto fail;

	// Patch the track length (track data starts at offset 22)
	{
		int tracklen = buf.len - 22;
		buf.data[18] = (tracklen >> 24) & 0xff;
		buf.data[19] = (tracklen >> 16) & 0xff;
		buf.data[20] = (tracklen >> 8) & 0xff;
		buf.data[21] = tracklen & 0xff;
	}

	*midi_out = buf.data;
	*midilen_out = buf.len;
	return 0;

fail:
	free(buf.data);
	return -1;
}
