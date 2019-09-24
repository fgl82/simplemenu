#include <stdlib.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>

#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/globals.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/system_logic.h"
#include "../headers/definitions.h"



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
	pictureMode=0;
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
		returnTo=atoi(argv[3]);
		pictureMode=atoi(argv[4]);
	} else {
		loadLastState();
	}
	setupDisplay();
	determineStartingScreen(sectionCount);
	updateScreen();
	SDL_EnableKeyRepeat(500,180);
	while (running) {
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_KEYDOWN:
				performAction();
				updateScreen();
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym==BTN_A) {
					hotKeyPressed=0;
					updateScreen();
				}
				break;
			default:
				break;
			}
		}
	}
	quit();
}
