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
//	DOOM graphics stuff for SDL2, supporting macOS and other platforms.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_video_sdl.c,v 1.0 2026/07/18 macos port $";

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include "SDL2/SDL.h"

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"
#include "doomdef.h"

// SDL2 window and renderer globals
static SDL_Window*		sdl_window = NULL;
static SDL_Renderer*	sdl_renderer = NULL;
static SDL_Texture*		sdl_texture = NULL;
static SDL_Surface*		sdl_surface = NULL;
static Uint32*			sdl_pixels = NULL;

// Video globals
static int				sdl_width = SCREENWIDTH;
static int				sdl_height = SCREENHEIGHT;
static boolean			sdl_fullscreen = false;
static int				multiply = 1;

// Input handling
static int				lastmousex = 0;
static int				lastmousey = 0;
static boolean			mousemoved = false;
static ticcmd_t			emptycmd;

//
// I_TranslateKey
// Translates SDL2 keycodes to DOOM keycodes
//
static int I_TranslateKey(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_LEFT:			return KEY_LEFTARROW;
	case SDLK_RIGHT:		return KEY_RIGHTARROW;
	case SDLK_DOWN:			return KEY_DOWNARROW;
	case SDLK_UP:			return KEY_UPARROW;
	case SDLK_ESCAPE:		return KEY_ESCAPE;
	case SDLK_RETURN:		return KEY_ENTER;
	case SDLK_TAB:			return KEY_TAB;
	case SDLK_F1:			return KEY_F1;
	case SDLK_F2:			return KEY_F2;
	case SDLK_F3:			return KEY_F3;
	case SDLK_F4:			return KEY_F4;
	case SDLK_F5:			return KEY_F5;
	case SDLK_F6:			return KEY_F6;
	case SDLK_F7:			return KEY_F7;
	case SDLK_F8:			return KEY_F8;
	case SDLK_F9:			return KEY_F9;
	case SDLK_F10:			return KEY_F10;
	case SDLK_F11:			return KEY_F11;
	case SDLK_F12:			return KEY_F12;
	case SDLK_BACKSPACE:	return KEY_BACKSPACE;
	case SDLK_DELETE:		return KEY_BACKSPACE;
	case SDLK_PAUSE:		return KEY_PAUSE;
	case SDLK_EQUALS:		return KEY_EQUALS;
	case SDLK_MINUS:		return KEY_MINUS;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:		return KEY_RSHIFT;
	case SDLK_LCTRL:
	case SDLK_RCTRL:		return KEY_RCTRL;
	case SDLK_LALT:
	case SDLK_RALT:			return KEY_RALT;
	default:
		// Regular printable characters
		if (key >= ' ' && key <= '~')
		{
			// Convert to lowercase
			if (key >= 'A' && key <= 'Z')
				return key - 'A' + 'a';
			return key;
		}
		break;
	}
	return 0;
}

//
// I_GetEvent
// Handle SDL2 events
//
void I_GetEvent(void)
{
	SDL_Event event;
	event_t doom_event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			// Window close button
			I_Quit();
			break;

		case SDL_KEYDOWN:
			{
				int key = I_TranslateKey(event.key.keysym.sym);
				if (key)
				{
					doom_event.type = ev_keydown;
					doom_event.data1 = key;
					D_PostEvent(&doom_event);
				}
			}
			break;

		case SDL_KEYUP:
			{
				int key = I_TranslateKey(event.key.keysym.sym);
				if (key)
				{
					doom_event.type = ev_keyup;
					doom_event.data1 = key;
					D_PostEvent(&doom_event);
				}
			}
			break;

		case SDL_MOUSEMOTION:
			{
				int dx = event.motion.xrel;
				int dy = event.motion.yrel;

				if (dx || dy)
				{
					doom_event.type = ev_mouse;
					doom_event.data1 = 0;  // button state set elsewhere
					doom_event.data2 = dx * 2;
					doom_event.data3 = -dy * 2;
					D_PostEvent(&doom_event);
					mousemoved = true;
				}
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			{
				// Mouse buttons are represented as bit flags in ev_mouse events
				// Bit 0 = Button 1 (left), Bit 1 = Button 2 (right), Bit 2 = Button 3 (middle)
				int button_bit = 0;
				
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					button_bit = 1;
					break;
				case SDL_BUTTON_RIGHT:
					button_bit = 2;
					break;
				case SDL_BUTTON_MIDDLE:
					button_bit = 4;
					break;
				default:
					continue;
				}

				doom_event.type = ev_mouse;
				doom_event.data1 = button_bit;
				doom_event.data2 = 0;
				doom_event.data3 = 0;
				D_PostEvent(&doom_event);
			}
			break;
		}
	}
}

//
// I_StartFrame
//
void I_StartFrame(void)
{
	// Update display
	I_GetEvent();
}

//
// I_UpdateGraphics
// Update the display with the current framebuffer
//
void I_UpdateGraphics(void)
{
	if (!sdl_window || !sdl_renderer || !sdl_texture)
		return;

	// Copy framebuffer to texture
	SDL_UpdateTexture(sdl_texture, NULL, screens[0], SCREENWIDTH);

	// Clear and render
	SDL_RenderClear(sdl_renderer);
	SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);
	SDL_RenderPresent(sdl_renderer);
}

//
// I_InitGraphics
// Initialize SDL2 graphics subsystem
//
void I_InitGraphics(void)
{
	char* displayenv;
	int windowflags;

	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		I_Error("SDL_Init failed: %s", SDL_GetError());

	windowflags = 0;

	// Check for fullscreen mode
	if (M_CheckParm("-fullscreen"))
		windowflags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	// Check for window scaling
	if (M_CheckParm("-2x"))
		multiply = 2;
	else if (M_CheckParm("-3x"))
		multiply = 3;

	// Calculate window size
	int window_width = SCREENWIDTH * multiply;
	int window_height = SCREENHEIGHT * multiply;

	// Create window
	sdl_window = SDL_CreateWindow(
		"DOOM",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		windowflags | SDL_WINDOW_SHOWN
	);

	if (!sdl_window)
		I_Error("SDL_CreateWindow failed: %s", SDL_GetError());

	// Create renderer
	sdl_renderer = SDL_CreateRenderer(
		sdl_window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!sdl_renderer)
		I_Error("SDL_CreateRenderer failed: %s", SDL_GetError());

	// Set renderer scale
	if (multiply > 1)
	{
		SDL_RenderSetScale(sdl_renderer, (float)multiply, (float)multiply);
	}

	// Create texture for framebuffer
	sdl_texture = SDL_CreateTexture(
		sdl_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREENWIDTH,
		SCREENHEIGHT
	);

	if (!sdl_texture)
		I_Error("SDL_CreateTexture failed: %s", SDL_GetError());

	// Create surface for palette conversion
	sdl_surface = SDL_CreateRGBSurface(
		0,
		SCREENWIDTH,
		SCREENHEIGHT,
		8,
		0, 0, 0, 0
	);

	if (!sdl_surface)
		I_Error("SDL_CreateRGBSurface failed: %s", SDL_GetError());

	// Enable relative mouse mode for better gameplay
	SDL_SetRelativeMouseMode(SDL_TRUE);

	printf("SDL2 Graphics initialized: %dx%d (scale: %dx)\n",
		   window_width, window_height, multiply);
}

//
// I_ShutdownGraphics
//
void I_ShutdownGraphics(void)
{
	SDL_SetRelativeMouseMode(SDL_FALSE);

	if (sdl_texture)
	{
		SDL_DestroyTexture(sdl_texture);
		sdl_texture = NULL;
	}

	if (sdl_renderer)
	{
		SDL_DestroyRenderer(sdl_renderer);
		sdl_renderer = NULL;
	}

	if (sdl_surface)
	{
		SDL_FreeSurface(sdl_surface);
		sdl_surface = NULL;
	}

	if (sdl_window)
	{
		SDL_DestroyWindow(sdl_window);
		sdl_window = NULL;
	}

	SDL_Quit();
}

//
// I_ReadScreen
// Read a portion of the screen into a buffer
//
void I_ReadScreen(byte* scr)
{
	memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

//
// STUB FUNCTIONS
//

void I_SetPalette(byte *palette)
{
	// In a real implementation, this would set the color palette
	// For SDL2 with 32-bit graphics, palette handling would be more complex
	// For now, we'll just ignore it since we're using 24-bit graphics
}

void I_StartTic(void)
{
	// Called each tic to handle input
	// We handle input continuously in I_GetEvent
}

void I_UpdateNoBlit(void)
{
	// Update without blitting - skip for now
}

void I_ShutdownMusic(void)
{
	// Shutdown music - stub for now
}
