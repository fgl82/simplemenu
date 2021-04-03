#include <stdlib.h>
#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/utils.h"
#include "../headers/system_logic.h"

int performAction(struct Node *node) {
	struct Rom *rom;
	if (node!=NULL) {
		rom = node->data;
	} else {
		rom = NULL;
	}
	if(currentState==SELECTING_SECTION) {
		if (keys[BTN_A]) {
			if (CURRENT_SECTION.backgroundSurface==NULL) {
				logMessage("INFO","Loading system background");
				CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
				resizeSectionBackground(&CURRENT_SECTION);
				logMessage("INFO","Loading system picture");
				CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
				resizeSectionSystemPicture(&CURRENT_SECTION);
			}
			loadGameList(0);
			if(CURRENT_SECTION.gameCount>0) {
				scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
			}
			pushEvent();
			currentState=BROWSING_GAME_LIST;
			return 1;
		}
		if (keys[BTN_START]) {
			currentState=SETTINGS_SCREEN;
			chosenSetting=SHUTDOWN_OPTION;
			selectedShutDownOption=0;
			currRawtime = time(NULL);
			currTime = localtime(&currRawtime);
			lastMin=currTime->tm_min;
			lastChargeLevel = getBatteryLevel();
//			pthread_create(&clockThread, NULL, updateClock,NULL);
			return 1;
		}
	}
	if (rom!=NULL&&keys[BTN_R1]&&keys[BTN_B]) {
		hideFullScreenModeMenu();
		if(currentSectionNumber!=favoritesSectionNumber) {
			loadGameList(1);
			return(1);
		}
	}
	if (keys[BTN_START]&&isUSBMode) {
		hotKeyPressed=0;
		isUSBMode=0;
		int ret = system("./usb_mode_off.sh");
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		return 0;
	}
	if(itsStoppedBecauseOfAnError&&!keys[BTN_A]) {
		return(0);
	}
	if(keys[BTN_B]) {
		hotKeyPressed=1;
		if (currentState==BROWSING_GAME_LIST) {
			if (keys[BTN_START]&&currentState!=SELECTING_SECTION) {
				hotKeyPressed=0;
				cycleFrequencies();
				aKeyComboWasPressed=1;
				return 0;
			}
			if (rom!=NULL&&keys[BTN_A]) {
				launchEmulator(rom);
				aKeyComboWasPressed=1;
				return 1;
			}
			if (rom!=NULL&&keys[BTN_X]) {
				if (!isPicModeMenuHidden) {
					resetPicModeHideMenuTimer();
				}
				callDeleteGame(rom);
				aKeyComboWasPressed=1;
				return 1;
			}
			if (keys[BTN_START]) {
				hotKeyPressed=0;
				int returnedValue = system("./usb_mode_on.sh");
				if (returnedValue==0) {
					isUSBMode = 1;
				} else {
					generateError("USB MODE  NOT AVAILABLE",0);
				}
				aKeyComboWasPressed=1;
			}
			if (rom!=NULL&&keys[BTN_SELECT]) {
				for(int i=0;i<25;i++) {
					selectRandomGame();
				}
				saveFavorites();
				launchGame(CURRENT_SECTION.currentGameNode->data);
			}
			if (rom!=NULL&&keys[BTN_DOWN]) {
				hotKeyPressed=1;
				CURRENT_SECTION.alphabeticalPaging=1;
				advancePage(rom);
				if(fullscreenMode) {
					resetPicModeHideMenuTimer();
				}
				aKeyComboWasPressed=1;
				return 0;
			}
			if (rom!=NULL&&keys[BTN_UP]) {
				hotKeyPressed=1;
				CURRENT_SECTION.alphabeticalPaging=1;
				rewindPage(rom);
				if(fullscreenMode) {
					resetPicModeHideMenuTimer();
				}
				aKeyComboWasPressed=1;
				return 0;
			}
		}
		if (currentState == BROWSING_GAME_LIST || (fullscreenMode)) {
			if(keys[BTN_RIGHT]) {
				currentState=fullscreenMode==1?SELECTING_SECTION:BROWSING_GAME_LIST;
				hotKeyPressed=0;
				int advanced = advanceSection(0);
				if(advanced) {
					if (CURRENT_SECTION.backgroundSurface == NULL) {
						logMessage("INFO","Loading system background");
						CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
						resizeSectionBackground(&CURRENT_SECTION);
						CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
						resizeSectionSystemPicture(&CURRENT_SECTION);
					}
					loadGameList(0);
				}
				if(CURRENT_SECTION.gameCount>0) {
					scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
				}
				aKeyComboWasPressed=1;
				return 0;
			}
			if(keys[BTN_LEFT]) {
				currentState=fullscreenMode==1?SELECTING_SECTION:BROWSING_GAME_LIST;
				hotKeyPressed=0;
				int rewinded = rewindSection(0);
				if(rewinded) {
					if (CURRENT_SECTION.backgroundSurface == NULL) {
						logMessage("INFO","Loading system background");
						CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
						resizeSectionBackground(&CURRENT_SECTION);
						logMessage("INFO","Loading system picture");
						CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
						resizeSectionSystemPicture(&CURRENT_SECTION);
					}
					loadGameList(0);
				}
				if(CURRENT_SECTION.gameCount>0) {
					scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
				}
				aKeyComboWasPressed=1;
				return 0;
			}
		}
	}
	if (CURRENT_SECTION.executables[1]!=NULL&&keys[BTN_SELECT]&&!favoritesSectionSelected) {
		currentState=SELECTING_EMULATOR;
		return 0;
	}
	if(keys[BTN_L1]) {
		currentState=SELECTING_SECTION;
		hotKeyPressed=0;
		if(currentSectionNumber!=favoritesSectionNumber&&autoHideLogos) {
			resetPicModeHideLogoTimer();
		}
		rewindSection(1);
		return 0;
	}

	if(keys[BTN_R1]) {
		printf("1\n");
		currentState=SELECTING_SECTION;
		hotKeyPressed=0;
		if(currentSectionNumber!=favoritesSectionNumber&&autoHideLogos) {
			resetPicModeHideLogoTimer();
		}
		advanceSection(1);
		return 0;
	}

	if (currentState!=SELECTING_EMULATOR&&!hotKeyPressed&&!isUSBMode) {

		if (rom!=NULL&&keys[BTN_X]) {
			if(!isPicModeMenuHidden) {
				resetPicModeHideMenuTimer();
			}
			if (!favoritesSectionSelected) {
				markAsFavorite(rom);
			} else {
				removeFavorite();
				if(favoritesSize==0) {
					showOrHideFavorites();
				}
			}
			return 0;
		}
		if (keys[BTN_START]) {
			chosenSetting=SHUTDOWN_OPTION;
			selectedShutDownOption=0;
			currentState=3;
			currRawtime = time(NULL);
			currTime = localtime(&currRawtime);
			lastMin=currTime->tm_min;
			lastChargeLevel = getBatteryLevel();
//			pthread_create(&clockThread, NULL, updateClock,NULL);
			return 0;
		}
		if (keys[BTN_R]) {
			showOrHideFavorites();
			return 0;
		}
		if (rom!=NULL&&keys[BTN_A]) {
			if(itsStoppedBecauseOfAnError) {
				if(thereIsACriticalError) {
					#ifndef TARGET_PC
					running=0;
					#else
					freeResources();
					saveLastState();
					saveFavorites();
					exit(0);
					#endif
				}
				itsStoppedBecauseOfAnError=0;
				return 0;
			}
			if (countGamesInPage()>0) {
				saveFavorites();
				launchGame(rom);
			}
			return 0;
		}
		if (keys[BTN_Y]) {
			int number = CURRENT_GAME_NUMBER;
			if (fullscreenMode) {
				fullscreenMode=0;
				ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
 			} else {
				fullscreenMode=1;
				ITEMS_PER_PAGE=FULLSCREEN_ITEMS_PER_PAGE;
			}
			if (CURRENT_SECTION.gameCount>0) {
				scrollToGame(number);
			}
		}
		if (rom!=NULL&&keys[BTN_DOWN]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			scrollDown();
			return 1;
		}
		if(rom!=NULL&&keys[BTN_UP]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			scrollUp();
			return 1;
		}
		if(rom!=NULL&&keys[BTN_RIGHT]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			advancePage(rom);
			return 1;
		}
		if(rom!=NULL&&keys[BTN_LEFT]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			rewindPage(rom);
			return 1;
		}
	}
	return 0;
}
