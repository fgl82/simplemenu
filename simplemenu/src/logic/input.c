#include <stddef.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>

#include "../headers/globals.h"

SDL_Event event;

int pollEvent() {
	return SDL_PollEvent(&event);
}

int getEventType() {
	return event.type;
}

int getPressedKey() {
	return event.key.keysym.sym;
}

int getKeyDown() {
	return SDL_KEYDOWN;
}

int getKeyUp() {
	return SDL_KEYUP;
}

void enableKeyRepeat() {
	SDL_EnableKeyRepeat(500,180);
}

void initializeKeys() {
	keys = SDL_GetKeyState(NULL);
}

void pumpEvents() {
	SDL_PumpEvents();
}

