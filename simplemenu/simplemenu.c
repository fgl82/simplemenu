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

int main() {
	printf("1\n");
	HW_Init();
	printf("2\n");
	initializeGlobals();
	printf("3\n");
	loadConfig();
	printf("4\n");
	loadLastState();
	printf("5\n");
	loadFavorites();
	printf("6\n");
	setupDisplay();
	printf("7\n");
	setupDecorations();
	printf("8\n");
	if(currentSectionNumber==lastSection) {
		printf("8.1\n");
		favoritesSectionSelected=1;
		loadFavoritesList();
	} else {
		printf("8.2\n");
		loadGameList();
	}
	printf("9\n");
	updateScreen();
	printf("10\n");
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
