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
static Uint32			sdl_palette[256];

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

// Gamepad (SDL GameController: Xbox / PS / etc. unified layout)
static SDL_GameController*	gamepad = NULL;
#define GAMEPAD_DEADZONE	4000

// Analog axes consumed by G_BuildTiccmd (g_game.c)
extern int joysidemove;		// left stick X: strafe
extern int joy_weaponchange;	// pending weapon index from L1/R1 (-1 = none)

// From doomstat/g_game: current weapon for L1/R1 cycling
#include "d_player.h"
extern player_t players[MAXPLAYERS];
extern int consoleplayer;
extern boolean usergame;

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
// I_OpenGamepad
// Open the first available game controller
//
static void I_OpenGamepad(void)
{
	int i;

	if (gamepad)
		return;

	for (i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i))
		{
			gamepad = SDL_GameControllerOpen(i);
			if (gamepad)
			{
				printf("Gamepad connected: %s\n",
					   SDL_GameControllerName(gamepad));
				break;
			}
		}
	}
}

//
// I_ApplyDeadzone
//
static int I_ApplyDeadzone(int value)
{
	if (value > -GAMEPAD_DEADZONE && value < GAMEPAD_DEADZONE)
		return 0;
	return value;
}

//
// I_CycleWeapon
// Select previous/next owned weapon relative to the current one
//
static void I_CycleWeapon(int dir)
{
	player_t* p;
	int w;
	int i;

	if (!usergame)
		return;

	p = &players[consoleplayer];
	w = p->readyweapon;

	for (i = 0; i < NUMWEAPONS; i++)
	{
		w = (w + dir + NUMWEAPONS) % NUMWEAPONS;
		// BT_WEAPONMASK is 3 bits: only weapons 0-7 selectable
		if (w <= 7 && p->weaponowned[w])
		{
			joy_weaponchange = w;
			return;
		}
	}
}

//
// I_PollGamepad
// Poll analog axes and post button state each frame
//
static void I_PollGamepad(void)
{
	event_t doom_event;
	int buttons;
	int turn;
	int fwd;
	int side;

	if (!gamepad)
		return;

	// Axes
	turn = I_ApplyDeadzone(
		SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_RIGHTX));
	fwd = I_ApplyDeadzone(
		SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_LEFTY));
	side = I_ApplyDeadzone(
		SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_LEFTX));

	// Strafe axis is consumed directly by G_BuildTiccmd
	joysidemove = side;

	// Buttons -> joybutton bits matching .doomrc defaults:
	// bit0 = fire (joyb_fire 0), bit2 = speed/run (joyb_speed 2),
	// bit3 = use (joyb_use 3)
	buttons = 0;

	// R2: fire
	if (SDL_GameControllerGetAxis(gamepad,
			SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 8000)
		buttons |= 1;

	// L2: run
	if (SDL_GameControllerGetAxis(gamepad,
			SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 8000)
		buttons |= 4;

	// A (Xbox) / Cross (PS): use/open
	if (SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_A))
		buttons |= 8;

	doom_event.type = ev_joystick;
	doom_event.data1 = buttons;
	doom_event.data2 = turn;   // right stick X: turn
	doom_event.data3 = fwd;    // left stick Y: forward/back
	D_PostEvent(&doom_event);
}

//
// I_GamepadButtonKey
// Map gamepad buttons to DOOM keys (menus, weapon cycling)
//
static int I_GamepadButtonKey(Uint8 button)
{
	switch (button)
	{
	case SDL_CONTROLLER_BUTTON_START:		return KEY_ESCAPE;
	case SDL_CONTROLLER_BUTTON_A:			return KEY_ENTER;
	case SDL_CONTROLLER_BUTTON_B:			return KEY_BACKSPACE;
	case SDL_CONTROLLER_BUTTON_DPAD_UP:		return KEY_UPARROW;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:	return KEY_DOWNARROW;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:	return KEY_LEFTARROW;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:	return KEY_RIGHTARROW;
	default:								return 0;
	}
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

		case SDL_CONTROLLERDEVICEADDED:
			I_OpenGamepad();
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			if (gamepad && event.cdevice.which ==
				SDL_JoystickInstanceID(
					SDL_GameControllerGetJoystick(gamepad)))
			{
				SDL_GameControllerClose(gamepad);
				gamepad = NULL;
				printf("Gamepad disconnected\n");
			}
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			// L1/R1: weapon cycling (gameplay)
			if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
				I_CycleWeapon(-1);
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
				I_CycleWeapon(1);
			else
			{
				// Menu navigation keys
				int key = I_GamepadButtonKey(event.cbutton.button);
				if (key)
				{
					doom_event.type = ev_keydown;
					doom_event.data1 = key;
					D_PostEvent(&doom_event);
				}
			}
			break;

		case SDL_CONTROLLERBUTTONUP:
			{
				int key = I_GamepadButtonKey(event.cbutton.button);
				if (key)
				{
					doom_event.type = ev_keyup;
					doom_event.data1 = key;
					D_PostEvent(&doom_event);
				}
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

	// Poll gamepad analog state
	I_PollGamepad();
}

//
// I_UpdateGraphics
// Update the display with the current framebuffer
//
void I_UpdateGraphics(void)
{
	int i;
	byte* src;

	if (!sdl_window || !sdl_renderer || !sdl_texture)
		return;

	// Allocate 32-bit conversion buffer on first use
	if (!sdl_pixels)
	{
		sdl_pixels = malloc(SCREENWIDTH * SCREENHEIGHT * sizeof(Uint32));
		if (!sdl_pixels)
			I_Error("I_UpdateGraphics: couldn't allocate pixel buffer");
	}

	// Convert 8-bit palettized framebuffer to 32-bit ARGB
	src = screens[0];
	for (i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++)
		sdl_pixels[i] = sdl_palette[src[i]];

	// Copy framebuffer to texture (pitch is in BYTES per row: 320 * 4)
	SDL_UpdateTexture(sdl_texture, NULL, sdl_pixels, SCREENWIDTH * sizeof(Uint32));

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

	printf("I_InitGraphics: Starting SDL2 initialization\n");
	fflush(stdout);

	// Initialize SDL2 (video + game controller support)
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
		printf("I_InitGraphics: SDL_Init failed: %s\n", SDL_GetError());
		fflush(stdout);
		I_Error("SDL_Init failed: %s", SDL_GetError());
	}

	// Open gamepad if one is already connected
	I_OpenGamepad();

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

	if (gamepad)
	{
		SDL_GameControllerClose(gamepad);
		gamepad = NULL;
	}

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
	// Convert DOOM's 256-color RGB palette (768 bytes) to ARGB8888
	int i;

	for (i = 0; i < 256; i++)
	{
		Uint32 r = palette[i*3 + 0];
		Uint32 g = palette[i*3 + 1];
		Uint32 b = palette[i*3 + 2];
		sdl_palette[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
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
