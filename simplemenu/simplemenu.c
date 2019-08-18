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
}

int main(int argc, char *argv[]) {
	HW_Init();
	initializeGlobals();
	loadConfig();
	loadLastState();
	loadFavorites();
	setupDisplay();
	setupDecorations();
	if(strcmp(CURRENT_SECTION.sectionName,"9999 IN 1")==0) {
		favoritesSectionSelected=1;
		loadFavoritesList();
	} else {
		loadGameList();
	}
	updateScreen();
	int shouldRepeat = 0;
	while (running) {
		if (shouldRepeat) {
			SDL_EnableKeyRepeat(50,180);
			shouldRepeat=0;
		}
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				shouldRepeat=performAction(event);
				updateScreen();
			}
			if (event.type == SDL_KEYUP) {
				break;
			}
		}
	}
	quit();
}
