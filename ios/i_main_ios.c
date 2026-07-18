// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// DESCRIPTION:
//	iOS main program. SDL2 on iOS requires SDL_main (UIApplicationMain
//	is started by SDL2main, which then calls this function).
//	Sets the working directory to the app bundle so DOOM.WAD is found,
//	then calls D_DoomMain.
//
//-----------------------------------------------------------------------------

#include <SDL.h>
#include <SDL_main.h>

#include <unistd.h>
#include <stdlib.h>

#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"

int
main
( int		argc,
  char**	argv )
{
    static char* args[] = { "doom" };

    (void)argc;
    (void)argv;

    // Point DOOM at the app bundle resource directory so the WAD is found
    char* base = SDL_GetBasePath();
    if (base)
    {
	printf("iOS base path: %s\n", base);
	setenv("DOOMWADDIR", base, 1);
	if (chdir(base) != 0)
	    printf("chdir failed\n");
	SDL_free(base);
    }
    else
    {
	printf("SDL_GetBasePath failed: %s\n", SDL_GetError());
    }

    myargc = sizeof(args) / sizeof(args[0]);
    myargv = args;

    D_DoomMain();

    return 0;
}
