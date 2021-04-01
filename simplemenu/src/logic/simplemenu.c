#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <dirent.h>

#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/input.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/string_utils.h"
#include "../headers/system_logic.h"
#include "../headers/utils.h"


void initializeGlobals() {
	running=1;
	currentSectionNumber=0;
	gamesInPage=0;
	CURRENT_SECTION.totalPages=0;
	MAX_GAMES_IN_SECTION=500000;
	favoritesSectionNumber=0;
	favoritesSize=0;
	favoritesSectionSelected=0;
	favoritesChanged=0;
	FULLSCREEN_ITEMS_PER_PAGE=12;
	MENU_ITEMS_PER_PAGE=10;
	ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
	isPicModeMenuHidden=1;
	footerVisibleInFullscreenMode=1;
	menuVisibleInFullscreenMode=1;
	autoHideLogos=0;
	stripGames=1;
	srand(time(0));
}

void resetFrameBuffer () {
	int ret = system("./scripts/reset_fb");
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
}

void critical_error_handler()
{
	logMessage("ERROR","Nice, a critical error!!!");
	closeLogFile();
	exit(0);
}

void sig_term_handler()
{
	logMessage("WARN","Received SIGTERM");
	running=0;
}

int main() {
	initializeGlobals();
	logMessage("INFO","Initialized Globals");
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = critical_error_handler;
	sa.sa_flags   = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGABRT, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGTERM, &sig_term_handler);
#if defined(TARGET_NPG) || defined(TARGET_OD) || defined TARGET_OD_BETA
	resetFrameBuffer();
	logMessage("INFO","Reset Framebuffer");
#endif
	createConfigFilesInHomeIfTheyDontExist();
	loadConfig();
	logMessage("INFO","Config loaded");
	initializeDisplay();
	int color[3] = {255,255,255} ;
	//	drawRectangleToScreen(SCREEN_WIDTH,SCREEN_HEIGHT,0 , 0, color);

	//	char *akashi = "resources/akashi.ttf";
	//	TTF_Font *fonti = TTF_OpenFont(akashi, calculateProportionalSizeOrDistance(14+18));
	//	drawTextOnScreen(fonti, NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "HELLO", color, VAlignMiddle|HAlignCenter);
	//	refreshScreen();

	logMessage("INFO","Validated configuration existence");
	checkThemes();
	logMessage("INFO","Themes checked");
	loadLastState();
	logMessage("INFO","Last state loaded");
	logMessage("INFO","Initialized Display");
	char temp[300];
#if defined(TARGET_BITTBOY) || defined(TARGET_RFW) || defined(TARGET_OD) || defined(TARGET_OD_BETA) || defined(TARGET_NPG) || defined(TARGET_PC)
	HW_Init();
	logMessage("INFO","HW Initialized");
	currentCPU = OC_NO;
	setCPU(OC_NO);
	snprintf(temp,sizeof(temp),"CPU speed set: %d",currentCPU);
	logMessage("INFO",temp);
#endif
	setupDisplayAndKeys();
	logMessage("INFO","Display and input successfully configured");
	checkIfDefault();
	logMessage("INFO","Default state checked");
	strcpy(temp,themes[activeTheme]);
	strcat(temp,"/theme.ini");
	logMessage("INFO","Loading theme");
	loadTheme(temp);
	logMessage("INFO","Loading section groups");
	loadSectionGroups();
	logMessage("INFO","Loading sections");
	int sectionCount=loadSections(sectionGroups[activeGroup].groupPath);
	logMessage("INFO","Loading favorites");
	loadFavorites();
	currentMode=3;
	MENU_ITEMS_PER_PAGE=itemsPerPage;
	FULLSCREEN_ITEMS_PER_PAGE=itemsPerPageFullscreen;
	if(fullscreenMode==0) {
		ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
	} else {
		ITEMS_PER_PAGE=FULLSCREEN_ITEMS_PER_PAGE;
	}
	freeFonts();
	initializeFonts();
	initializeSettingsFonts();
	logMessage("INFO","Fonts initialized");
#if defined(TARGET_BITTBOY) || defined(TARGET_RFW) || defined(TARGET_OD) || defined(TARGET_OD_BETA) || defined(TARGET_NPG)
	initSuspendTimer();
	logMessage("INFO","Suspend timer initialized");
#endif
	logMessage("INFO","Determining starting screen");
	determineStartingScreen(sectionCount);
	while(strlen(CURRENT_SECTION.sectionName)<1) {
		advanceSection(0);
	}
	if (CURRENT_SECTION.currentGameNode!=NULL) {
		nullUpdate=0;
		//		updateScreen(CURRENT_SECTION.currentGameNode->data);
	} else {
		nullUpdate=1;
		//		updateScreen(NULL);
	}
	enableKeyRepeat();
	SDL_Event event;

	const int GAME_FPS=60;
	Uint32 start_time;
	// 	startScreenTimer();
	while(running) {
	    start_time=SDL_GetTicks();
		refreshScreen();
		if(1000/GAME_FPS>SDL_GetTicks()-start_time) {
		      SDL_Delay(1000/GAME_FPS-(SDL_GetTicks()-start_time));
		}

//		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );
		updateScreen(nullUpdate==1?NULL:CURRENT_SECTION.currentGameNode->data);
		while (SDL_PollEvent(&event)) {
			start_time=SDL_GetTicks();
			if (currentlyChoosing==3) {
				currRawtime = time(NULL);
				currTime = localtime(&currRawtime);
				int batt = 100;
				if (lastChargeLevel > batt || batt > lastChargeLevel + 1) {
					lastChargeLevel = batt;
					nullUpdate=1;
					//				updateScreen(NULL);
				}
				if(currTime->tm_min!=lastMin) {
					lastMin=currTime->tm_min;
					nullUpdate=1;
					//				updateScreen(NULL);
				}
			}
			if(event.type==getKeyDown()){
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
					nullUpdate=0;
					//				updateScreen(CURRENT_SECTION.currentGameNode->data);
				} else {
					nullUpdate=1;
					//				updateScreen(NULL);
				}
			} else if (event.type==getKeyUp()&&!isUSBMode) {
				displayLogo = 0;
				if(event.key.keysym.sym==BTN_B&&!currentlyChoosing) {
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
						nullUpdate=0;
						//					updateScreen(CURRENT_SECTION.currentGameNode->data);
					} else {
						nullUpdate=1;
						//					updateScreen(NULL);
					}
					aKeyComboWasPressed=0;
				}
				if(event.key.keysym.sym==BTN_SELECT&&!hotKeyPressed) {
					if (CURRENT_SECTION.currentGameNode!=NULL) {
						nullUpdate=0;
						//					updateScreen(CURRENT_SECTION.currentGameNode->data);
					} else {
						nullUpdate=1;
						//					updateScreen(NULL);
					}
				}
			}
			//		if (wasNull) {
			//			updateScreen(NULL);
			//		} else {
			//			updateScreen(CURRENT_SECTION.currentGameNode->data);
			//		}
			//		refreshScreen();
			//	    // set FPS 60
			//	    if(1000/GAME_FPS>SDL_GetTicks()-start_time) {
			//	      SDL_Delay(1000/GAME_FPS-(SDL_GetTicks()-start_time));
			//		}
		}
	}
	quit();
}
