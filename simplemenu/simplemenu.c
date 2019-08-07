#include <bits/types/FILE.h>
#include <control.h>
#include <globals.h>
#include <logic.h>
#include <screen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <unistd.h>

void initializeGlobals() {
	running=1;
	currentGame = 0;
	currentPage = 0;
	currentSection = 0;
	gamesInPage=0;
	totalPages=0;
}

int main(int argc, char *argv[]) {
	readConfig();
	initializeGlobals();
	setupDisplay();
	setupDecorations();
	loadGameList();
	updateScreen();
	while (running) {
		SDL_EnableKeyRepeat(30,180);
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				performAction(event);
				updateScreen();
			}
			if (event.type == SDL_KEYUP) {
				break;
			}
		}
	}
	quit();
}
