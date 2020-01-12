#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
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
	currentCPU=OC_NO;
	favoritesSectionSelected=0;
	favoritesChanged=0;
//	pictureMode=0;
	isPicModeMenuHidden=1;
#ifndef TARGET_PC
#endif
	srand(time(0));
}

void resetFrameBuffer () {
	int ret = system("./scripts/reset_fb");
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
}

void sig_term_handler(int signum)
{
	running=0;
}


//void *checkClock(void *x_void_ptr) {
//	while (1) {
//		currRawtime = time (NULL);
//		currTime = localtime(&currRawtime);
//		if(currTime->tm_sec>lastSec) {
//			lastSec=currTime->tm_sec;
//			if(!isSuspended&&!leftOrRightPressed&&!isUSBMode) {
//				printf("iupdate last\n");
//				lastChargeLevel = getBatteryLevel();
//				updateScreen();
//			}
//		}
//		free (currTime);
//	}
//	printf("pepe\n");
//}

int main(int argc, char* argv[]) {
	//	testSectionLoad();
	//	exit(0);
	//	lastChargeLevel = getBatteryLevel();
	//	currRawtime = time(NULL);
	//	currTime = localtime(&currRawtime);
	//	lastSec=currTime->tm_sec;
	//	pthread_mutex_init(&lock, NULL);
	//	pthread_create(&clockThread, NULL, checkClock,NULL);
	signal(SIGTERM, &sig_term_handler);
	#ifdef TARGET_RG350
	resetFrameBuffer();
	#endif
	createConfigFilesInHomeIfTheyDontExist();
	loadConfig();
	#if defined(TARGET_BITTBOY) || defined(TARGET_RG300) || defined(TARGET_RG350)
	HW_Init();
	setCPU(OC_NO);
	#endif
	initializeGlobals();
	setupDisplayAndKeys();
	int sectionCount=loadSections();
//	int sectionCount=testSectionLoad();
//	int sectionCount=testSectionLoad2();
//	int sectionCount=1;
	loadFavorites();
	if (argv[1]!=NULL) {
		setSectionsState(argv[1]);
		currentSectionNumber=atoi(argv[2]);
		returnTo=atoi(argv[3]);
		pictureMode=atoi(argv[4]);
	} else {
		loadLastState();
	}
	#if defined(TARGET_BITTBOY) || defined(TARGET_RG300) || defined(TARGET_RG350)
	initSuspendTimer();
	#endif
//	initPicModeHideMenuTimer();
	determineStartingScreen(sectionCount);
	updateScreen();
	enableKeyRepeat();
	while (running) {
		while(pollEvent()){
			if(getEventType()==getKeyDown()){
				if (!isSuspended) {
					if (!currentlyChoosingEmulator) {
						performAction();
					} else {
						performChoosingAction();
					}
				}
				#ifndef TARGET_PC
				resetTimeoutTimer();
				#endif
				updateScreen();
			} else if (getEventType()==getKeyUp()) {
				if(getPressedKey()==BTN_B) {
					hotKeyPressed=0;
					if(pictureMode&&!currentlySectionSwitching&&CURRENT_SECTION.alphabeticalPaging==1) {
						resetPicModeHideMenuTimer();
					}
					currentlySectionSwitching=0;
//					hidePicModeMenu();
					updateScreen();
				}
				if(getPressedKey()==BTN_SELECT) {
					if (stripGames) {
						stripGames=0;
					} else {
						stripGames=1;
					}
					updateScreen();
				}
			}
		}
	}
	quit();
}
