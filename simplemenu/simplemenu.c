#include <control.h>
#include <globals.h>
#include <logic.h>
#include <screen.h>
#include <system_logic.h>
#include <definitions.h>
#include <config.h>
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

void determineStartingScreen(int sectionCount) {
	if(sectionCount==0||currentSectionNumber==favoritesSectionNumber) {
		favoritesSectionSelected=1;
		loadFavoritesList();
	} else {
		loadGameList();
	}
}

int main() {
	HW_Init();
	initializeGlobals();
	int sectionCount=loadConfig();
	loadLastState();
	loadFavorites();
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
