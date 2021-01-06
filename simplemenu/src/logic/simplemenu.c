#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
	MAX_GAMES_IN_SECTION=50000;
	favoritesSectionNumber=0;
	favoritesSize=0;
	favoritesSectionSelected=0;
	favoritesChanged=0;
	FULLSCREEN_ITEMS_PER_PAGE=12;
	MENU_ITEMS_PER_PAGE=10;
	ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
	isPicModeMenuHidden=1;
	autoHideLogos=1;
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
	#if defined(TARGET_NPG) || defined(TARGET_RG350) || defined TARGET_RG350_BETA
	resetFrameBuffer();
	logMessage("INFO","Reset Framebuffer");
	#endif
	createConfigFilesInHomeIfTheyDontExist();
	logMessage("INFO","Validated configuration existence");
	checkThemes();
	logMessage("INFO","Themes checked");
	loadLastState();
	logMessage("INFO","Last state loaded");
	loadConfig();
	logMessage("INFO","Config loaded");
	initializeDisplay();
	logMessage("INFO","Initialized Display");
	char temp[300];
	#if defined(TARGET_BITTBOY) || defined(TARGET_RG300) || defined(TARGET_RG350) || defined(TARGET_RG350_BETA) || defined(TARGET_NPG) || defined(TARGET_PC)
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
	switch (currentMode) {
		case 0:
			if (itemsInSimple>0) {
				fontSize=baseFont;
				currentMode=0;
				MENU_ITEMS_PER_PAGE=itemsInSimple;
				FULLSCREEN_ITEMS_PER_PAGE=itemsInFullSimple;
				break;
			} else {
				fontSize=fontSizeCustom;
				currentMode=3;
				MENU_ITEMS_PER_PAGE=itemsInCustom;
				FULLSCREEN_ITEMS_PER_PAGE=itemsInFullCustom;
				break;
			}
		case 1:
			if (itemsInTraditional>0) {
				fontSize=baseFont-2;
				currentMode=1;
				MENU_ITEMS_PER_PAGE=itemsInTraditional;
				FULLSCREEN_ITEMS_PER_PAGE=itemsInFullTraditional;
				break;
			} else {
				fontSize=fontSizeCustom;
				currentMode=3;
				MENU_ITEMS_PER_PAGE=itemsInCustom;
				FULLSCREEN_ITEMS_PER_PAGE=itemsInFullCustom;
				break;
			}
		case 2:
			if (itemsInDrunkenMonkey>0) {
				fontSize=baseFont-4;
				MENU_ITEMS_PER_PAGE=itemsInDrunkenMonkey;
				FULLSCREEN_ITEMS_PER_PAGE=itemsInFullDrunkenMonkey;
				currentMode=2;
				break;
			} else {
				fontSize=fontSizeCustom;
				currentMode=3;
				MENU_ITEMS_PER_PAGE=itemsInCustom;
				FULLSCREEN_ITEMS_PER_PAGE=itemsInFullCustom;
				break;
			}
		default:
			fontSize=fontSizeCustom;
			currentMode=3;
			MENU_ITEMS_PER_PAGE=itemsInCustom;
			FULLSCREEN_ITEMS_PER_PAGE=itemsInFullCustom;
			break;
	}
	if(fullscreenMode==0) {
		ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
	} else {
		ITEMS_PER_PAGE=FULLSCREEN_ITEMS_PER_PAGE;
	}
	freeFonts();
	initializeFonts();
	initializeSettingsFonts();
	logMessage("INFO","Fonts initialized");
	#if defined(TARGET_BITTBOY) || defined(TARGET_RG300) || defined(TARGET_RG350) || defined(TARGET_RG350_BETA) || defined(TARGET_NPG)
	initSuspendTimer();
	logMessage("INFO","Suspend timer initialized");
	#endif
	logMessage("INFO","Determining starting screen");
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
		if (currentlyChoosing==3) {
			currRawtime = time(NULL);
			currTime = localtime(&currRawtime);
			int batt = getBatteryLevel();
			if (lastChargeLevel > batt || batt > lastChargeLevel + 1) {
				lastChargeLevel = batt;
				updateScreen(NULL);
			}
			if(currTime->tm_min!=lastMin) {
				lastMin=currTime->tm_min;
				updateScreen(NULL);
			}
		}
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
			} else if (getEventType()==getKeyUp()&&!isUSBMode) {
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
					if (CURRENT_SECTION.currentGameNode!=NULL) {
						updateScreen(CURRENT_SECTION.currentGameNode->data);
					} else {
						updateScreen(NULL);
					}
				}
			}
		}
	}
	quit();
}
