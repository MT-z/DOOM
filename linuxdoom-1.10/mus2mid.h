// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//	MUS (DMX music format) to Standard MIDI File converter.
//
//-----------------------------------------------------------------------------

#ifndef __MUS2MID_H__
#define __MUS2MID_H__

// Convert a MUS lump held in memory into a format-0 Standard MIDI File.
//
// mus / muslen:      input MUS data and its length in bytes
// midi_out:          receives a malloc()ed buffer with the MIDI data
//                    (caller must free())
// midilen_out:       receives the length of the MIDI data
//
// Returns 0 on success, nonzero on failure.
int mus2mid(const unsigned char* mus, int muslen,
            unsigned char** midi_out, int* midilen_out);

#endif // __MUS2MID_H__
