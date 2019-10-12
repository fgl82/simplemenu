#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/input.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/system_logic.h"

void initializeGlobals() {
	running=1;
	currentSectionNumber=0;
	gamesInPage=0;
	CURRENT_SECTION.totalPages=0;
	favoritesSectionNumber=0;
	favoritesSize=0;
	currentCPU=MED_OC;
	favoritesSectionSelected=0;
	favoritesChanged=0;
	pictureMode=0;
}

int main(int argc, char* argv[]) {
	HW_Init();
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
	enableKeyRepeat(500.180);
	while (running) {
		while(pollEvent()){
			if(getEventType()==getKeyDown()){
				performAction();
				updateScreen();
			} else if (getEventType()==getKeyUp()) {
				if(getPressedKey()==BTN_TA) {
					hotKeyPressed=0;
					leftOrRightPressed=0;
					updateScreen();
				}
			}
		}
	}
	quit();
}
