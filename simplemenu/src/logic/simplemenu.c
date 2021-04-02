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
	logMessage("INFO","Reset Framebuffer");
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

void initialSetup() {
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
	#endif
	createConfigFilesInHomeIfTheyDontExist();
	loadConfig();
	initializeDisplay();
	checkThemes();
	loadLastState();
	HW_Init();
	currentCPU = OC_NO;
	setCPU(OC_NO);
	setupDisplayAndKeys();
	checkIfDefault();
	char temp[300];
	strcpy(temp,themes[activeTheme]);
	strcat(temp,"/theme.ini");
	logMessage("INFO","Loading theme");
	loadTheme(temp);
	loadSectionGroups();
	int sectionCount=loadSections(sectionGroups[activeGroup].groupPath);
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
	#if defined(TARGET_BITTBOY) || defined(TARGET_RFW) || defined(TARGET_OD) || defined(TARGET_OD_BETA) || defined(TARGET_NPG)
	initSuspendTimer();
	#endif
	determineStartingScreen(sectionCount);
	while(strlen(CURRENT_SECTION.sectionName)<1) {
		advanceSection(0);
	}
	enableKeyRepeat();
}

int main() {
	initialSetup();
	SDL_Event event;
	const int GAME_FPS=60;
	const int FRAME_DURATION_IN_MILLISECONDS = 1000/GAME_FPS;
	Uint32 start_time;
	while(running) {
	    start_time=SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			if(event.type==getKeyDown()){
				if (!isSuspended) {
					switch (currentState) {
						case BROWSING_GAME_LIST:
							performAction(CURRENT_SECTION.currentGameNode);
							break;
						case SELECTING_EMULATOR:
							performChoosingAction();
							break;
						case CHOOSING_GROUP:
							performGroupChoosingAction();
							break;
						case SETTINGS_SCREEN:
							performSettingsChoosingAction();
							break;
					}
				}
				resetScreenOffTimer();
			} else if (event.type==getKeyUp()&&!isUSBMode) {
				if(event.key.keysym.sym==BTN_B&&!(currentState>BROWSING_GAME_LIST)) {
					if (!currentlySectionSwitching) {
						if (!aKeyComboWasPressed&&currentSectionNumber!=favoritesSectionNumber&&sectionGroupCounter>1) {
							beforeTryingToSwitchGroup = activeGroup;
							currentState=2;
						}
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
					aKeyComboWasPressed=0;
				}
			}
		}
		updateScreen(CURRENT_SECTION.currentGameNode);
		refreshScreen();
		//Time spent on one loop
		int timeSpent = SDL_GetTicks()-start_time;
		//If it took less than a frame
		if(timeSpent < FRAME_DURATION_IN_MILLISECONDS) {
			//Wait the remaining time until one frame completes
		    SDL_Delay(FRAME_DURATION_IN_MILLISECONDS-timeSpent);
		}
	}
	quit();
}
