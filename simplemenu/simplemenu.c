#include <config.h>
#include <control.h>
#include <definitions.h>
#include <globals.h>
#include <logic.h>
#include <screen.h>
#include <stdlib.h>
#include <system_logic.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>

void initializeGlobals() {
	running=1;
	currentSectionNumber=0;
	gamesInPage=0;
	totalPages=0;
	favoritesSectionNumber=0;
	favoritesSize=0;
	currentCPU=MED_OC;
	favoritesSectionSelected=0;
	favoritesChanged=0;
}

int main(int argc, char* argv[]) {
	HW_Init();
	putenv("SDL_NOMOUSE=1");
	putenv("HOME=/mnt/");
	initializeGlobals();
	int sectionCount=loadConfig();
	loadFavorites();
	if (argv[1]!=NULL) {
		setSectionsState(argv[1]);
		currentSectionNumber=atoi(argv[2]);
	} else {
		loadLastState();
	}
	setupDisplay();
	setupDecorations();
	determineStartingScreen(sectionCount);
	updateScreen();
	int shouldRepeat = 0;
	while (running) {
		if (shouldRepeat) {
			SDL_EnableKeyRepeat(50,180);
			shouldRepeat=0;
		}
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				shouldRepeat=performAction();
				updateScreen();
			}
			if (event.type == SDL_KEYUP) {
				break;
			}
		}
	}
	quit();
}
