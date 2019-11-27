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
	pictureMode=0;
	#ifndef TARGET_PC
	#endif
	srand(time(0));
}

void resetFrameBuffer () {
	system("./frontend_start");
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
//	lastChargeLevel = getBatteryLevel();
//	currRawtime = time(NULL);
//	currTime = localtime(&currRawtime);
//	lastSec=currTime->tm_sec;
//	pthread_mutex_init(&lock, NULL);
//	pthread_create(&clockThread, NULL, checkClock,NULL);
	signal(SIGTERM, &sig_term_handler);
	#ifndef TARGET_PC
	resetFrameBuffer();
	HW_Init();
	#endif
	createConfigFilesInHomeIfTheyDontExist();
	loadConfig();
	initializeGlobals();
	setupDisplayAndKeys();
	int sectionCount=loadSections();
	loadFavorites();
	if (argv[1]!=NULL) {
		setSectionsState(argv[1]);
		currentSectionNumber=atoi(argv[2]);
		returnTo=atoi(argv[3]);
		pictureMode=atoi(argv[4]);
	} else {
		loadLastState();
	}
	#ifndef TARGET_PC
	initSuspendTimer();
	#endif
	determineStartingScreen(sectionCount);
	updateScreen();
	enableKeyRepeat();
	while (running) {
		while(pollEvent()){
			if(getEventType()==getKeyDown()){
				if (!isSuspended) {
					performAction();
				}
				#ifndef TARGET_PC
				resetTimeoutTimer();
				#endif
				updateScreen();
			} else if (getEventType()==getKeyUp()) {
				if(getPressedKey()==BTN_B) {
					hotKeyPressed=0;
					leftOrRightPressed=0;
					updateScreen();
				}
			}
		}
	}
	quit();
}
