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
#include "../headers/graphics.h"
#include "../headers/system_logic.h"
#include "../headers/doubly_linked_rom_list.h"


void initializeGlobals() {
	running=1;
	currentSectionNumber=0;
	gamesInPage=0;
	CURRENT_SECTION.totalPages=0;
	MAX_GAMES_IN_SECTION=50000;
	favoritesSectionNumber=0;
	favoritesSize=0;
	currentCPU=OC_NO;
	favoritesSectionSelected=0;
	favoritesChanged=0;
	FULLSCREEN_ITEMS_PER_PAGE=12;
	MENU_ITEMS_PER_PAGE=10;
	ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
	isPicModeMenuHidden=1;
	autoHideLogos=1;
	stripGames=1;
	fontSize=14;
	srand(time(0));
	MAGIC_NUMBER = 315;
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
	#if defined(TARGET_NPG) || defined(TARGET_RG350)
	resetFrameBuffer();
	#endif
	createConfigFilesInHomeIfTheyDontExist();
	loadConfig();
	#if defined(TARGET_BITTBOY) || defined(TARGET_RG300) || defined(TARGET_RG350) || defined(TARGET_NPG)
	HW_Init();
	setCPU(OC_NO);
	#endif
	initializeGlobals();
	setupDisplayAndKeys();
	loadLastState();
	checkIfDefault();
	checkThemes();
	char *temp=malloc(8000);
	strcpy(temp,themes[activeTheme]);
	strcat(temp,"/theme.ini");
	loadTheme(temp);
	free(temp);
	int sectionCount=loadSections(sectionGroups[activeGroup].groupPath);
	loadFavorites();
//	if (argv[1]!=NULL) {
//		setSectionsState(argv[1]);
//		currentSectionNumber=atoi(argv[2]);
//		returnTo=atoi(argv[3]);
//		fullscreenMode=atoi(argv[4]);
//	}

	FULLSCREEN_ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE+(MENU_ITEMS_PER_PAGE*2/10);
	switch (MENU_ITEMS_PER_PAGE) {
	    case 16:
	    	fontSize=10;
	    	FULLSCREEN_ITEMS_PER_PAGE-=2;
	      break;
	    case 10:
	    	fontSize=14;
	        break;
	    default:
	    	fontSize=12;
//	    	FULLSCREEN_ITEMS_PER_PAGE-=1;
	}
	if(fullscreenMode==0) {
		ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
	} else {
		ITEMS_PER_PAGE=FULLSCREEN_ITEMS_PER_PAGE;
	}
	freeFonts();
	initializeFonts();
	#if defined(TARGET_BITTBOY) || defined(TARGET_RG300) || defined(TARGET_RG350)
	initSuspendTimer();
	#endif
	determineStartingScreen(sectionCount);
	while(strlen(CURRENT_SECTION.sectionName)<1) {
		advanceSection(0);
	}
	if (CURRENT_SECTION.currentGameNode!=NULL) {
		updateScreen(CURRENT_SECTION.currentGameNode->data);
	} else {
		updateScreen(NULL);
	}
	enableKeyRepeat();
	while (running) {
		while(pollEvent()){
			if(getEventType()==getKeyDown()){
				if (!isSuspended) {
					if (currentlyChoosing==0) {
						if (CURRENT_SECTION.currentGameNode!=NULL) {
							performAction(CURRENT_SECTION.currentGameNode->data);
						} else {
							performAction(NULL);
						}
					} else {
						if (currentlyChoosing==1) {
							performChoosingAction();
						} else if (currentlyChoosing==2) {
							performGroupChoosingAction();
						} else if (currentlyChoosing==3) {
							performSettingsChoosingAction();
						}
					}
				}
				#ifndef TARGET_PC
				resetScreenOffTimer();
				#endif
				if (CURRENT_SECTION.currentGameNode!=NULL) {
					updateScreen(CURRENT_SECTION.currentGameNode->data);
				} else {
					updateScreen(NULL);
				}
			} else if (getEventType()==getKeyUp()) {
				if(getPressedKey()==BTN_B&&!currentlyChoosing) {
					if (!currentlySectionSwitching&&!aKeyComboWasPressed&&currentSectionNumber!=favoritesSectionNumber&&sectionGroupCounter>1) {
						beforeTryingToSwitchGroup = activeGroup;
						currentlyChoosing=2;
					}
					hotKeyPressed=0;
					if(fullscreenMode) {
						if(currentlySectionSwitching) {
							hideFullScreenModeMenu();
						} else if (CURRENT_SECTION.alphabeticalPaging) {
							resetPicModeHideMenuTimer();
						}
					}
					CURRENT_SECTION.alphabeticalPaging=0;
					if (aKeyComboWasPressed) {
						currentlySectionSwitching=0;
					}
					if (CURRENT_SECTION.currentGameNode!=NULL) {
						updateScreen(CURRENT_SECTION.currentGameNode->data);
					} else {
						updateScreen(NULL);
					}
					aKeyComboWasPressed=0;
				}
				if(getPressedKey()==BTN_SELECT&&!hotKeyPressed) {
//					if (stripGames) {
//						stripGames=0;
//					} else {
//						stripGames=1;
//					}
//					currentlyChoosing=3;
					if (CURRENT_SECTION.currentGameNode!=NULL) {
						updateScreen(CURRENT_SECTION.currentGameNode->data);
					} else {
						updateScreen(NULL);
					}
				}
			}
		}
	}
//	PrintDoublyLinkedRomList(CURRENT_SECTION.head);
	quit();
}
