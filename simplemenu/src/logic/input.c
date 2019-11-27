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

int isLeftOrRight() {
	return event.jaxis.axis == 0;
}

int isUp() {
	const int JOYSTICK_DEAD_ZONE = 0;
	return (event.jaxis.axis == 1&&event.jaxis.value < JOYSTICK_DEAD_ZONE);
	//return (event.jhat.value & SDL_HAT_UP);
}

int isDown() {
	const int JOYSTICK_DEAD_ZONE = 0;
	return (event.jaxis.axis == 1&&event.jaxis.value > JOYSTICK_DEAD_ZONE);
	//return (event.jhat.value & SDL_HAT_UP);
}



int getPressedKey() {
	return event.key.keysym.sym;
}

int getJoystickPressedDirection() {
	return event.jhat.value;
}

int getKeyDown() {
	return SDL_KEYDOWN;
}

int getKeyUp() {
	return SDL_KEYUP;
}

int getJoystickMotion() {
	return SDL_JOYAXISMOTION;
}

void enableKeyRepeat() {
	SDL_EnableKeyRepeat(500,180);
}

void initializeKeys() {
	keys = SDL_GetKeyState(NULL);
//	SDL_JoystickEventState(SDL_ENABLE);
//	for (int i = 0; i < SDL_NumJoysticks(); i++)
//	{
//		if (strcmp(SDL_JoystickName(i), "linkdev device (Analog 2-axis 8-button 2-hat)") == 0)
//			joystick = SDL_JoystickOpen(i);
//	}
}

void pumpEvents() {
	SDL_PumpEvents();
}

