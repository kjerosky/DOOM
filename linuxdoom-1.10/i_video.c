// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
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
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <errno.h>
#include <signal.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

#include <SDL3/SDL.h>
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* screen_texture = NULL;
Uint32 sdl_palette[256];

void sdl_cleanup() {
	if (screen_texture != NULL) {
		SDL_DestroyTexture(screen_texture);
	}

	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
	}

	if (window != NULL) {
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
}

void cleanup_and_quit() {
	sdl_cleanup();
	I_Quit();
}

int translate_key_scancode(SDL_Scancode scancode) {
	int translated = KEY_ESCAPE;

	switch (scancode) {
		case SDL_SCANCODE_LEFT:	translated = KEY_LEFTARROW;	break;
		case SDL_SCANCODE_RIGHT:	translated = KEY_RIGHTARROW;	break;
		case SDL_SCANCODE_DOWN:	translated = KEY_DOWNARROW;	break;
		case SDL_SCANCODE_UP:	translated = KEY_UPARROW;	break;
		case SDL_SCANCODE_ESCAPE:	translated = KEY_ESCAPE;	break;
		case SDL_SCANCODE_RETURN:	translated = KEY_ENTER;		break;
		case SDL_SCANCODE_TAB:	translated = KEY_TAB;		break;
		case SDL_SCANCODE_F1:	translated = KEY_F1;		break;
		case SDL_SCANCODE_F2:	translated = KEY_F2;		break;
		case SDL_SCANCODE_F3:	translated = KEY_F3;		break;
		case SDL_SCANCODE_F4:	translated = KEY_F4;		break;
		case SDL_SCANCODE_F5:	translated = KEY_F5;		break;
		case SDL_SCANCODE_F6:	translated = KEY_F6;		break;
		case SDL_SCANCODE_F7:	translated = KEY_F7;		break;
		case SDL_SCANCODE_F8:	translated = KEY_F8;		break;
		case SDL_SCANCODE_F9:	translated = KEY_F9;		break;
		case SDL_SCANCODE_F10:	translated = KEY_F10;		break;
		case SDL_SCANCODE_F11:	translated = KEY_F11;		break;
		case SDL_SCANCODE_F12:	translated = KEY_F12;		break;

		case SDL_SCANCODE_BACKSPACE:
		case SDL_SCANCODE_DELETE:
			translated = KEY_BACKSPACE;
			break;

		case SDL_SCANCODE_PAUSE:	translated = KEY_PAUSE;		break;

		case SDL_SCANCODE_KP_EQUALS:
		case SDL_SCANCODE_EQUALS:	translated = KEY_EQUALS;	break;

		case SDL_SCANCODE_KP_MINUS:
		case SDL_SCANCODE_MINUS:	translated = KEY_MINUS;		break;

		case SDL_SCANCODE_LSHIFT:
		case SDL_SCANCODE_RSHIFT:
			translated = KEY_RSHIFT;
			break;

		case SDL_SCANCODE_LCTRL:
		case SDL_SCANCODE_RCTRL:
			translated = KEY_RCTRL;
			break;

		case SDL_SCANCODE_LALT:
		case SDL_SCANCODE_RALT:
			translated = KEY_RALT;
			break;

		default:
			translated = SDL_GetKeyFromScancode(scancode, SDL_KMOD_NONE, false);
			if (translated >= 'A' && translated <= 'Z') {
				translated = translated - 'A' + 'a';
			}
			break;
	}

	return translated;
}

void I_ShutdownGraphics(void)
{
  sdl_cleanup();
}

int determine_mouse_buttons_state(SDL_MouseButtonFlags flags) {
	return
		((flags & SDL_BUTTON_LMASK) ? 1 : 0) |
		((flags & SDL_BUTTON_MMASK) ? 2 : 0) |
		((flags & SDL_BUTTON_RMASK) ? 4 : 0)
	;
}

void I_GetEvent(void)
{
	event_t event;

	float mouse_x;
	float mouse_y;

	SDL_Event sdl_event;
	while (SDL_PollEvent(&sdl_event)) {
		switch (sdl_event.type) {
			case SDL_EVENT_QUIT:
				cleanup_and_quit();
				break;

			case SDL_EVENT_KEY_DOWN:
				event.type = ev_keydown;
				event.data1 = translate_key_scancode(sdl_event.key.scancode);
				D_PostEvent(&event);
				break;

			case SDL_EVENT_KEY_UP:
				event.type = ev_keyup;
				event.data1 = translate_key_scancode(sdl_event.key.scancode);
				D_PostEvent(&event);
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				event.type = ev_mouse;
				event.data1 = determine_mouse_buttons_state(SDL_GetMouseState(&mouse_x, &mouse_y));
				event.data2 = 0;
				event.data3 = 0;
				D_PostEvent(&event);
				break;

			case SDL_EVENT_MOUSE_BUTTON_UP:
				event.type = ev_mouse;
				event.data1 = determine_mouse_buttons_state(SDL_GetMouseState(&mouse_x, &mouse_y));
				event.data2 = 0;
				event.data3 = 0;
				D_PostEvent(&event);
				break;

			case SDL_EVENT_MOUSE_MOTION:
				event.type = ev_mouse;
				event.data1 = determine_mouse_buttons_state(sdl_event.motion.state);
				event.data2 = sdl_event.motion.xrel;
				event.data3 = -sdl_event.motion.yrel;
				D_PostEvent(&event);
				break;
		}
	}
}

//
// I_StartTic
//
void I_StartTic (void)
{
	I_GetEvent();
}


//
// I_FinishUpdate
//
void I_FinishUpdate (void)
{

    static int	lasttic;
    int		tics;
    int		i;
    // UNUSED static unsigned char *bigscreen=0;

    // draws little dots on the bottom of the screen
    if (devparm)
    {

	i = I_GetTime();
	tics = i - lasttic;
	lasttic = i;
	if (tics > 20) tics = 20;

	for (i=0 ; i<tics*2 ; i+=2)
	    screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
	for ( ; i<20*2 ; i+=2)
	    screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;
    
    }

	// Finish up processing input events.
	I_GetEvent();

	// --- SDL display ---

	Uint32* texture_pixels;
	int texture_pixels_row_length;
	SDL_LockTexture(screen_texture, NULL, (void**)&texture_pixels, &texture_pixels_row_length);
	for (int i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++) {
		texture_pixels[i] = sdl_palette[screens[0][i]];
	}
	SDL_UnlockTexture(screen_texture);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderTexture(renderer, screen_texture, NULL, NULL);

	SDL_RenderPresent(renderer);
}


//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}


void UploadNewPalette(byte *palette)
{

    register int	i;
    register int	c;

	const SDL_PixelFormatDetails *pixel_format_details = SDL_GetPixelFormatDetails(screen_texture->format);
	for (i = 0; i < 256; i++) {
		c = gammatable[usegamma][*palette++];
		Uint8 red = (c << 8) + c;
		c = gammatable[usegamma][*palette++];
		Uint8 green = (c << 8) + c;
		c = gammatable[usegamma][*palette++];
		Uint8 blue = (c << 8) + c;

		sdl_palette[i] = SDL_MapRGB(pixel_format_details, NULL, red, green, blue);
	}
}

//
// I_SetPalette
//
void I_SetPalette (byte* palette)
{
    UploadNewPalette(palette);
}

void I_InitGraphics(void)
{
    signal(SIGINT, (void (*)(int)) cleanup_and_quit);

	screens[0] = (unsigned char *) malloc (SCREENWIDTH * SCREENHEIGHT);

	// --- SDL setup ---

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "[ERROR] SDL_Init error: %s\n", SDL_GetError());
		exit(-2);
	}

	window = SDL_CreateWindow("DOOM", SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		fprintf(stderr, "[ERROR] SDL_CreateWindow error: %s\n", SDL_GetError());
		sdl_cleanup();
		exit(-2);
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		fprintf(stderr, "[ERROR] SDL_CreateRenderer error: %s\n", SDL_GetError());
		sdl_cleanup();
		exit(-2);
	}

	screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREENWIDTH, SCREENHEIGHT);
	if (screen_texture == NULL) {
		fprintf(stderr, "[ERROR] SDL_CreateTexture error: %s\n", SDL_GetError());
		sdl_cleanup();
		exit(-2);
	}

	SDL_SetTextureScaleMode(screen_texture, SDL_SCALEMODE_NEAREST);

	const SDL_PixelFormatDetails* pixel_format_details = SDL_GetPixelFormatDetails(screen_texture->format);
	for (int i = 0; i < 256; i++) {
		sdl_palette[i] = SDL_MapRGB(pixel_format_details, NULL, 0, 0, 0);
	}
}
