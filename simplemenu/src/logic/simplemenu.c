#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
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
//	autoHideLogos=0;
	stripGames=1;
	srand(time(0));
}

void resetFrameBuffer () {
	int ret = system("./scripts/reset_fb");
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
	logMessage("INFO","resetFrameBuffer","Reset Framebuffer");
}

void critical_error_handler()
{
	logMessage("ERROR","critical_error_handler","Nice, a critical error!!!");
	closeLogFile();
	exit(0);
}

void sig_term_handler()
{
	logMessage("WARN","sig_term_handler","Received SIGTERM");
	running=0;
}

void initialSetup() {
	initializeGlobals();
	logMessage("INFO","initialSetup","Initialized Globals");
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
	freeFonts();
	initializeFonts();
	initializeSettingsFonts();
	createThemesInHomeIfTheyDontExist();
	checkThemes();
	loadLastState();
	HW_Init();
	currentCPU = OC_NO;
#ifndef TARGET_OD_BETA
	logMessage("INFO","initialSetup","Setting CPU to base");
	setCPU(currentCPU);
#endif
	setupKeys();
	checkIfDefault();
}

void initialSetup2() {
	char temp[300];
	strcpy(temp,themes[activeTheme]);
	strcat(temp,"/theme.ini");
	logMessage("INFO","initialSetup2","Loading theme");
	loadTheme(temp);
	logMessage("INFO","initialSetup2","Loading section groups");
	loadSectionGroups();
	logMessage("INFO","initialSetup2","Loading sections");
	int sectionCount=loadSections(sectionGroups[activeGroup].groupPath);
	logMessage("INFO","initialSetup2","Loading favorites");
	loadFavorites();
	currentMode=3;
	MENU_ITEMS_PER_PAGE=itemsPerPage;
	FULLSCREEN_ITEMS_PER_PAGE=itemsPerPageFullscreen;
	if(fullscreenMode==0) {
		ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
	} else {
		ITEMS_PER_PAGE=FULLSCREEN_ITEMS_PER_PAGE;
	}
	#if defined(TARGET_BITTBOY) || defined(TARGET_RFW) || defined(TARGET_OD) || defined(TARGET_OD_BETA) || defined(TARGET_NPG)
	initSuspendTimer();
	#endif
	determineStartingScreen(sectionCount);
	enableKeyRepeat();
	lastChargeLevel=getBatteryLevel();
}

void processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if(event.type==getKeyDown()){
			if (!isSuspended) {
				switch (currentState) {
					case BROWSING_GAME_LIST:
						performAction(CURRENT_SECTION.currentGameNode);
						break;
					case SELECTING_SECTION:
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
					case HELP_SCREEN_1:
						performHelpAction();
						break;
					case HELP_SCREEN_2:
						performHelpAction();
						break;
					case AFTER_RUNNING_LAUNCH_AT_BOOT:
						performLaunchAtBootQuitScreenChoosingAction();
						break;
				}
			}
			resetScreenOffTimer();
			if (currentState!=AFTER_RUNNING_LAUNCH_AT_BOOT) {
				refreshRequest=1;
//				updateScreen(CURRENT_SECTION.currentGameNode);
//				refreshScreen();
			}
		} else if (event.type==getKeyUp()) {
			if (currentState==BROWSING_GAME_LIST &&  previousState != SETTINGS_SCREEN && previousState != SELECTING_EMULATOR ) {
				if(((int)event.key.keysym.sym)==BTN_B) {
					if (!aKeyComboWasPressed&&currentSectionNumber!=favoritesSectionNumber) {
						currentState=SELECTING_SECTION;
					}
					hotKeyPressed=0;
					if(fullscreenMode) {
						if(currentState==SELECTING_SECTION) {
	//						hideFullScreenModeMenu();
						} else if (CURRENT_SECTION.alphabeticalPaging) {
							resetPicModeHideMenuTimer();
						}
					}
					CURRENT_SECTION.alphabeticalPaging=0;
					if (aKeyComboWasPressed) {
						currentState=BROWSING_GAME_LIST;
					}
					aKeyComboWasPressed=0;
					if (currentState!=AFTER_RUNNING_LAUNCH_AT_BOOT) {
						refreshRequest=1;
//						updateScreen(CURRENT_SECTION.currentGameNode);
//						refreshScreen();
					}
				}
			} else 	if (currentState==SELECTING_SECTION) {
				if(((int)event.key.keysym.sym)==BTN_B) {
					if (aKeyComboWasPressed==0) {
						if (currentSectionNumber!=favoritesSectionNumber&&sectionGroupCounter>1) {
							beforeTryingToSwitchGroup = activeGroup;
							currentState=CHOOSING_GROUP;
						}
					} else {
						hotKeyPressed=0;
						aKeyComboWasPressed=0;
						CURRENT_SECTION.alphabeticalPaging=0;
						currentState=BROWSING_GAME_LIST;
					}
					if (currentState!=AFTER_RUNNING_LAUNCH_AT_BOOT) {
						refreshRequest=1;
//						updateScreen(CURRENT_SECTION.currentGameNode);
//						refreshScreen();
					}
				}
			}
			previousState = currentState;
		}
//#if defined (TARGET_BITTBOY)
//		else if (event.type==getKeyUp()&&currentState==SELECTING_SECTION) {
//			if(aKeyComboWasPressed==1&&((int)event.key.keysym.sym)==BTN_B) {
//				hotKeyPressed=0;
//				CURRENT_SECTION.alphabeticalPaging=0;
//				aKeyComboWasPressed=0;
//				currentState=BROWSING_GAME_LIST;
//				if (currentState!=AFTER_RUNNING_LAUNCH_AT_BOOT) {
//					updateScreen(CURRENT_SECTION.currentGameNode);
//					refreshScreen();
//				}
//			}
//		}
//#endif
//		else if (event.type==SDL_MOUSEMOTION) {
			if (currentState==BROWSING_GAME_LIST_AFTER_TIMER) {
				loadGameList(0);
				currentState=BROWSING_GAME_LIST;
			}
			if (currentState!=AFTER_RUNNING_LAUNCH_AT_BOOT) {
//				updateScreen(CURRENT_SECTION.currentGameNode);
//				refreshScreen();
			}
//		}
//		break;
	}
}

int main() {
	logMessage("INFO","main","Setup 1");
	initialSetup();
	logMessage("INFO","main","Setup 2");
	initialSetup2();
	logMessage("INFO","main","Checking launch at boot");
	struct AutostartRom *launchAtBootGame = getLaunchAtBoot();
	if (launchAtBootGame!=NULL) {
		if (wasRunningFlag()) {
			currentState=AFTER_RUNNING_LAUNCH_AT_BOOT;
			resetShutdownTimer();
		} else {
			logMessage("INFO","main","Launching at boot");
			launchAutoStartGame(launchAtBootGame->rom, launchAtBootGame->emulatorDir, launchAtBootGame->emulator);
		}
	} else {
		currentState=BROWSING_GAME_LIST;
		pushEvent();
	}
	const int GAME_FPS=60;
	const int FRAME_DURATION_IN_MILLISECONDS = 1000/GAME_FPS;
	Uint32 start_time;
	updateScreen(CURRENT_SECTION.currentGameNode);
	refreshScreen();
	startBatteryTimer();
	while(running) {
		start_time=SDL_GetTicks();
		processEvents();
		if(refreshRequest) {
			updateScreen(CURRENT_SECTION.currentGameNode);
			refreshRequest=0;
			refreshScreen();
		}
//		//Time spent on one loop
		int timeSpent = SDL_GetTicks()-start_time;
		//If it took less than a frame
		if(timeSpent < FRAME_DURATION_IN_MILLISECONDS) {
			//Wait the remaining time until one frame completes
			SDL_Delay(FRAME_DURATION_IN_MILLISECONDS-timeSpent);
		}
	}
	int notDefaultButTryingToRebootOrShutDown = (shutDownEnabled==0&&(selectedShutDownOption==1||selectedShutDownOption==2));
	if(shutDownEnabled||notDefaultButTryingToRebootOrShutDown) {
		currentState=SHUTTING_DOWN;
		updateScreen(CURRENT_SECTION.currentGameNode);
		refreshScreen();
		sleep(1);
	}
	quit();
}
